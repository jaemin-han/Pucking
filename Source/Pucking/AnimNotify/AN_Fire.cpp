// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Fire.h"

#include "Camera/CameraComponent.h"
#include "Interfaces/FireInterface.h"
#include "Interfaces/GetActorCompMap.h"

void UAN_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::Notify(MeshComp, Animation);

	if(!MeshComp->GetOwner()) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if(IGetActorCompMap* GetOwnerActorComponents = Cast<IGetActorCompMap>(OwnerActor))
	{
		TArray<UActorComponent*> FireActorComponents = GetOwnerActorComponents->ReturnActorComponents(FName("FireInterface"));
		for(auto FireActorComponent : FireActorComponents)
		{
			IFireInterface* OwnerFireInterface = Cast<IFireInterface>(FireActorComponent);
			if(OwnerFireInterface)
			{
				// Fire의 LineTrace 기준점은 카메라
				if (UCameraComponent* CameraComponent = MeshComp->GetOwner()->FindComponentByClass<UCameraComponent>())
				{
					FVector OriginStartLoc = CameraComponent->GetComponentLocation();
	
					OriginStartLoc.X +=  MeshComp->GetOwner()->GetActorLocation().X - OriginStartLoc.X;
					OriginStartLoc.Y += - 40;
					
					OwnerFireInterface->Fire(OriginStartLoc, CameraComponent->GetForwardVector());
				}
			}
		}
	}

}
