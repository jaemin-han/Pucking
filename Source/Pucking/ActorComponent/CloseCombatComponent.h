// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CloseCombatComponent.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_ThreeParams(FCombatMeshAttachment, UStaticMeshComponent*, USceneComponent*, USceneComponent*);

UCLASS()
class PUCKING_API UCloseCombatComponent : public UBoxComponent
{
	GENERATED_BODY()
public:	
	UCloseCombatComponent();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FCombatMeshAttachment CombatMeshAttachment;

	UFUNCTION(BlueprintCallable)
	void DisableBoxCollision();
	UFUNCTION(BlueprintCallable)
	void EnableBoxCollision();
	
	void ClearIgnoreActors();
protected:
	virtual void BeginPlay() override;
	bool ActorHasSameTag(AActor* OtherActor);
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere)
	AActor* OwnerActor;
	
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	UStaticMeshComponent* CloseCombatMeshComp;
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	USceneComponent* BoxTraceStart;
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	USceneComponent* BoxTraceEnd;

	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	FVector BoxTraceExtent = FVector(1.f,30.f,10.f);

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	bool bShowBoxDebug = false;
};