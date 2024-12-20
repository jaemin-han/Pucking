// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/EquipInterface.h"
#include "Interfaces/FireInterface.h"
#include "Interfaces/ReloadInterface.h"
#include "GunActorComponent.generated.h"

class UStaticMeshComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UGunActorComponent : public UActorComponent, public IFireInterface, public IReloadInterface, public IEquipInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun DataTable")
	UDataTable* GunInfoDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun StaticMesh")
	UStaticMesh* GunStaticMesh;

	UPROPERTY()
	UStaticMeshComponent* GunStaticMeshComponent;

public:
	// 장착할 Actor와 SocketName, Transform 정보
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;
	
	// Gun StaticMesh에 격발
	virtual void Fire(class UArrowComponent* GunArrowComponent) override;

	// Gun Struct의 탄환 값을 늘려준다
	virtual void Reload(class FShotgunInfo& GunInfo) override;
};
