// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuckingCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ActorComponent/CloseCombatComponent.h"
#include "InputMappingContext.h"
#include "ActorComponent/EnhanceInputActorComponent.h"
#include "ActorComponent/HookComponent.h"
#include "ActorComponent/JetPackMovementComponent.h"
#include "ActorComponent/PlayerStatusComponent.h"
#include "Blueprint/UserWidget.h"
#include "Common/CommonStruct.h"
#include "Interfaces/BindInputInterface.h"
#include "Interfaces/DelegateInterface.h"
#include "Interfaces/GetMagazineInterface.h"
#include "Interfaces/IsCurWeaponTypeInterface.h"
#include "World/PuckPlayerState.h"
#include "UI/HUD/MainHUD.h"
#include "UI/Skill/SkillWidget.h"
#include "UI/Inventory/InventoryGrid.h"
#include "UI/Equip/EquipWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APuckingCharacter

APuckingCharacter::APuckingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	// 카메라가 캐릭터 오른쪽에 오게 추가
	CameraBoom->SetRelativeLocationAndRotation(FVector(0, 80, 70), FRotator(-20, 0, 0));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EnhanceInputActorComponent = CreateDefaultSubobject<
		UEnhanceInputActorComponent>(TEXT("EnhanceInputActorComponent"));
	//CloseCombatComponent = CreateDefaultSubobject<UCloseCombatComponent>(TEXT("CloseCombatCompnent"));

	// pause 상태에서도 tick 이 동작하도록 설정
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void APuckingCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	Tags.Add(FName("Player"));

	// EssenceInterface
	PuckPlayerState = Cast<APuckPlayerState>(GetController()->PlayerState);

	// HealthMarbleInterface
	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (auto* Component : Components)
	{
		if (UPlayerStatusComponent* StatusComponent = Cast<UPlayerStatusComponent>(Component))
		{
			PlayerStatusComponent = StatusComponent;
		}
	}

	// EquipComponent Delegate
	if (UEquipComponent* EquipComponent = FindComponentByClass<UEquipComponent>())
	{
		// MainHUD
		if (MainHUDClass)
		{
			MainHUD = CreateWidget<UMainHUD>(GetWorld(), MainHUDClass);
			MainHUD->AddToViewport(0);
			MainHUD->SetAmmoImageTintRed(0);
			EquipComponent->MainHUD = MainHUD;
		}

		// 총알 소비할 때 UI에 반영하는 Delegate Event Bind
		for (UActorComponent* GunActorComponent : Components)
		{
			if (IDelegateInterface* HasMagazineComponent = Cast<IDelegateInterface>(GunActorComponent))
			{
				MainHUD->BindMagazineUIEvent(HasMagazineComponent);
			}
		}

		// Delegate Event Bind
		EquipComponent->OnWeaponTypeChanged.AddDynamic(this, &APuckingCharacter::ChangeWeaponInputMapping);
		EquipComponent->OnWeaponTypeChanged.AddDynamic(this, &APuckingCharacter::GetRemainMagazine);
		EquipComponent->OnWeaponTypeChanged.Broadcast(EWeaponType::Rifle);
	}

	if (UJetPackMovementComponent* JetPackComponent = FindComponentByClass<UJetPackMovementComponent>())
	{
		JetPackComponent->SubCoolTimeUI = MainHUD->CoolTimeUI;
	}

	if (UHookComponent* HookComponent = FindComponentByClass<UHookComponent>())
	{
		HookComponent->SubCoolTimeUI = MainHUD->CoolTimeUI;
	}

	PC = GetWorld()->GetFirstPlayerController();
}

void APuckingCharacter::AddEssence(const int32 AddEssence)
{
	if (!IsValid(PuckPlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("PuckPlayerState is nullptr"));
		return;
	}

	PuckPlayerState->AddEssence(AddEssence);
}

