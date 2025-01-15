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

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

#pragma region IEssenceInterface, IHealthMarbleInterface
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Essence", meta = (AllowPrivateAccess = "true"))
	class APuckPlayerState* PuckPlayerState;
public:
	virtual void AddEssence(const int32 AddEssence) override;
	virtual void ApplyHeal(float HealAmount) override;
#pragma endregion

#pragma region CloseCombatComponent, Hammer
private:
	UPROPERTY(EditAnywhere, Category = "CloseCombat")
	class UCloseCombatComponent* CloseCombatComponent;
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
	
};

