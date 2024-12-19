// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// item image texture
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDragDropOperation", meta = (AllowPrivateAccess = true))
	class UTexture2D* ItemThumbnail;
	// item slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDragDropOperation", meta = (AllowPrivateAccess = true))
	class UItemSlot* ItemSlot;
};
