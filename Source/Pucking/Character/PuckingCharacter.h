// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EssenceInterface.h"
#include "Interfaces/HealthMarbleInterface.h"
#include "Logging/LogMacros.h"
#include "PuckingCharacter.generated.h"

enum class EWeaponType : uint8;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APuckingCharacter : public ACharacter, public IEssenceInterface, public IHealthMarbleInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** SkillWidget OnOff Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillWidgetOnOffAction;

	/** Inventory OnOff Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryOnOffAction;
	
	// Input Binding 해주는 ActorComponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BindInput ActorComponent", meta = (AllowPrivateAccess = "true"))
	class UEnhanceInputActorComponent* EnhanceInputActorComponent;

	// EnhancedInputLocalPlayerSubsystem
	UPROPERTY()
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;

	// UEnhancedInputComponent
	UPROPERTY()
	class UEnhancedInputComponent* EnhancedInputComponent;

public:
	APuckingCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	void GetHit(const FHitResult& Hit);

	// 캐릭터 이동 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movable")
	bool bIsMovable = true;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerController* PC;

#pragma region IEssenceInterface, IHealthMarbleInterface
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Essence", meta = (AllowPrivateAccess = "true"))
	class APuckPlayerState* PuckPlayerState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Essence", meta = (AllowPrivateAccess = "true"))
	class UPlayerStatusComponent* PlayerStatusComponent;
public:
	virtual void AddEssence(const int32 AddEssence) override;
	virtual void ApplyHeal(float HealAmount) override;
#pragma endregion

#pragma region CloseCombatComponent, Hammer
private:
	// UPROPERTY(EditAnywhere, Category = "CloseCombat")
	// class UCloseCombatComponent* CloseCombatComponent;
	UPROPERTY(EditAnywhere, Category = "CloseCombat")
	UStaticMesh* HammerMesh;
public:
	UFUNCTION()
	void OnCombatCompAttachment(UStaticMeshComponent* TargetMeshComp, USceneComponent* BoxTraceStart, USceneComponent* BoxTraceEnd);
#pragma endregion

#pragma region IIsCurWeaponTypeInterface
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bind Componets")
	TArray<UActorComponent*> BindComponents;
private:
	UFUNCTION()
	void ChangeWeaponInputMapping(EWeaponType ChangedWeaponType);
#pragma endregion

#pragma region UserWidget
public:
	// 남은 총알 개수
	UFUNCTION()
	void GetRemainMagazine(EWeaponType TargetWeapon);

	// MainHUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UserWidget")
	TSubclassOf<class UMainHUD> MainHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserWidget")
	class UMainHUD* MainHUD;

	// Skill Widget Instance (PuckPlayerState 에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserWidget")
	class USkillWidget* SkillWidget;

	// SkillWidget OnOff UFunction
	UFUNCTION()
	void SkillWidgetOnOff();

	// Inventory Widget Instance (InventoryComponent 에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserWidget")
	class UInventoryGrid* InventoryGrid;

	// EquipWidget Instance (EquipComponent 에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserWidget")
	class UEquipWidget* EquipWidget;

	// Inventory OnOff UFunction
	UFUNCTION()
	void InventoryOnOff();
	
	// HUD 를 제외한 위젯 set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserWidget")
	TSet<UUserWidget*> WidgetSet;

	// Controller SetInputMode, bShowMouseCursor 처리 함수
	void HUDOnOff(bool bIsOn);

	// Widget 들을 키거나 끄는 함수
	void HandleWidgetOnOff(TSet<UUserWidget*>& InWidgetSet, bool bIsOn);

#pragma endregion
};

