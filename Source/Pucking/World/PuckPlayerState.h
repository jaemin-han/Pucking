// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PuckPlayerState.generated.h"

// Essence 값이 변할 때 호출될 delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEssenceChanged, int32, NewEssence);

/**
 * 
 */
UCLASS()
class PUCKING_API APuckPlayerState : public APlayerState
{
	GENERATED_BODY()

	// Skill Widget Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USkillWidget> SkillWidgetClass;
	// Skill Widget Instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	USkillWidget* SkillWidgetInstance;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essence")
	int32 Essence;

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

public:
	APuckPlayerState();
	int32 GetEssence() const { return Essence; }

	void SetEssence(const int32 NewEssence)
	{
		Essence = NewEssence;
		OnEssenceChanged.Broadcast(Essence);
	}

	void AddEssence(const int32 AddEssence)
	{
		Essence += AddEssence;
		OnEssenceChanged.Broadcast(Essence);
	}

	// int32 를 입력으로 받는다. 이는 소모할 Essence 를 의미한다.
	// Essence 가 충분하다면 true 를 반환하고, Essence 를 소모한다.
	// Essence 가 부족하다면 false 를 반환한다.
	bool ConsumeEssence(const int32 ConsumeEssence);

	FOnEssenceChanged OnEssenceChanged;
};