void APuckingCharacter::ApplyHeal(float HealAmount)
{
	if (!IsValid(PlayerStatusComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStatusComponent is nullptr"));
		return;
	}
	PlayerStatusComponent->EatHealingPack(HealAmount);
}

void APuckingCharacter::OnCombatCompAttachment(UStaticMeshComponent* TargetMeshComp, USceneComponent* BoxTraceStart,
                                               USceneComponent* BoxTraceEnd)
{
	// FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	// FAttachmentTransformRules TransformRules_Relative(EAttachmentRule::KeepRelative, true);
	// if(CloseCombatComponent && TargetMeshComp && BoxTraceStart && BoxTraceEnd)
	// {
	// 	TargetMeshComp->AttachToComponent(GetMesh(), TransformRules, "CloseCombatSocket");
	// 	BoxTraceStart->AttachToComponent(TargetMeshComp, TransformRules_Relative);
	// 	BoxTraceEnd->AttachToComponent(TargetMeshComp, TransformRules_Relative);
	// 	CloseCombatComponent->AttachToComponent(TargetMeshComp, TransformRules_Relative);
	// 	CloseCombatComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	
	// 	TargetMeshComp->SetStaticMesh(HammerMesh);
	// }
}

void APuckingCharacter::DeactivateAllMappingContext()
{
	TArray<FInputParameter> ChangedParameters;
	for (UActorComponent* BindComponent : BindComponents)
	{
		// Input Bind할 Parameter를 받을 수 있는 Interface
		IBindInputInterface* BindInputInterface = Cast<IBindInputInterface>(BindComponent);
		if (BindInputInterface)
		{
			for (auto& ActorComponentInputParam : BindInputInterface->ReturnInputParameter())
			{
				EnhanceInputActorComponent->DeactivateMappingContext(Subsystem, EnhancedInputComponent,ActorComponentInputParam);
			}
		}
	}
}

// Change WeaponType
void APuckingCharacter::ChangeWeaponInputMapping(EWeaponType ChangedWeaponType)
{
	TArray<FInputParameter> ChangedParameters;
	for (UActorComponent* BindComponent : BindComponents)
	{
		// 현재 WeaponType을 체크할 수 있는 Interface
		IIsCurWeaponTypeInterface* CurWeaponTypeInterface = Cast<IIsCurWeaponTypeInterface>(BindComponent);

		// Input Bind할 Parameter를 받을 수 있는 Interface
		IBindInputInterface* BindInputInterface = Cast<IBindInputInterface>(BindComponent);

		if (CurWeaponTypeInterface && BindInputInterface)
		{
			for (auto& ActorComponentInputParam : BindInputInterface->ReturnInputParameter())
			{
				// 먼저 전체 InputAction을 삭제
				EnhanceInputActorComponent->DeactivateMappingContext(Subsystem, EnhancedInputComponent,
				                                                     ActorComponentInputParam);

				// 현재 WeaponType 체크
				bool IsCurWeaponType = CurWeaponTypeInterface->IsCurWeaponType(ChangedWeaponType);
				if (IsCurWeaponType)
				{
					ChangedParameters.Add(ActorComponentInputParam);
				}
			}
		}
	}

	for (int32 i = 0; i < ChangedParameters.Num(); i++)
	{
		// 현재 WeaponType만 다시 추가
		EnhanceInputActorComponent->ActivateMappingContext(Subsystem, EnhancedInputComponent, ChangedParameters[i]);
	}
}

void APuckingCharacter::GetRemainMagazine(EWeaponType TargetWeapon)
{
	for (UActorComponent* BindComponent : BindComponents)
	{
		if (IGetMagazineInterface* WeaponInterface = Cast<IGetMagazineInterface>(BindComponent))
		{
			if (TargetWeapon == WeaponInterface->GetWeaponType())
			{
				MainHUD->SetSubHUDMagazine(WeaponInterface->GetMaxMagazine(), WeaponInterface->GetCurMagazine());
			}
		}
	}
}

void APuckingCharacter::SkillWidgetOnOff()
{
	if (!SkillWidget)
		return;

	TSet<UUserWidget*> SkillWidgetSet;
	SkillWidgetSet.Add(SkillWidget);

	if (SkillWidget->IsInViewport())
	{
		HandleWidgetOnOff(SkillWidgetSet, false);
	}
	else
	{
		HandleWidgetOnOff(SkillWidgetSet, true);
	}
}

void APuckingCharacter::InventoryOnOff()
{
	if (!InventoryGrid || !EquipWidget)
		return;

	TSet<UUserWidget*> InventoryWidgetSet;
	InventoryWidgetSet.Add(InventoryGrid);
	InventoryWidgetSet.Add(EquipWidget);
	//InventoryWidgetSet.Add();

	if (InventoryGrid->IsInViewport())
	{
		HandleWidgetOnOff(InventoryWidgetSet, false);
	}
	else
	{
		HandleWidgetOnOff(InventoryWidgetSet, true);
	}
}

void APuckingCharacter::HUDOnOff(bool bIsOn)
{
	if (bIsOn)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PC->SetPause(false);
		MainHUD->AddToViewport();
	}
	else
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
		MainHUD->RemoveFromParent();
	}
}

