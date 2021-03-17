// Copyright 2018 Sabre Dart Studios

#include "OWSCharacterMovementComponent.h"

UOWSCharacterMovementComponent::UOWSCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	IsSprinting = false;
	MaxMaxWalkSpeed = 2000.f;
}


void UOWSCharacterMovementComponent::StartSprinting()
{
	bRequestToStartSprinting = true;
}

void UOWSCharacterMovementComponent::StopSprinting()
{
	bRequestToStartSprinting = false;
}

float UOWSCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (IsMovingForward() && bRequestToStartSprinting)
	{
		MaxSpeed *= SprintSpeedMultiplier;
	}

	return MaxSpeed;
}

float UOWSCharacterMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();

	if (IsMovingForward() && bRequestToStartSprinting)
	{
		MaxAccel *= SprintAccelerationMultiplier;
	}

	return MaxAccel;
}

bool UOWSCharacterMovementComponent::IsMovingForward() const
{
	if (!PawnOwner)
	{
		return false;
	}

	FVector Forward = PawnOwner->GetActorForwardVector();
	FVector TempMoveDirection = Velocity.GetSafeNormal();

	//Ignore vertical movement
	Forward.Z = 0.0f;
	TempMoveDirection.Z = 0.0f;

	float VelocityDot = FVector::DotProduct(Forward, TempMoveDirection);
	return VelocityDot > 0.7f; //Check to make sure difference between headings is not too great.
}

void UOWSCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	//Set Max Walk Speed
	if (bRequestMaxWalkSpeedChange)
	{
		bRequestMaxWalkSpeedChange = false;
		MaxWalkSpeed = MyNewMaxWalkSpeed;
	}

	//Dodge
	if (bWantsToDodge)
	{
		bWantsToDodge = false;

		//Only dodge if on the ground (in the air causes problems trying to get the two modes to line up due to friction)
		if (IsMovingOnGround())
		{
			MoveDirection.Normalize();
			FVector DodgeVelocity = MoveDirection * DodgeStrength;
			//Set Z component to zero so we don't go up
			DodgeVelocity.Z = 0.0f;

			Launch(DodgeVelocity);
		}
	}
}


bool UOWSCharacterMovementComponent::HandlePendingLaunch()
{
	if (!PendingLaunchVelocity.IsZero() && HasValidData())
	{
		Velocity = PendingLaunchVelocity;
		//Remmed out so our dodge move won't play the falling animation.
		//SetMovementMode(MOVE_Falling);
		PendingLaunchVelocity = FVector::ZeroVector;
		bForceNextFloorCheck = true;
		return true;
	}

	return false;
}

//============================================================================================
//Replication
//============================================================================================

//Set input flags on character from saved inputs
void UOWSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	bRequestToStartSprinting = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
	bWantsToDodge = (Flags&FSavedMove_Character::FLAG_Custom_1) != 0;
	bRequestMaxWalkSpeedChange = (Flags&FSavedMove_Character::FLAG_Custom_2) != 0;
}

class FNetworkPredictionData_Client* UOWSCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	check(PawnOwner->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UOWSCharacterMovementComponent* MutableThis = const_cast<UOWSCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_OWS(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UOWSCharacterMovementComponent::FSavedMove_OWS::Clear()
{
	Super::Clear();

	bSavedRequestToStartSprinting = false;
	bSavedRequestMaxWalkSpeedChange = false;
	SavedWalkSpeed = 0.f;
	bSavedWantsToDodge = false;
	SavedMoveDirection = FVector::ZeroVector;
}

uint8 UOWSCharacterMovementComponent::FSavedMove_OWS::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}
	
	if (bSavedWantsToDodge)
	{
		Result |= FLAG_Custom_1;
	}

	if (bSavedRequestMaxWalkSpeedChange)
	{
		Result |= FLAG_Custom_2;
	}
	
	return Result;
}

bool UOWSCharacterMovementComponent::FSavedMove_OWS::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (bSavedRequestToStartSprinting != ((FSavedMove_OWS*)&NewMove)->bSavedRequestToStartSprinting)
	{
		return false;
	}
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_OWS*)&NewMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}
	if (SavedWalkSpeed != ((FSavedMove_OWS*)&NewMove)->SavedWalkSpeed)
	{
		return false;
	}
	if (bSavedWantsToDodge != ((FSavedMove_OWS*)&NewMove)->bSavedWantsToDodge)
	{
		return false;
	}
	if (SavedMoveDirection != ((FSavedMove_OWS*)&NewMove)->SavedMoveDirection)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UOWSCharacterMovementComponent::FSavedMove_OWS::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UOWSCharacterMovementComponent* CharacterMovement = Cast<UOWSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestToStartSprinting = CharacterMovement->bRequestToStartSprinting;
		bSavedRequestMaxWalkSpeedChange = CharacterMovement->bRequestMaxWalkSpeedChange;
		SavedWalkSpeed = CharacterMovement->MyNewMaxWalkSpeed;
		bSavedWantsToDodge = CharacterMovement->bWantsToDodge;
		SavedMoveDirection = CharacterMovement->MoveDirection;
	}
}

void UOWSCharacterMovementComponent::FSavedMove_OWS::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UOWSCharacterMovementComponent* CharacterMovement = Cast<UOWSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->MoveDirection = SavedMoveDirection;
		CharacterMovement->MyNewMaxWalkSpeed = SavedWalkSpeed;
	}
}

UOWSCharacterMovementComponent::FNetworkPredictionData_Client_OWS::FNetworkPredictionData_Client_OWS(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UOWSCharacterMovementComponent::FNetworkPredictionData_Client_OWS::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_OWS());
}


//Set Max Walk Speed RPC to transfer the current Max Walk Speed from the Owning Client to the Server
bool UOWSCharacterMovementComponent::Server_SetMaxWalkSpeed_Validate(const float NewMaxWalkSpeed)
{
	if (NewMaxWalkSpeed < 0.f || NewMaxWalkSpeed > MaxMaxWalkSpeed)
		return false;
	else
		return true;
}

void UOWSCharacterMovementComponent::Server_SetMaxWalkSpeed_Implementation(const float NewMaxWalkSpeed)
{
	MyNewMaxWalkSpeed = NewMaxWalkSpeed;
}

void UOWSCharacterMovementComponent::SetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		MyNewMaxWalkSpeed = NewMaxWalkSpeed;
		Server_SetMaxWalkSpeed(NewMaxWalkSpeed);
	}

	bRequestMaxWalkSpeedChange = true;
}

//Dodge RPC to transfer the current Move Direction from the Owning Client to the Server
bool UOWSCharacterMovementComponent::Server_MoveDirection_Validate(const FVector& MoveDir)
{
	return true;
}

void UOWSCharacterMovementComponent::Server_MoveDirection_Implementation(const FVector& MoveDir)
{
	MoveDirection = MoveDir;
}

//Trigger the Dodge ability on the Owning Client
void UOWSCharacterMovementComponent::DoDodge()
{
	if (PawnOwner->IsLocallyControlled())
	{
		MoveDirection = PawnOwner->GetLastMovementInputVector();
		Server_MoveDirection(MoveDirection);
	}

	bWantsToDodge = true;
}