// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CloseCombatComponent.h"

#include "StatusComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Enemy/Weapon.h"
#include "Interfaces/StatusInterface.h"

class AEnemyBase;

UCloseCombatComponent::UCloseCombatComponent()
{
	CloseCombatMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CloseCombatMeshComp"));
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
}

void UCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	/////내가 소유한 Component를 Owner Actor를 찾아가서 나한테 Attach 시켜주기
	//소유주가 Weapon일 경우의 Delegate Binding & Execute
	if(AWeapon* OwnerWeapon = Cast<AWeapon>(OwnerActor))
	{
		CombatMeshAttachment.BindUObject(OwnerWeapon, &AWeapon::OnCombatMeshAttachment);
		CombatMeshAttachment.Execute(CloseCombatMeshComp, BoxTraceStart, BoxTraceEnd);
	}
	//소유주가 Weapon일 경우의 Delegate Binding & Execute
	if(AEnemyBase* OwnerEnemy = Cast<AEnemyBase>(OwnerActor))
	{
		CombatMeshAttachment.BindUObject(OwnerEnemy, &AEnemyBase::OnCombatCompAttachment);
		CombatMeshAttachment.Execute(CloseCombatMeshComp, BoxTraceStart, BoxTraceEnd);
	}
	OnComponentBeginOverlap.AddDynamic(this, &UCloseCombatComponent::OnBoxOverlap);
}

bool UCloseCombatComponent::ActorHasSameTag(AActor* OtherActor)
{
	return OwnerActor->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void UCloseCombatComponent::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ActorHasSameTag(OtherActor)) return;
	
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	FHitResult _HitRes;
	ETraceTypeQuery TraceQuery = UEngineTypes::ConvertToTraceType(ECC_Pawn);
	bool bIsHit = UKismetSystemLibrary::BoxTraceSingle(this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		TraceQuery,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		_HitRes,
		true
		);
	
	if(bIsHit)
	{
		if(AActor* HitActor = _HitRes.GetActor())
		{
			IStatusInterface* StatInterface = Cast<IStatusInterface>(HitActor->FindComponentByClass<UStatusComponent>());
			if(StatInterface)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Hit");
				//때린 녀석 넘겨 주기(Combat Component의 주인 Pawn)
				StatInterface->DamageProcessing(OwnerActor, _HitRes);
			}
			//IgnoreActors.AddUnique(_HitRes.GetActor());
			//Anim Notify로 Box Collision Enable/Disable 추가하기
		}
	}
}