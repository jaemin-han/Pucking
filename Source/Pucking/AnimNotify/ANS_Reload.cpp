// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Reload.h"

#include "Interfaces/GetActorCompMap.h"
#include "Interfaces/ReloadInterface.h"

class IGetActorCompMap;

void UANS_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
}

void UANS_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::NotifyEnd(MeshComp, Animation);

	if(!MeshComp->GetOwner()) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	
	if(IGetActorCompMap* GetOwnerActorComponents = Cast<IGetActorCompMap>(OwnerActor))
	{
		TArray<UActorComponent*> ReloadActorComponents = GetOwnerActorComponents->ReturnActorComponents(FName("ReloadInterface"));
		for(auto ReloadActorComponent : ReloadActorComponents)
		{
			IReloadInterface* OwnerReloadInterface = Cast<IReloadInterface>(ReloadActorComponent);
			if(OwnerReloadInterface)
			{
				OwnerReloadInterface->Reload();
			}
		}
	}
	
}
