// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/JetPackMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/PuckAnimInstance.h"
#include "Common/CommonStruct.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/MontageFSMInterface.h"
#include "UI/HUD/SubUI/SubCoolTimeUI.h"

// Sets default values for this component's properties
UJetPackMovementComponent::UJetPackMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurEnergy = TotalEnergy;
}


// Called when the game starts
void UJetPackMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitSettings();
}


// Called every frame
void UJetPackMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
	if(!OwnerCharacter) return;

	if(bIsFlyingCool) {
		StartCooling(DeltaTime);
		return;
	}
	FVector UpVector = OwnerCharacter->GetActorUpVector();

	if(bIsFlying)
	{
		// 상승
		OwnerCharacter->AddMovementInput(UpVector, UpFlyingForce);
	}
	else
	{
		// 하강
		OwnerCharacter->AddMovementInput(UpVector, DownFlyingForce * -1);
	}

	// JetPack 에너지 증감
	ManageJetPackEnergy(bIsFlying);

	// 캐릭터 Movement에 따라 변수 수정
	SetInputParam();

	// 캐릭터 Flying 애니메이션
	if(CharAnimInstance)
	{
		SciJetpackInputs(CharAnimInstance, InputForwardValue, InputRightValue, InputTurnValue, InputUpValue);
	}
}

TArray<struct FInputParameter> UJetPackMovementComponent::ReturnInputParameter()
{
	if(JetPackInputContext)
	{
		// 초기화
		InputParameters.Empty();
		
		// Fire
		if(FlyingInputAction)
		{
			FInputParameter StartFlyingInputParameter;
			
			StartFlyingInputParameter.TargetClass = this;
			StartFlyingInputParameter.TriggerEvent = ETriggerEvent::Started;
			//StartFlyingInputParameter.TriggerEvent = ETriggerEvent::Triggered;
			StartFlyingInputParameter.InputMappingContext = JetPackInputContext;
			StartFlyingInputParameter.InputAction = FlyingInputAction;
			StartFlyingInputParameter.CallbackFunc = FName("Input_StartFlying");

			InputParameters.Add(StartFlyingInputParameter);

			FInputParameter StopFlyingInputParameter;
			
			StopFlyingInputParameter.TargetClass = this;
			StopFlyingInputParameter.TriggerEvent = ETriggerEvent::Completed;
			StopFlyingInputParameter.InputMappingContext = JetPackInputContext;
			StopFlyingInputParameter.InputAction = FlyingInputAction;
			StopFlyingInputParameter.CallbackFunc = FName("Input_StopFlying");

			InputParameters.Add(StopFlyingInputParameter);
		}
	}
	
	return InputParameters;
}


void UJetPackMovementComponent::InitSettings()
{
	if(GetOwner())
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
		
		if(OwnerCharacter && OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			//FVector(20, 10, 0)
			FTransform InputTransform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0));
			Equip(OwnerCharacter->GetMesh(), FName("spine_03"), InputTransform);
			
			CharAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	
			if(CharAnimInstance)
			{
				InitBPSetting(CharAnimInstance);

				AnimMontageInstance = Cast<IMontageFSMInterface>(CharAnimInstance);
				if(UPuckAnimInstance* AnumIns = Cast<UPuckAnimInstance>(CharAnimInstance))
				{
					AnumIns->OnChangeFsm.AddDynamic(this, &UJetPackMovementComponent::GetCurrentFsm);
				}
			}
		}
	}
	
}

void UJetPackMovementComponent::SetIsFlying(bool Flying)
{
	this->bIsFlying = Flying;
}

void UJetPackMovementComponent::MoveToFlight()
{
	if(OwnerCharacter && !bIsFlyingCool)
	{
		bool IsCanFlight = ChangeJetpackFsm(ECharacterFSM::JetpackMode);
		if(!IsCanFlight)
		{
			MoveToFailing();	
			return;
		}
		
		SetIsFlying(true);
		
		FRotator CharacterMovementRotator = FRotator::ZeroRotator;
		CharacterMovementRotator.Yaw = FlyingRotationRateZ; 
		
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		//OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		
		OwnerCharacter->GetCharacterMovement()->RotationRate = CharacterMovementRotator;

		// 캐릭터 Flying 애니메이션
		if(CharAnimInstance)
		{
			SciJetpackFlying(CharAnimInstance, true);
		}
	}
}

void UJetPackMovementComponent::MoveToFailing()
{
	if(OwnerCharacter && bIsFlying)
	{
		bool IsCanFlight = ChangeJetpackFsm(ECharacterFSM::Idle);
		if(!IsCanFlight) return;
		
		SetIsFlying(false);

		FRotator CharacterMovementRotator = FRotator::ZeroRotator;
		CharacterMovementRotator.Yaw = WalkingRotationRateZ;
		
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		//OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		
		OwnerCharacter->GetCharacterMovement()->RotationRate = CharacterMovementRotator;

		// 캐릭터 Flying 애니메이션
		if(CharAnimInstance)
		{
			SciJetpackFlying(CharAnimInstance, false);
		}
	}
}

