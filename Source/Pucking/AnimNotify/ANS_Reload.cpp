// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Reload.h"
#include "Interfaces/ReloadInterface.h"

void UANS_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
}

void UANS_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::NotifyEnd(MeshComp, Animation);

	AActor* OwnerActor = MeshComp->GetOwner();
	if(OwnerActor)
	{
		IReloadInterface* OwnerReloadInterface = Cast<IReloadInterface>(OwnerActor);
		if(OwnerReloadInterface)
		{
			OwnerReloadInterface->Reload();
		}
	}
	
}
