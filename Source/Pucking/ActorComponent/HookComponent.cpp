// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HookComponent.h"

#include "InputTriggers.h"
#include "Common/CommonStruct.h"
#include "CableComponent.h"

// Sets default values for this component's properties
UHookComponent::UHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("HookComponent Cable"));
	CableComponent->CableLength = 300.f;
	CableComponent->NumSegments = 10;
	CableComponent->CableWidth = 5.f;
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

			CableComponent->SetupAttachment(HookSkeletalMeshComponent);
			CableComponent->SetRelativeLocation(FVector(0, 0, 0));
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

void UHookComponent::AttachCableToActor(const AActor* Actor, FVector HitLocation)
{
	if(CableComponent)
	{
		//CableComponent->SetAttachEndTo(Actor, NAME_None);
		CableComponent->EndLocation = HitLocation;
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("%s"), *HitLocation.ToString()));
	}
}

