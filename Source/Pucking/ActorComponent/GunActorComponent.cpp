// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/GunActorComponent.h"

#include "Runtime/Core/Tests/Containers/TestUtils.h"

// Sets default values for this component's properties
UGunActorComponent::UGunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bIsShootAble = true;

	// EquipActorComponent의 Delegate 구독
	//TODO 생성자에서 구독했을 때 정상적으로 동작하는지 확인 필수
	// GetOwner->EquipActorComponent->OnChangeAmmoEvent.AddDynamic(this, &GunActorComponent::BindChangeAmmoEvent);
}


// Called when the game starts
void UGunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunActorComponent::SetDefaultGunInfoStruct(FName TableRows)
{
	// DataTable 정보를 세팅
	if (GunInfoDataTable)
	{
		FGunInfoStruct* DT_GunData = GunInfoDataTable->FindRow<FGunInfoStruct>(FName(TableRows), TEXT(""));
		
		GunInfoStruct.GunType = DT_GunData->GunType;
		GunInfoStruct.Magazine = DT_GunData->Magazine;
		GunInfoStruct.MaxMagazine = DT_GunData->MaxMagazine;
		GunInfoStruct.DefaultDamage = DT_GunData->DefaultDamage;
		GunInfoStruct.SpreadY = DT_GunData->SpreadY;
		GunInfoStruct.SpreadZ = DT_GunData->SpreadZ;
		GunInfoStruct.RecoilPitch = DT_GunData->RecoilPitch;
		GunInfoStruct.RecoilYaw = DT_GunData->RecoilYaw;
		GunInfoStruct.Range = DT_GunData->Range;
		GunInfoStruct.ShootInterval = DT_GunData->ShootInterval;
		
		//UE_LOG(LogTemp, Warning, TEXT("%s Struct Data is Set"), *TableRows.ToString());
	}
}

void UGunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName,
                               FTransform ActorTransform)
{
}

void UGunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
}

void UGunActorComponent::Reload()
{
	// TODO EquipActorComponent에서 현재 총알 개수를 받아옴
	// int32 CurrentMagazine = GetOwner()->EquipmentActorComponent->GetFunc(GunInfoStruct.MaxMagazine);
	// GunInfoStruct.Magazine = CurrentMagazine; 
	GunInfoStruct.Magazine = GunInfoStruct.MaxMagazine;
}

void UGunActorComponent::SetShootInterval(float IntervalTime)
{
	GunInfoStruct.ShootInterval = IntervalTime;
}

bool UGunActorComponent::GetIsShootAble() const
{
	return bIsShootAble;
}

void UGunActorComponent::SetIsShootAble(bool ShootAble)
{
	bIsShootAble = ShootAble;
}

void UGunActorComponent::SetSpreadRange(float Y, float Z)
{
	GunInfoStruct.SpreadY = Y;
	GunInfoStruct.SpreadZ = Z;
}

void UGunActorComponent::CameraShakeRecoil()
{
}

void UGunActorComponent::BindChangeAmmoEvent()
{
	Reload();
}
