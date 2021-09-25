// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/DataAsset/TrackDataAsset/TrackDataAsset.h"

#include "Particles/ParticleSystem.h"

UParticleSystem* FTrackData::GetParticleSystem()
{
	UParticleSystem * R_ParticleSystem = Cast<UParticleSystem>(ParticleSystemSoftClassPtr.LoadSynchronous());
	return R_ParticleSystem;
}

UParticleSystem* FTrackData::GetSprayParticleSystem()
{
	UParticleSystem * R_ParticleSystem = Cast<UParticleSystem>(ParticleSystemSpraySoftClassPtr.LoadSynchronous());
	return R_ParticleSystem;
}

void FTrackData::GetRotation(FRotator& NewRotation)
{
	NewRotation = Rotation;	
}

UParticleSystem * UTrackDataAsset::GetParticle(const TEnumAsByte<EPhysicalSurface> CurrentSurface)
{
	UParticleSystem * R_ParticleSystem = nullptr;
	for (int I=0; I < TrackDatas.Num(); I++)
	{
		if(TrackDatas[I].SurfaceType == CurrentSurface)
		{
			R_ParticleSystem =	TrackDatas[I].GetParticleSystem();
			break;
		}
	}
	return R_ParticleSystem;
}

UParticleSystem* UTrackDataAsset::GetSprayParticle(const TEnumAsByte<EPhysicalSurface> CurrentSurface)
{
	UParticleSystem * R_ParticleSystem = nullptr;
	for (int I=0; I < TrackDatas.Num(); I++)
	{
		if(TrackDatas[I].SurfaceType == CurrentSurface)
		{
			R_ParticleSystem =	TrackDatas[I].GetSprayParticleSystem();
			break;
		}
	}
	return R_ParticleSystem;
}

void UTrackDataAsset::GetRotation(const TEnumAsByte<EPhysicalSurface> CurrentSurface, FRotator& NewRotation)
{
	for (int I=0; I < TrackDatas.Num(); I++)
	{
		if(TrackDatas[I].SurfaceType == CurrentSurface)
		{
			TrackDatas[I].GetRotation(NewRotation);
			return;
		}
	}
}
