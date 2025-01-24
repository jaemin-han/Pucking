// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUI.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API ULevelUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPuckGameInstance* PuckGameInstance;

	UFUNCTION(BlueprintCallable)
	void VisibleHalfTimer(bool bHalf);

private:
	UFUNCTION(BlueprintCallable)
	void GameOverHideWidget();
};
