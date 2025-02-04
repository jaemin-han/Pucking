// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HookComponent.h"

#include "InputTriggers.h"
#include "Common/CommonStruct.h"
#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/MontageFSMInterface.h"
#include "UI/HUD/SubUI/SubCoolTimeUI.h"

// Sets default values for this component's properties
UHookComponent::UHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Cable
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("HookComponent Cable"));
	CableComponent->CableLength = 10.f;
	CableComponent->CableWidth = 50.f;
	CableComponent->NumSegments = 2;
	
	/*CableComponent->bEnableCollision = true; // 충돌 활성화
	CableComponent->SolverIterations = 16;  // 물리 시뮬레이션 정확도 향상
	CableComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화*/
	
	CableComponent->SetVisibility(false);
	CableComponent->bAttachEnd = false;

	// Timeline Event
	HookTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	TimelineEvent.BindUFunction(this, FName("StartHookTimer"));
	EndTimelineEvent.BindUFunction(this, FName("EndHookTimer"));
}


// Called when the game starts
void UHookComponent::BeginPlay()
{
	Super::BeginPlay();

	// 초기화
	InitActorComponent();
}


// Called every frame
void UHookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if(IsHookCool)
	{
		CheckCoolTime += DeltaTime;

		float CoolTimeRate = 0.f;
		if(CheckCoolTime > 0.f)
		{
			CoolTimeRate = CheckCoolTime / HookCoolTime;
		}
		SubCoolTimeUI->SetHookGauge(CoolTimeRate);
		if(CheckCoolTime >= HookCoolTime)
		{
			IsHookCool = false;
			CheckCoolTime = 0.f;
		}
	}
}


void UHookComponent::InitActorComponent()
{
	// Owner Actor를 먼저 확인
	if(GetOwner())
	{
		if(USkeletalMeshComponent* CharacterSkeletal = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
		{
			Equip(CharacterSkeletal, AttachedSocketName, FTransform(FRotator(60, 180, 0), FVector(0, 0, 0), FVector(0.2f)));	
		}

		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

		// 캐싱
		if(OwnerCharacter)
		{
			// 플레이어 카메라
			PlayerSpringArmComponent = OwnerCharacter->GetComponentByClass<USpringArmComponent>();
			OriginSpringArmLength = PlayerSpringArmComponent->TargetArmLength;

			// 플레이어 AnimInstance
			if(OwnerCharacter->GetMesh()->GetAnimInstance())
			{
				OwnerFsmInterface = Cast<IMontageFSMInterface>(OwnerCharacter->GetMesh()->GetAnimInstance());
			}
			// Character Movement
			OwnerMovement = OwnerCharacter->GetCharacterMovement();

			// Character Movement 초기 설정값
			OriginGravity =	OwnerMovement->GravityScale;
			OriginAirControl = OwnerMovement->AirControl;
			OriginGroundFriction = OwnerMovement->GroundFriction;
		}
	}

	// Timeline 이벤트 - CableComponent의 EndLocation 갱신
	if(CurveFloat)
	{
		HookTimelineComponent->AddInterpFloat(CurveFloat, TimelineEvent);
		HookTimelineComponent->SetTimelineFinishedFunc(EndTimelineEvent);
		
		HookTimelineComponent->SetLooping(false);
		HookTimelineComponent->SetTimelineLength(0.8f);	
	}

	// 예측지점에 생성할 Actor
	if(HookPreviewClass)
	{
		HookPreviewActor = GetWorld()->SpawnActor<AActor>(HookPreviewClass);
		HookPreviewActor->SetActorHiddenInGame(true);
	}
}


void UHookComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	if(HookSkeletalMesh)
	{
		HookSkeletalMeshComponent = NewObject<USkeletalMeshComponent>(TargetSkeletalMeshComp->GetOwner(), FName("HookSkeletalMesh"));
		if(HookSkeletalMeshComponent)
		{
			HookSkeletalMeshComponent->SetRelativeTransform(ActorTransform);
			HookSkeletalMeshComponent->SetSkeletalMesh(HookSkeletalMesh);
			HookSkeletalMeshComponent->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			HookSkeletalMeshComponent->RegisterComponent();
			
			if(CableComponent)
			{
				CableComponent->AttachToComponent(HookSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("CableComponent"));
				CableComponent->RegisterComponent();
			}

			if(GetOwner())
			{
				GetOwner()->AddInstanceComponent(HookSkeletalMeshComponent);
				GetOwner()->AddInstanceComponent(CableComponent);
			}
		}
	}
}

