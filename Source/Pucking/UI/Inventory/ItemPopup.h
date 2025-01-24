// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemPopup.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UItemPopup : public UUserWidget
{
	GENERATED_BODY()

	// Text_ItemName
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ItemName;

	// Image_Item
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	// ItemBackground
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Background;

	// UGP_MainOptions
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UGP_MainOptions;

	// UGP_AddOptions
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UGP_AddOptions;

	// font
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPopup", meta = (AllowPrivateAccess = true))
	FSlateFontInfo Font;

	// font color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPopup", meta = (AllowPrivateAccess = true))
	FSlateColor FontColor;

	// magic font color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPopup", meta = (AllowPrivateAccess = true))
	FSlateColor MagicFontColor;

	// rare font color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPopup", meta = (AllowPrivateAccess = true))
	FSlateColor RareFontColor;

	// magic background color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPopup", meta = (AllowPrivateAccess = true))
	FLinearColor MagicBackgroundColor;

	// rare background color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPopup", meta = (AllowPrivateAccess = true))
	FLinearColor RareBackgroundColor;

public:
	// ItemSlot 을 입력으로 받아서 ItemPopup 을 업데이트
	UFUNCTION(BlueprintCallable)
	bool UpdateItemPopup(class UItemSlot* ItemSlot);
};
