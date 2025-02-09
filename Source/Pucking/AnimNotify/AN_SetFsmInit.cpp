// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SetFsmInit.h"

#include "Common/CommonEnum.h"
#include "Interfaces/MontageFSMInterface.h"

void UAN_SetFsmInit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::Notify(MeshComp, Animation);

	if(MeshComp->GetAnimInstance())
	{
		if(IMontageFSMInterface* MontageFsm = Cast<IMontageFSMInterface>(MeshComp->GetAnimInstance()))
		{
			UE_LOG(LogTemp, Error, TEXT("초기화"));
			MontageFsm->ReceiveFsm(ECharacterFSM::Idle);
		}
	}
}
