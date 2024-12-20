// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ShieldTaskComponent.h"
#include "ActorComponent/StatusComponent.h"
#include "ActorComponent/HP_ManagementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values for this component's properties
UShieldTaskComponent::UShieldTaskComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldNiagara"));
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSysAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_Shield_2.NS_Shield_2'"));
	
	if (NiagaraSysAsset.Succeeded())
	{
		NiagaraSys = NiagaraSysAsset.Object;
		NiagaraComp->SetAsset(NiagaraSys);
		NiagaraComp->bAutoActivate = false;
	}
	
	// ...
}


// Called when the game starts
void UShieldTaskComponent::BeginPlay()
{
	Super::BeginPlay();
	Status = GetOwner()->FindComponentByClass<UStatusComponent>();
	HP_Management = GetOwner()->FindComponentByClass<UHP_ManagementComponent>();
	if (Status)
	{
		CurrentShield = Status->MaxShield;
	}
	NiagaraComp->SetActive(true, false);
	// ...
	
}


// Called every frame
void UShieldTaskComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), FString::Printf(TEXT("SHIELD : %.1f"), CurrentShield), 0, FColor::White, 0.005f, false, 2.0f);
	NiagaraComp->SetWorldLocation(GetOwner()->GetActorLocation());
	NiagaraComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	
	// ...
}

//실드가 데미지 받음
void UShieldTaskComponent::ShieldTakeDamage(float damage, EDamageType damageType)
{

	//실드가 없는 상태면
	if (CurrentShield <= 0)
	{
		NiagaraComp->Deactivate();
		NiagaraComp->SetVisibility(false);
		
		CurrentShield = 0;
		//체력 처리로 이동
		HP_Management->HPTakeDamage(damage, damageType);
	}
	else if (CurrentShield > 0)
	{
		CurrentShield -= damage;

	}
	GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);

	//데미지 받고 3초 후 실드 회복 시작
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoveryDelayTimer, this, &UShieldTaskComponent::ShieldRecovery, 3.0f, false);
}


//실드 회복
void UShieldTaskComponent::ShieldRecovery()
{
	if (CurrentShield <= 0)
	{
		
		NiagaraComp->Deactivate();
		NiagaraComp->SetVisibility(false);
		
		CurrentShield = 0;
	}
	else if (CurrentShield > 0)
	{
		NiagaraComp->SetVisibility(true);
		NiagaraComp->SetActive(false, true);
	}


	CurrentShield++;
	if (CurrentShield < Status->MaxShield)
	{
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoverySpeedTimer, this, &UShieldTaskComponent::ShieldRecovery, 0.001f, false);
	}
	else if (CurrentShield >= Status->MaxShield)
	{
		GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);
		return;
	}
}

