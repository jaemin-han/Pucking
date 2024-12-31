// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/GunActorComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunActorComponent::UGunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bIsShootAble = true;
}


// Called when the game starts
void UGunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(GetOwner())
	{
		UCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
		if (CameraComponent)
		{
			OwnerCameraComp = CameraComponent;
		}
	}
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

void UGunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	if(GunSkeletalMesh)
	{
		if(USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(TargetSkeletalMeshComp->GetOwner()))
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			
			SkeletalMeshComponent->SetRelativeRotation(FRotator(90, 0, 180));
			SkeletalMeshComponent->SetSkeletalMesh(GunSkeletalMesh);
			SkeletalMeshComponent->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			SkeletalMeshComponent->RegisterComponent();
		}
	}
}

void UGunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
}

void UGunActorComponent::Reload()
{
	int32 RemainAmmo = OnRemainAmmo.Execute(GunInfoStruct.MaxMagazine);
	GunInfoStruct.Magazine = RemainAmmo;
	UE_LOG(LogTemp, Warning, TEXT("Parameter is %d, Return Value is %d"), GunInfoStruct.MaxMagazine, RemainAmmo);
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

void UGunActorComponent::Input_Fire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("GunActorComponent"));
}

FInputParameter& UGunActorComponent::ReturnInputParameter()
{
	return InputParameter;
}