void APuckingCharacter::HandleWidgetOnOff(TSet<UUserWidget*>& InWidgetSet, bool bIsOn)
{
	if (bIsOn)
	{
		// WidgetSet 에서 InWidgetSet 에 포함된 Widget 만 AddToViewport
		for (UUserWidget* Widget : WidgetSet)
		{
			if (InWidgetSet.Contains(Widget))
			{
				// UE_LOG(LogTemp, Warning, TEXT("AddToViewport : %s"), *Widget->GetName());
				Widget->AddToViewport();
			}
			else
			{
				// UE_LOG(LogTemp, Warning, TEXT("RemoveFromParent : %s"), *Widget->GetName());
				Widget->RemoveFromParent();
			}
		}
	}
	else
	{
		// InWidgetSet 을 RemoveFromParent
		for (UUserWidget* Widget : InWidgetSet)
		{
			// UE_LOG(LogTemp, Warning, TEXT("RemoveFromParent : %s"), *Widget->GetName());
			Widget->RemoveFromParent();
		}
	}

	/*	WidgetSet 에 Viewport 에 있는 Widget 이 하나도 없다면
	 *	OnWidgetOnOff(false) 를 호출해서 InputMode 를 GameOnly 로 설정
	 * 	그게 아니라 Viewport 에 있는 Widget 이 하나라도 있다면
	 * 	OnWidgetOnOff(true) 를 호출해서 InputMode 를 GameAndUI 로 설정
	 */
	bool bIsWidgetOn = false;
	for (UUserWidget* Widget : WidgetSet)
	{
		if (Widget->IsInViewport())
		{
			// UE_LOG(LogTemp, Warning, TEXT("Widget is in Viewport : %s"), *Widget->GetName());
			bIsWidgetOn = true;
			break;
		}
	}

	if (bIsWidgetOn)
	{
		HUDOnOff(false);
		if(OnToggleWidget.IsBound())
		{
			//Crosshair UI 안 보이게
			OnToggleWidget.Broadcast(false);
		}
	}
	else
	{
		HUDOnOff(true);
		if(OnToggleWidget.IsBound())
		{
			//Crosshair UI 보이게
			OnToggleWidget.Broadcast(true);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void APuckingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APuckingCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APuckingCharacter::Look);

		// SkillWidget OnOff
		EnhancedInputComponent->BindAction(SkillWidgetOnOffAction, ETriggerEvent::Started, this,
		                                   &APuckingCharacter::SkillWidgetOnOff);

		// Inventory OnOff
		EnhancedInputComponent->BindAction(InventoryOnOffAction, ETriggerEvent::Started, this,
		                                   &APuckingCharacter::InventoryOnOff);

		/*TArray<UActorComponent*> Components;
		GetComponents<UActorComponent>(Components);*/

		TArray<UActorComponent*> Components = GetComponentsByInterface(UBindInputInterface::StaticClass());

		for (UActorComponent* ChildActorComponent : Components)
		{
			if (IBindInputInterface* BindInputInterface = Cast<IBindInputInterface>(ChildActorComponent))
			{
				BindComponents.Add(ChildActorComponent);
				for (auto& ActorComponentInputParam : BindInputInterface->ReturnInputParameter())
				{
					EnhanceInputActorComponent->BindInput(Subsystem, EnhancedInputComponent, ActorComponentInputParam);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void APuckingCharacter::Move(const FInputActionValue& Value)
{
	if (!bIsMovable) return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APuckingCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APuckingCharacter::GetHit(const FHitResult& Hit)
{
}
