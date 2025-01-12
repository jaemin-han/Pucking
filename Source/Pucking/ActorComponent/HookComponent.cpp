// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HookComponent.h"

#include "InputTriggers.h"
#include "Common/CommonStruct.h"
#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UHookComponent::UHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("HookComponent Cable"));
	CableComponent->CableLength = 500.f;
	CableComponent->CableWidth = 5.f;
	CableComponent->NumSegments = 1;       // 충분한 세그먼트 수 (너무 적으면 움직임이 딱딱할 수 있음)
	
	/*CableComponent->bEnableCollision = true; // 충돌 활성화
	CableComponent->SolverIterations = 16;  // 물리 시뮬레이션 정확도 향상
	CableComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화*/

	CableComponent->SetVisibility(false);
	CableComponent->bAttachEnd = false;
}


// Called when the game starts
void UHookComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			
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

void UHookComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	// 끝 위치 = 시작 위치에다가 (전방방향 * 범위)를 더함
	FVector EndLoc = StartLoc + ForwardVector * HookRange;
		
	FHitResult _hitRes;

	FCollisionQueryParams _collisionParam;
	_collisionParam.AddIgnoredActor(GetOwner());
		
	bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_Pawn, _collisionParam);
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Blue, true, 5.f);
	
	if(isHit)
	{
		if(AActor* hitActor = _hitRes.GetActor())
		{
			AttachCableToActor(hitActor, _hitRes.ImpactPoint);
		}
	}
}

void UHookComponent::Input_Hook()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Shoot Hook")));
}

TArray<FInputParameter> UHookComponent::ReturnInputParameter()
{
	if(HookInputMappingContext)
	{
		if(HookInputAction)
		{
			// Reload Input 함수
			FInputParameter HookInputParameter;
		
			HookInputParameter.TargetClass = this;
			HookInputParameter.TriggerEvent = ETriggerEvent::Started;
			HookInputParameter.InputMappingContext = HookInputMappingContext;
			HookInputParameter.InputAction = HookInputAction;
			HookInputParameter.CallbackFunc = FName("Input_Hook");

			InputParameters.Add(HookInputParameter);
		}
	}

	return InputParameters;
}

void UHookComponent::AttachCableToActor(AActor* Actor, FVector HitLocation)
{
	if(CableComponent)
	{
		CableComponent->SetVisibility(true);
		CableComponent->bAttachEnd = true;
		IsCanHookShoot = false;
		
		if(GetOwner())
		{
			FVector PlayerLocation = GetOwner()->GetActorLocation();
			FVector UnitDir = (HitLocation - PlayerLocation).GetSafeNormal();
			
			if(ACharacter* Player = Cast<ACharacter>(GetOwner()))
			{
				Player->LaunchCharacter(UnitDir * 2500.f, true, true);
			}
		}
		
		GetWorld()->GetTimerManager().SetTimer(HookTimer, [this, HitLocation]()
		{
			if(GetOwner())
			{
				FVector EndLoc = GetOwner()->GetActorTransform().InverseTransformPosition(HitLocation);
				CableComponent->EndLocation = EndLoc;
			}
			float Distance = FVector::Distance(GetOwner()->GetActorLocation(), HitLocation);
			if(Distance < 100.f)
			{
				CableComponent->SetVisibility(false);
				CableComponent->bAttachEnd = false;

				GetWorld()->GetTimerManager().ClearTimer(HookTimer);
			}
		}, 0.0016, true);
	}
}
