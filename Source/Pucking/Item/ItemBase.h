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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void ConstructMesh() const;

public:
	// Item Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemInstanceData ItemData;
};
