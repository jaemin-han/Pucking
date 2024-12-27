// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Fire.h"

#include "Camera/CameraComponent.h"
#include "Interfaces/FireInterface.h"

void UAN_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::Notify(MeshComp, Animation);

	AActor* OwnerActor = MeshComp->GetOwner();
	if(OwnerActor)
	{
		// Actor가 FireInterface를 가지고 있는지 확인
		IFireInterface* OwnerFireInterface = Cast<IFireInterface>(OwnerActor);
		if(OwnerFireInterface)
		{
			// Fire의 LineTrace 기준점은 카메라
			UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>();
			if(CameraComponent)
			{
				FVector StartLoc = CameraComponent->GetComponentLocation();
				FVector CameraForwardVector = CameraComponent->GetForwardVector();
				
				OwnerFireInterface->Fire(StartLoc, CameraForwardVector);	
			}
		}
	}
}
