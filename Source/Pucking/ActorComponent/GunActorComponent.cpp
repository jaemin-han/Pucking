// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/GunActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
		OwnerCharacter = Cast<ACharacter>(GetOwner());
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
	}
}

void UGunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	if(GunSkeletalMesh)
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(TargetSkeletalMeshComp->GetOwner());
		if(SkeletalMeshComponent)
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
	// 발사 직후 사격 불가능 상태
	this->SetIsShootAble(false);

	// TODO 이후 AnimNotify에서 설정해줘야함(캔슬 됐을 때 포함) 
	FTimerHandle ShootAbleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ShootAbleTimerHandle, [this]()
	{
		this->SetIsShootAble(true);
	}, GunInfoStruct.ShootInterval, false);
	
	if(MuzzleParticle)
	{
		FVector MuzzleLoc = SkeletalMeshComponent->GetSocketLocation(FName("Muzzle"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, MuzzleLoc, FRotator(0, 0, 0));	
	}
}

void UGunActorComponent::Reload()
{
	//TODO Merge 후 주석 해제
	//int32 RemainAmmo = OnRemainAmmo.Execute(GunInfoStruct.MaxMagazine);
	int32 RemainAmmo = GunInfoStruct.MaxMagazine;
	GunInfoStruct.Magazine = RemainAmmo;
	
	SetIsShootAble(true);
}

void UGunActorComponent::SetShootInterval(float IntervalTime)
{
	GunInfoStruct.ShootInterval = IntervalTime;
}

bool UGunActorComponent::GetIsShootAble()
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
}

TArray<FInputParameter> UGunActorComponent::ReturnInputParameter()
{
	return InputParameters;
}

void UGunActorComponent::Input_Fire(const FInputActionValue& Value)
{
}

void UGunActorComponent::Input_Reload()
{
	SetIsShootAble(false);
}

void UGunActorComponent::PlayOwnerMontage(class UAnimMontage* OwnerMontage)
{
	if(!OwnerCharacter || !OwnerMontage) return;
	
	if(UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		if(!OwnerAnimIns->Montage_IsPlaying(OwnerMontage))
		{
			OwnerAnimIns->Montage_Play(OwnerMontage);	
		}
	}
	
}
