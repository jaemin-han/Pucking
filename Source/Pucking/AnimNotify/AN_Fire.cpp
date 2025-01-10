// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Fire.h"

#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/FireInterface.h"

void UAN_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::Notify(MeshComp, Animation);
	
	if(!MeshComp->GetOwner()) return;
	
	AActor* OwnerActor = MeshComp->GetOwner();
	TArray<UActorComponent*> FireActorComponents = OwnerActor->GetComponentsByInterface(UFireInterface::StaticClass());
	
	for(UActorComponent* FireActorComponent : FireActorComponents)
	{
		// X는 Actor 기준, Y, Z는 카메라 기준
		if(IFireInterface* OwnerFireInterface = Cast<IFireInterface>(FireActorComponent))
		{
			// Fire의 LineTrace 기준점은 스프링암 기준
			if (USpringArmComponent* SpringArmComponent = MeshComp->GetOwner()->FindComponentByClass<USpringArmComponent>())
			{
				// SpringArm의 ForwardVector가 고정이므로 자식인 카메라 컴포넌트 사용
				FVector ForwardVector = FVector::ZeroVector;
				if(SpringArmComponent->GetChildComponent(0))
				{
					ForwardVector = SpringArmComponent->GetChildComponent(0)->GetForwardVector();
				}
				
				FVector OriginStartLoc = SpringArmComponent->GetComponentLocation();

				// 위치는 스프링암 기준, ForwardVector는 카메라 기준
				OwnerFireInterface->Fire(OriginStartLoc, ForwardVector);
			}
		}
	}

}
