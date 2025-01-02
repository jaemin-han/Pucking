// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PuckingPlayerCha.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponent/EnhanceInputActorComponent.h"
#include "ActorComponent/RifleActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APuckingPlayerCha::APuckingPlayerCha()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCompo = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmCompo->SetRelativeLocationAndRotation(FVector(0, 80, 70), FRotator(-20, 0, 0));
	SpringArmCompo->SetupAttachment(RootComponent);
	SpringArmCompo->TargetArmLength = 350;
	SpringArmCompo->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmCompo);
	CameraComp->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// ActorComponent
	EnhanceInputActorComponent = CreateDefaultSubobject<UEnhanceInputActorComponent>(TEXT("EnhanceInputActorComponent")); 

	//TODO 플레이어 무기 상태 초기화 Test
}

// Called when the game starts or when spawned
void APuckingPlayerCha::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UActorComponent*> Components;
	GetComponents<UActorComponent>(Components);
		
	for(UActorComponent* ChildActorComponent : Components)
	{
		if(IEquipInterface* WeaponEquipInterface = Cast<IEquipInterface>(ChildActorComponent))
		{
			if(USkeletalMeshComponent* CharacterSkeletal = GetComponentByClass<USkeletalMeshComponent>())
			{
				WeaponEquipInterface->Equip(CharacterSkeletal, FName("GunSocket"), FTransform(FVector::ZeroVector));	
			}
		}
	}
}

// Called every frame
void APuckingPlayerCha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APuckingPlayerCha::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(!InputMappingContext) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 기본 움직임 Input
		EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &APuckingPlayerCha::InputMove);
		EnhancedInputComponent->BindAction(LookUpIA, ETriggerEvent::Triggered, this, &APuckingPlayerCha::InputLook);
		EnhancedInputComponent->BindAction(TurnIA, ETriggerEvent::Triggered, this, &APuckingPlayerCha::InputTurn);
		EnhancedInputComponent->BindAction(JumpIA, ETriggerEvent::Started, this, &APuckingPlayerCha::InputJump);

		// ActorComponent 별 Input
		TArray<UActorComponent*> Components;
		GetComponents<UActorComponent>(Components);
		
		for(UActorComponent* ChildActorComponent : Components)
		{
			if(IBindInputInterface* BindInputInterface = Cast<IBindInputInterface>(ChildActorComponent))
			{
				for(auto ActorComponentInputParam : BindInputInterface->ReturnInputParameter())
				{
					EnhanceInputActorComponent->BindInput(Subsystem, EnhancedInputComponent, ActorComponentInputParam);
				}
			}

			// 인터페이스 리스트 출력
			UClass* ComponentClass = ChildActorComponent->GetClass();
			
			// BlueprintAble로 새로 추가한 ActorComponent만 체크
			if(ComponentClass->GetName().Contains("BP"))
			{
				// Blueprint 대신 Class로
				ComponentClass = ComponentClass->GetSuperClass();
				// 최상위 ActorComponent의 Interface를 체크
				if(ComponentClass->GetSuperClass()->GetName().Len() != FName("ActorComponent").GetStringLength())
				{
					ComponentClass = ComponentClass->GetSuperClass();
				}
				// 상속받은 Interface 검사
				for (const FImplementedInterface& Interface : ComponentClass->Interfaces)
				{
					if (Interface.Class)
					{
						FName MapKeyName = *Interface.Class.GetName();
					
						if(ActorComponentInterfaceMap.Contains(MapKeyName))
						{
							ActorComponentInterfaceMap[MapKeyName].Add(ChildActorComponent);
						}
						else
						{
							ActorComponentInterfaceMap.Add(MapKeyName, { ChildActorComponent });
						}
					}
				}
				
			}
		}
		
	}
}

TArray<UActorComponent*> APuckingPlayerCha::ReturnActorComponents(FName KeyName)
{
	if(ActorComponentInterfaceMap.Contains(KeyName))
	{
		return ActorComponentInterfaceMap[KeyName];
	}
	
	return {};
}

void APuckingPlayerCha::InputMove(const FInputActionValue& Value)
{
	//입력 값을 FVector 형태로
	const FVector _CurrentValue = Value.Get<FVector>();
	
	if(Controller)
	{
		//MoveDirection 벡터에 입력된 X, Y 값을 설정
		MoveDirection.X = _CurrentValue.Y;	//좌우 방향 값
		MoveDirection.Y = _CurrentValue.X;	//전후 방향 값
	}

	//카메라의 현재 회전에 맞춰서 MoveDirection을 변환
	//[위치 - 스케일 - 회전]
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	AddMovementInput(MoveDirection);
	MoveDirection = FVector::ZeroVector;
}

void APuckingPlayerCha::InputLook(const FInputActionValue& Value)
{
	float _CurrentValue = Value.Get<float>() * -1;
	AddControllerPitchInput(_CurrentValue);
}

void APuckingPlayerCha::InputTurn(const FInputActionValue& Value)
{
	const float v = Value.Get<float>();
	AddControllerYawInput(v);
}

void APuckingPlayerCha::InputJump(const FInputActionValue& Value)
{
	Jump();
}