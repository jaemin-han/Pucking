// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HookComponent.h"

#include "InputTriggers.h"
#include "Common/CommonStruct.h"
#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UHookComponent::UHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("HookComponent Cable"));
	CableComponent->CableLength = 100.f;
	CableComponent->CableWidth = 10.f;
	CableComponent->NumSegments = 2;       // 세그먼트 수 (너무 적으면 움직임이 딱딱할 수 있음)
	
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

	// ...
	// Owner Actor를 먼저 확인
	if(GetOwner())
	{
		if(USkeletalMeshComponent* CharacterSkeletal = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
		{
			Equip(CharacterSkeletal, FName("hand_l"), FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(0.2f)));	
		}

		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		
		if(OwnerCharacter)
		{
			PlayerSpringArmComponent = OwnerCharacter->GetComponentByClass<USpringArmComponent>();
			OriginSpringArmLength = PlayerSpringArmComponent->TargetArmLength;
			
			OwnerAnimIns = OwnerCharacter->GetMesh()->GetAnimInstance();
		}
	}

	if(CurveFloat)
	{
		HookTimelineComponent->AddInterpFloat(CurveFloat, TimelineEvent);
		HookTimelineComponent->SetTimelineFinishedFunc(EndTimelineEvent);
		
		HookTimelineComponent->SetLooping(false);
		HookTimelineComponent->SetTimelineLength(0.8f);	
	}

	/*CableComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CableComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	CableComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);*/
}


// Called every frame
void UHookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHookComponent::InitActorComponent()
{
}

void UHookComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	if(HookSkeletalMesh)
	{
		HookSkeletalMeshComponent = NewObject<USkeletalMeshComponent>(TargetSkeletalMeshComp->GetOwner());
		if(HookSkeletalMeshComponent)
		{
			HookSkeletalMeshComponent->SetRelativeTransform(ActorTransform);
			HookSkeletalMeshComponent->SetSkeletalMesh(HookSkeletalMesh);
			HookSkeletalMeshComponent->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			HookSkeletalMeshComponent->RegisterComponent();
			
			if(CableComponent)
			{
				CableComponent->AttachToComponent(HookSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	}
}

void UHookComponent::ShootHook(FVector StartLoc, FVector ForwardVector)
{
	// 끝 위치 = 시작 위치에다가 (전방방향 * 범위)를 더함
	FVector EndLoc = StartLoc + ForwardVector * HookRange;
	
	/*FHitResult _hitRes;

	FCollisionQueryParams _collisionParam;
	_collisionParam.AddIgnoredActor(GetOwner());
	
	bool IsHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_Pawn, _collisionParam);
	
	if(IsHit)
	{
		if(Cast<AActor>(_hitRes.GetActor()))
		{
			HitActor = _hitRes.GetActor();
		}
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Blue, true, 5.f);

		// HitActor가 있으면 Cable 끝 = HitLocation
		DestinationVector = _hitRes.ImpactPoint;
	}
	else
	{
		// HitActor가 없다면 Cable 끝 = 사정거리 끝
		DestinationVector = EndLoc;
	}*/

	DestinationVector = EndLoc;

	// Cable 가시화
	CableComponent->SetVisibility(true);
	CableComponent->bAttachEnd = true;

	// Cable 날림 
	HookTimelineComponent->PlayFromStart();
	
}

void UHookComponent::Input_HookMode()
{
	if(OwnerAnimIns && HookModeMontage)
	{
		OwnerAnimIns->Montage_Play(HookModeMontage);
		PlayerSpringArmComponent->TargetArmLength = HookModeSpringArmLength;
	}
}

void UHookComponent::Input_HookShoot()
{
	if(OwnerAnimIns && HookShootMontage)
	{
		OwnerAnimIns->Montage_Play(HookShootMontage);
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

	FTimerHandle ClearHookTimer;
	GetWorld()->GetTimerManager().SetTimer(ClearHookTimer, [this]()
	{
		CableComponent->SetVisibility(false);
	}, 1.f, false);
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

			float UnitDir = (HitLocation - PlayerLocation).Normalize();
			FVector LaunchPower = SubtractLoc * LaunchRate;
			
			if(ACharacter* Player = Cast<ACharacter>(GetOwner()))
			{
				Player->LaunchCharacter(UnitDir * LaunchPower, true, true);

				// Launch 후 1초 뒤에 자동으로 초기화
				FTimerHandle ClearHookTimer;
				GetWorld()->GetTimerManager().SetTimer(ClearHookTimer, [this]()
				{
					InitCableComponent();
				}, 0.5f, false);
			}
		} 
	}
}

void UHookComponent::StartHookTimer(float Value)
{
	// 카메라 원래대로
	float LerpArmLength = FMath::Lerp(HookModeSpringArmLength, OriginSpringArmLength, Value);
	PlayerSpringArmComponent->TargetArmLength = LerpArmLength;
	
	// GetOwner 기준의 로컬 좌표 계산
	FVector OriginLoc = GetOwner()->GetActorTransform().InverseTransformPosition(GetOwner()->GetActorLocation());
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

	// 검지하는 구체 반지름
	float SphereRadius = 20.f;

	// 이동하는 월드 좌표
	FVector OwnerLoc = GetOwner()->GetActorLocation();
	FVector MoveToWorldLocation = FMath::Lerp(OwnerLoc, DestinationVector, Value);
	
	bIsHitActor = GetWorld()->SweepSingleByChannel(_HitRes, OwnerLoc, MoveToWorldLocation,FQuat::Identity,
		ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), _CollisionParam);
	DrawDebugSphere(GetWorld(), MoveToWorldLocation, SphereRadius, 10, FColor::Blue, false, 3.f);
	if(bIsHitActor)
	{
		bIsHitActor = true;
		DestinationVector = _HitRes.ImpactPoint;
		
		HookTimelineComponent->Stop();
		EndHookTimer();
	}
}

void UHookComponent::EndHookTimer()
{
	if(bIsHitActor)
	{
		// 성공하면 캐릭터 이동
		// Actor 기준
		LaunchToCable(DestinationVector);
	}
	else
	{
		//TODO: Hook으로 이동 실패 시
		InitCableComponent();
	}
}
