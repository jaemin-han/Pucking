// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstanceData.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"


UCLASS()
class PUCKING_API AItemBase : public AActor
{
	GENERATED_BODY()

protected:

	// sphere collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Collision", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	// Item Static Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemStaticMesh;

	// Item Skeletal Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemSkeletalMesh;

public:
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 아이템이 습득된 후 처리, 호환성을 위해 순수 가상 함수로 구현하지 않았음
	virtual void OnPickup();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void ConstructMesh() const;

public:
	// Item Type
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;
	// Item Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemInstanceData ItemData;
};