void UJetPackMovementComponent::SetInputParam()
{
	if(GetOwner())
	{
		FVector OwnerVelocity = GetOwner()->GetVelocity();
		FVector OwnerForward = GetOwner()->GetActorForwardVector();
		FVector OwnerRight = GetOwner()->GetActorRightVector();

		// 앞뒤
		if(OwnerVelocity.Dot(OwnerForward) < 0)
		{
			InputForwardValue = -1;
		}
		else if(OwnerVelocity.Dot(OwnerForward) > 0)
		{
			InputForwardValue = 1;
		}
		// 좌우
		if(OwnerVelocity.Dot(OwnerRight) < 0)
		{
			InputRightValue = -1;
		}
		else if(OwnerVelocity.Dot(OwnerRight) > 0)
		{
			InputRightValue = 1;
		}
		
		// 위아래
		if(OwnerCharacter->GetCharacterMovement()->IsFalling())
		{
			InputUpValue = 1;	
		}
		else
		{
			InputUpValue = 0;
		}

		// 회전
		if(OwnerCharacter->GetController())
		{
			if(APlayerController* CharacterPlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
			{
				float MouseX;
				float MouseY;
				CharacterPlayerController->GetMousePosition(MouseX, MouseY);

				InputTurnValue = MouseX;
			}
		}
	}
}

void UJetPackMovementComponent::StartCooling(float DeltaTime)
{
	if(FlyingCoolTime <= CheckCoolTime)
	{
		bIsFlyingCool = false;
		SciJetpackEquip(true);
		CurEnergy = TotalEnergy;
		
		CheckCoolTime = 0.f;
		
		SubCoolTimeUI->SetJetpackReady();
	}
	else
	{	
		if(!SubCoolTimeUI) return;
		
		CheckCoolTime += DeltaTime;
		float CoolTimeRate = 0.f;
		if(CheckCoolTime > 0.f)
		{
			CoolTimeRate = CheckCoolTime / FlyingCoolTime;
		}
		SubCoolTimeUI->SetJetpackGauge(CoolTimeRate);
	}
}

bool UJetPackMovementComponent::ChangeJetpackFsm(ECharacterFSM TargetFsm)
{
	if(AnimMontageInstance)
	{
		bool IsCanChangeFsm = AnimMontageInstance->CheckFsmByEnum(TargetFsm);
		if(IsCanChangeFsm)
		{
			//AnimMontageInstance->ReceiveFsm(TargetFsm);
			return IsCanChangeFsm;
		}
	}
	return false;
}

void UJetPackMovementComponent::GetCurrentFsm(ECharacterFSM TargetFsm)
{
	if(TargetFsm == ECharacterFSM::Hooking || TargetFsm == ECharacterFSM::HookStart)
	{
		MoveToFailing();
	}
}

void UJetPackMovementComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	if(JetPackActorComponent.Get())
	{
		JetPackActor = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass());
		if(JetPackActor)
		{
			JetPackActor->SetChildActorClass(JetPackActorComponent);
			JetPackActor->SetRelativeTransform(ActorTransform);
			JetPackActor->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			JetPackActor->RegisterComponent();
		}
	}
}

void UJetPackMovementComponent::Input_StartFlying(const FInputActionValue& Value)
{
	//ToggleFlight();
	MoveToFlight();
}

void UJetPackMovementComponent::Input_StopFlying(const FInputActionValue& Value)
{
	//ToggleFlight();
	MoveToFailing();
}

void UJetPackMovementComponent::ManageJetPackEnergy(bool Flying)
{
	if(Flying)
	{
		if(0 < CurEnergy)
		{
			CurEnergy -= ChangeEnergy;
		}
	}
	else
	{
		if(CurEnergy < TotalEnergy)
		{
			CurEnergy += ChangeEnergy;
		}
	}

	if(!bIsFlyingCool)
	{
		float CoolTimeRate = CurEnergy / TotalEnergy;
		SubCoolTimeUI->SetJetpackGauge(CoolTimeRate);	
	}

	// 에너지 다 썼을 떄
	if(0 >= CurEnergy)
	{
		// 상태 전환
		MoveToFailing();

		// 쿨다운 시작
		ManageJetPackCoolTime();
	}
}

void UJetPackMovementComponent::ManageJetPackCoolTime()
{
	bIsFlyingCool = true;
	SciJetpackEquip(false);
	// 쿨타임
	/*GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, [this]()
	{
		// 초기화
		bIsFlyingCool = false;
		SciJetpackEquip(true);
		CurEnergy = TotalEnergy;
		
		GetWorld()->GetTimerManager().ClearTimer(CoolTimeHandle);
	}, FlyingCoolTime, false);*/
}

// 제트팩 에너지 강화 옵션
void UJetPackMovementComponent::IncreaseJetpackEnergy()
{
	if(RateJetpackEnergy >= 4) return;
	
	RateJetpackEnergy++;
	TotalEnergy *= 1.3;
}

