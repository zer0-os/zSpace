// Copyright 2020 Sabre Dart Studios


#include "Game/WheeledVehiclePawn/DataAsset/TrackDataAsset/TrackDataAsset.h"

#include "Particles/ParticleSystem.h"

UParticleSystem* FTrackData::GetParticleSystem()
{
	UParticleSystem * R_ParticleSystem = Cast<UParticleSystem>(ParticleSystemSoftClassPtr.LoadSynchronous());
	return R_ParticleSystem;
}

UParticleSystem * UTrackDataAsset::GetParticle(TEnumAsByte<EPhysicalSurface> CurrentSurface)
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