void UHookComponent::ShootHook(FVector StartLoc, FVector ForwardVector)
{
	// 끝 위치 = 시작 위치에다가 (전방방향 * 범위)를 더함
	FVector EndLoc = StartLoc + ForwardVector * HookRange;

	DestinationVector = EndLoc;

	// Cable 가시화
	CableComponent->SetVisibility(true);
	CableComponent->bAttachEnd = true;

	// Cable 날림 
	HookTimelineComponent->PlayFromStart();
	
}

void UHookComponent::Input_HookMode()
{
	if(IsHookCool) return;
	if(!CheckCanHook(ECharacterMontage::HookMode)) return;
	
	if(OwnerFsmInterface && HookModeMontage)
	{
		//OwnerAnimIns->Montage_Play(HookModeMontage);
		OwnerFsmInterface->ReceiveMontageState(ECharacterMontage::HookMode);
		if(PlayerSpringArmComponent)
		{
			PlayerSpringArmComponent->TargetArmLength = HookModeSpringArmLength;
			
			if(!HookPreviewActor) return;
			
			FHitResult TraceHitRes;
			FCollisionQueryParams _CollisionParam;
			_CollisionParam.AddIgnoredActor(GetOwner());
			_CollisionParam.AddIgnoredComponent(HookSkeletalMeshComponent);
			
			FVector StartLoc = HookSkeletalMeshComponent->GetComponentLocation();
			FVector Dir = PlayerSpringArmComponent->GetChildComponent(0)->GetForwardVector();
			FVector EndLoc = StartLoc + (Dir * HookRange);
			
			bIsHitActor = GetWorld()->LineTraceSingleByChannel(TraceHitRes, StartLoc, EndLoc, ECC_GameTraceChannel4, _CollisionParam);
			
			if(bIsHitActor)
			{
				float Distance = FVector::Dist(TraceHitRes.ImpactPoint, StartLoc);
				if(Distance > MinCanHook)
				{
					HookPreviewActor->SetActorLocation(TraceHitRes.ImpactPoint);
					HookPreviewActor->SetActorHiddenInGame(false);
					return;
				}
			}
			
			HookPreviewActor->SetActorHiddenInGame(true);
		}
	}
}

void UHookComponent::Input_HookShoot()
{
	if(IsHookCool) return;
	if(!CheckCanHook(ECharacterMontage::Hooking)) return;
	
	/*if(OwnerAnimIns && HookShootMontage)
	{
		OwnerAnimIns->Montage_Play(HookShootMontage);
	}*/
	if(OwnerFsmInterface)
	{
		OwnerFsmInterface->ReceiveMontageState(ECharacterMontage::Hooking);
	}
}


TArray<FInputParameter> UHookComponent::ReturnInputParameter()
{
	if(HookInputMappingContext)
	{
		if(HookInputAction)
		{
			// Hook Input 함수
			FInputParameter HookModeParameter;
		
			HookModeParameter.TargetClass = this;
			HookModeParameter.TriggerEvent = ETriggerEvent::Triggered;
			HookModeParameter.InputMappingContext = HookInputMappingContext;
			HookModeParameter.InputAction = HookInputAction;
			HookModeParameter.CallbackFunc = FName("Input_HookMode");

			InputParameters.Add(HookModeParameter);

			FInputParameter HookShootParameter;
		
			HookShootParameter.TargetClass = this;
			HookShootParameter.TriggerEvent = ETriggerEvent::Completed;
			HookShootParameter.InputMappingContext = HookInputMappingContext;
			HookShootParameter.InputAction = HookInputAction;
			HookShootParameter.CallbackFunc = FName("Input_HookShoot");

			InputParameters.Add(HookShootParameter);
		}
	}

	return InputParameters;
}


void UHookComponent::InitCableComponent()
{
	bIsHitActor = false;
	
	CableComponent->bAttachEnd = false;
	CableComponent->CableLength = 10;
	
	OwnerMovement->GravityScale = OriginGravity;
	OwnerMovement->AirControl = OriginAirControl;
	OwnerMovement->GroundFriction = OriginGroundFriction;

	/*FTimerHandle ClearHookTimer;
	GetWorld()->GetTimerManager().SetTimer(ClearHookTimer, [this]()
	{
		CableComponent->SetVisibility(false);
	}, 0.5f, false);*/
	CableComponent->SetVisibility(false);
}

