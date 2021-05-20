// Copyright 2020 Sabre Dart Studios

#pragma once

#include "CoreMinimal.h"

#include "EtherlinkerTypes.h"
#include "Misc/EnumRange.h"
#include "Components/ActorComponent.h"

#include "EthereumTerminalComponent.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EEthOperationType : uint8 
{
	 getWalletBalance   UMETA(DisplayName = "getWalletBalance")
	,execContractMethod UMETA(DisplayName = "execContractMethod")
	,deployContract     UMETA(DisplayName = "deployContract")
	,createWallet       UMETA(DisplayName = "createWallet") 
	,transferEther      UMETA(DisplayName = "transferEther" )
	,EMPTY				UMETA(DisplayName = "empty")
	
};


UENUM(BlueprintType, Blueprintable)
enum class EEthereumParamTypes : uint8
{
	 String       UMETA(DisplayName = "String")
	,Number	      UMETA(DisplayName = "Number")
	,ETH	      UMETA(DisplayName = "ETH")
	,Boolean      UMETA(DisplayName = "Boolean")
	,Bytes        UMETA(DisplayName = "Bytes")
	,StringArray  UMETA(DisplayName = "StringArray")
	,BooleanArray UMETA(DisplayName = "BooleanArray")
	,NumberArray  UMETA(DisplayName = "NumberArray")
	,Address	  UMETA(DisplayName = "Address")
	
};
ENUM_RANGE_BY_COUNT(EEthereumParamTypes, EEthereumParamTypes::Address);

//ENUM_RANGE_BY_FIRST_AND_LAST(EEthereumParamTypes, EEthereumParamTypes::String, EEthereumParamTypes::Address );

USTRUCT(BlueprintType)
struct FZSEtherlinkerRequestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SenderID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WalletAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ContractAddress;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ContractName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ContractMethodName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ContractMethodParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EEthereumParamTypes> ContractMethodParamTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ConvertResultFromWeiToEth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GasPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GasLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InfuraURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReceiverAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EthAmountToSend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEthOperationType OperationType;
	
	FEtherlinkerRequestData GetEtherlinkerRequestData() const;
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSPACE_API UEthereumTerminalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEthereumTerminalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	class UEthereumTerminalDataAsset * EthereumTerminalDataAsset = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true))
	FZSEtherlinkerRequestData EtherlinkerRequestData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	uint8 bCanBeUsed:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	uint8 bCallCompleted:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	uint8 bUseCurrentWalletAddressAsFirstParameter:1;

	class UZSEthereumActorComponent * GetZsEthereumActorComponent(class APawn * NewInteractor);
	
public:

	void SetZSEtherlinkerRequestData();

	UFUNCTION()
	void ResponseReceived();
		

	UFUNCTION(BlueprintCallable)	
	void Use(class APawn * NewInteractor);
};
