// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CommonEnum.h"
#include "GameFramework/Character.h"
#include "Interfaces/GetActorCompMap.h"
#include "PuckingPlayerCha.generated.h"
enum class EWeaponType : uint8;
class UInputAction;
UCLASS()
class PUCKING_API APuckingPlayerCha : public ACharacter, public IGetActorCompMap
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APuckingPlayerCha();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Camera Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USpringArmComponent* SpringArmCompo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UCameraComponent* CameraComp;
	
	// InputContextMapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputContextMapping")
	class UInputMappingContext* InputMappingContext;

	// ActorComponent
	// Input Binding 해주는 ActorComponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BindInput ActorComponent")
	class UEnhanceInputActorComponent* EnhanceInputActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Weapon Type")
	EWeaponType WeaponType = EWeaponType::Rifle;

public:
	// AnimNotify와의 연계
	UFUNCTION()
	virtual TArray<UActorComponent*> ReturnActorComponents(FName KeyName) override;
	
	TMap<FName, TArray<UActorComponent*>> ActorComponentInterfaceMap;

protected:
	// Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default InputAction")
	UInputAction* MoveIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default InputAction")
	UInputAction* LookUpIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default InputAction")
	UInputAction* TurnIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default InputAction")
	UInputAction* JumpIA;
	
	 // Player Default Movement
	UFUNCTION()
	void InputMove(const FInputActionValue& Value);
	
	UFUNCTION()
	void InputJump(const FInputActionValue& Value);

	UFUNCTION()
	void InputLook(const FInputActionValue& Value);

	UFUNCTION()
	void InputTurn(const FInputActionValue& Value);

private:
	// 캐릭터 이동 변수
	UPROPERTY()
	FVector MoveDirection;
	
};
