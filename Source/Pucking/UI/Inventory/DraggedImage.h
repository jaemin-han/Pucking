// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggedImage.generated.h"

/**
 * 
 */
// todo: 얘 삭제해도 될듯?
UCLASS()
class PUCKING_API UDraggedImage : public UUserWidget
{
	GENERATED_BODY()

public:
	// Image_Dragged
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Dragged;
};