// 
void UHookComponent::LaunchToCable(const FVector& HitLocation)
{
	if(CableComponent)
	{
		IsCanHookShoot = false;
		
		if(GetOwner())
		{
			FVector PlayerLocation = GetOwner()->GetActorLocation();
			FVector SubtractLoc = HitLocation - PlayerLocation;

			/*float UnitDir = (HitLocation - PlayerLocation).Normalize();
			FVector LaunchPower = SubtractLoc * LaunchRate;*/

			FVector UnitDir = SubtractLoc.GetSafeNormal();
			FVector LaunchPower = UnitDir * LaunchRate;
			
			if(ACharacter* Player = Cast<ACharacter>(GetOwner()))
			{
				if(PlayerLocation.Z > HitLocation.Z)
				{
					UE_LOG(LogTemp, Warning, TEXT("아래"));
				}
				
				OwnerMovement->GravityScale = 0.f;
				OwnerMovement->AirControl = 0.2f;
				OwnerMovement->GroundFriction = 0.f;
				
				Player->LaunchCharacter(LaunchPower, true, true);

				/*if(StartMontage)
				{
					OwnerAnimIns->Montage_Play(StartMontage);
				}*/
				if(OwnerFsmInterface)
				{
					OwnerFsmInterface->ReceiveMontageState(ECharacterMontage::HookStart);
				}
				
				/*EndDelegate.BindUObject(this, &UHookComponent::OnHookMontageEnd);
				OwnerAnimIns->Montage_SetEndDelegate(EndDelegate, StartMontage);*/
				
				// Launch 후 일정 시간 뒤에 자동으로 초기화
				FTimerHandle ClearHookTimer;
				GetWorld()->GetTimerManager().SetTimer(ClearHookTimer, [this]()
				{
					InitCableComponent();
				}, 0.5f, false);
			}
		} 
	}
}

bool UHookComponent::CheckCanHook(ECharacterMontage HookFsmMontage)
{
	bool IsCanMontage = false;
	if(OwnerFsmInterface)
	{
		IsCanMontage = OwnerFsmInterface->CheckChangeStateByMontage(HookFsmMontage);
	}
	return IsCanMontage;
}

void UHookComponent::StartHookTimer(float Value)
{
	// 데칼 안 보이게
	HookPreviewActor->SetActorHiddenInGame(true);
	
	// 카메라 원래대로
	float LerpArmLength = FMath::Lerp(HookModeSpringArmLength, OriginSpringArmLength, Value);
	PlayerSpringArmComponent->TargetArmLength = LerpArmLength;
	
	// Component 및 GetOwner 기준의 로컬 좌표 계산
	FVector OriginLoc = HookSkeletalMeshComponent->GetComponentTransform().InverseTransformPosition(GetOwner()->GetActorLocation());
	FVector EndLoc = GetOwner()->GetActorTransform().InverseTransformPosition(DestinationVector);

	// Lerp로 위치 계산
	FVector MoveToLocation = FMath::Lerp(OriginLoc, EndLoc, Value);

	// CableComponent의 로컬 좌표로 EndLocation 갱신
	CableComponent->EndLocation = MoveToLocation;

	// Sphere Collision으로 HitActor 체크
	FHitResult _HitRes;
	FCollisionQueryParams _CollisionParam;
	_CollisionParam.AddIgnoredActor(GetOwner());
	_CollisionParam.AddIgnoredComponent(HookSkeletalMeshComponent);

	// 이동하는 월드 좌표
	
	FVector OwnerLoc = HookSkeletalMeshComponent->GetComponentLocation();
	FVector MoveToWorldLocation = FMath::Lerp(OwnerLoc, DestinationVector, Value);
	
	bIsHitActor = GetWorld()->SweepSingleByChannel(_HitRes, OwnerLoc, MoveToWorldLocation,FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(HookTraceRadius), _CollisionParam);
	//DrawDebugSphere(GetWorld(), MoveToWorldLocation, HookTraceRadius, 10, FColor::Blue, false, 3.f);
	
	if(bIsHitActor)
	{
		bIsHitActor = true;
		DestinationVector = _HitRes.ImpactPoint;

		float Distance = FVector::Dist(DestinationVector, GetOwner()->GetActorLocation());
		if(Distance < MinCanHook-(150.f))
		{
			bIsHitActor = false;
		}
		/*HookTimelineComponent->Stop();
		EndHookTimer();*/
	}
}

void UHookComponent::EndHookTimer()
{
	if(bIsHitActor)
	{
		// 맞추면 이동 + 쿨타임 시작
		IsHookCool = true;
		
		/*FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [this]()
		{
			LaunchToCable(DestinationVector);		
		}, 0.2f, false);*/
		LaunchToCable(DestinationVector);
	}
	else
	{
		//TODO: Hook으로 이동 실패 시
		InitCableComponent();
	}
}

void UHookComponent::OnHookMontageStartCallback(UAnimMontage* Montage, bool bInterrupted)
{
}

void UHookComponent::OnHookMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
}
