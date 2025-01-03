// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CommonStruct.h"
#include "Components/ActorComponent.h"
#include "Interfaces/BindInputInterface.h"
#include "AnimComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UAnimComponent : public UActorComponent, public IBindInputInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	ACharacter* Owner;

	// Walk and Jog blend space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UBlendSpace* WalkAndJogBlendSpace;

	// IronSight blend space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UBlendSpace* IronSightBlendSpace;

public:
	// Sets default values for this component's properties
	UAnimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float JogSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsJogging;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsIronSight;

public:
#pragma region IBindInputInterface
	UPROPERTY()
	TArray<FInputParameter> InputParameters;

	// AnimMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* AnimInputMappingContext;

	// JogAction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JogAction;

	// IronSightAction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UInputAction* IronSightAction;

	virtual TArray<FInputParameter> ReturnInputParameter() override;

#pragma endregion

private:
	void SetBlendSpaceSpeeds(float NewWalkSpeed, float NewJogSpeed);
	// Jog
	UFUNCTION()
	void HandleStartJog();
	UFUNCTION()
	void HandleEndJog();
	// IronSight
	UFUNCTION()
	void HandleStartIronSight();
	UFUNCTION()
	void HandleEndIronSight();
};


