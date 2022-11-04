// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTutorTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/UI_PlayerProperty.h"
#include "UI/UI_HUD.h"
#include "Interface/Interaction.h"
#include "Components/WidgetComponent.h"
#include "UI/UIHeadTipBar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/QuestComponent.h"
#include "MyPlayerController.h"
#include "Player/QuestNPC.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Equipment/EquipObject.h"
#include "Enemy/EnemyBase.h"
#include "Engine/World.h"
#include "Projectile/ProjectileItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Functionality/MyGameplayStatic.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StatusComponent.h"


UAbilitySystemComponent* AMyTutorTestCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}



//////////////////////////////////////////////////////////////////////////
// AMyTutorTestCharacter

AMyTutorTestCharacter::AMyTutorTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HeadTipWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadTipWidgetComp"));
	HeadTipWidgetComp->SetupAttachment(RootComponent);
	HeadTipWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HeadTipWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	// Create a QuestComponent
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

	HitActor = nullptr;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	CurrentWeaponType = EWeaponType::None;

	TouchStartPos = FVector::ZeroVector;
	TurnSpeed = -5.f;

	//状态组件
	StatusComp = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
}

void AMyTutorTestCharacter::SetRWeaponGeneOverlap(bool bInGeneraOverlap)
{
	if (RightWeaponComp == nullptr)
		return;

	if (AEquipObject* Equip = Cast<AEquipObject>(RightWeaponComp->GetChildActor()))
	{
		Equip->SetGenerateOverlap(bInGeneraOverlap);
	}
}

void AMyTutorTestCharacter::OnRep_FightState()
{
	if (LeftWeaponComp)
	{
		LeftWeaponComp->SetHiddenInGame(!bFightState);
	}

	if (RightWeaponComp)
	{
		RightWeaponComp->SetHiddenInGame(!bFightState);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyTutorTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("OpenQuest", IE_Pressed, this, &AMyTutorTestCharacter::OpenQuestUI);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMyTutorTestCharacter::OnInteractiveInput);
	PlayerInputComponent->BindAction("SwitchFightState", IE_Pressed, this, &AMyTutorTestCharacter::ServerSwitchState);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AMyTutorTestCharacter::LoopSwitchWeapon);
	/*PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyTutorTestCharacter::NormalAttack);*/
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyTutorTestCharacter::AcceleRun);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyTutorTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyTutorTestCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyTutorTestCharacter::OnTouchPressed);
	PlayerInputComponent->BindTouch(IE_Repeat, this, &AMyTutorTestCharacter::OnTouchMoved);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
// 	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
// 	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyTutorTestCharacter::TurnAtRate);
// 	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
// 	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyTutorTestCharacter::LookUpAtRate);

	// handle touch devices
// 	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyTutorTestCharacter::TouchStarted);
// 	PlayerInputComponent->BindTouch(IE_Released, this, &AMyTutorTestCharacter::TouchStopped);

	//AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(""))
}

void AMyTutorTestCharacter::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchStartPos = Location;
}

void AMyTutorTestCharacter::OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	const FVector DeltaLocation = TouchStartPos - Location;
	AddControllerPitchInput(DeltaLocation.Y * TurnSpeed * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(DeltaLocation.X * TurnSpeed * GetWorld()->GetDeltaSeconds());

	TouchStartPos = Location;
}

void AMyTutorTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 服务器端不执行，但是在客户端和模拟端都执行
	if (!HasAuthority())
	{
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUIHeadTipBar* UIHead = Cast<UUIHeadTipBar>(HeadTipWidgetComp->GetWidget());
			if (UIHead)
			{
				UIHead->SetHeadTipforName(FText::FromName(CharacterName));
				UIHead->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
			}
		}


		//可以判断是模拟端还是客户端
		if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0) || GetLocalRole() == ROLE_AutonomousProxy)
		{
			GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &AMyTutorTestCharacter::LineTraceInteraction, 0.2f, true);
		}
	}

	if (AbilitySystem == nullptr) return;

	if (HasAuthority() && MyAbilities.Num())
	{
		for (auto i = 0; i < MyAbilities.Num(); ++i)
		{
			if (MyAbilities[i] == nullptr) continue;

			AbilitySystem->GiveAbility(FGameplayAbilitySpec(MyAbilities[i].GetDefaultObject(), 1, 0));
		}
	}
	AbilitySystem->InitAbilityActorInfo(this, this);

	for (TSubclassOf<UAttributeSet>& Set : AttributeSets)
	{
		AbilitySystem->InitStats(Set, AttrDataTable);
	}
}

void AMyTutorTestCharacter::LineTraceInteraction()
{
	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + (FollowCamera->GetComponentRotation().Vector() * TraceDistance);

	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery4, false, { this }, EDrawDebugTrace::None, OutHit, true);

	if (OutHit.bBlockingHit)
	{
		if (OutHit.GetActor() != HitActor)
		{
			// 观察者模式，我不需要具体知道是哪个观察者被调用，只要它实现了这个接口就行

			if (IInteraction* Observer = Cast<IInteraction>(HitActor))
			{
				Observer->EndInteraction(this);
			}

			HitActor = OutHit.GetActor();
			if (IInteraction* Observer = Cast<IInteraction>(HitActor))
			{
				Observer->InitInteraction(this);
			}
		}
	}
	else
	{
		if (IInteraction* Observer = Cast<IInteraction>(HitActor))
		{
			Observer->EndInteraction(this);
		}
		HitActor = nullptr;
	}
}


void AMyTutorTestCharacter::OnHealthUpdate()
{
	//客户端特定的功能
	if (IsLocallyControlled())
	{
		OnPropertyChange.Broadcast(CurrentHealth / MaxHealth, FText::FromName(CharacterName));

		if (CurrentHealth <= 0)
		{
			Died();
		}
	}

	//服务器特定的功能
	if (GetLocalRole() == ROLE_Authority)
	{

	}

	//客户端和模拟端走的功能
	if (GetLocalRole() != ROLE_Authority)
	{
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUIHeadTipBar* UIHead = Cast<UUIHeadTipBar>(HeadTipWidgetComp->GetWidget());
			if (UIHead)
			{
				UIHead->SetHeadTipforName(FText::FromName(CharacterName));
				UIHead->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
			}
		}
	}

	//在所有机器上都执行的函数。 
	/*
		因任何因伤害或死亡而产生的特殊功能都应放在这里。
	*/
	if (CurrentHealth <= 0.f)
	{
		OnCharcterDied();
	}

}

void AMyTutorTestCharacter::OnCharcterDied()
{
	Super::OnCharcterDied();
}

void AMyTutorTestCharacter::NetMult_PlayMontage_Implementation(UAnimMontage* Montage)
{
	//模拟端和服务端进行播放动画
	if (GetLocalRole() != ROLE_AutonomousProxy)
	{
		UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInst)
		{
			AnimInst->Montage_Play(Montage);
		}
	}
}

void AMyTutorTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//LineTraceInteraction();
}

void AMyTutorTestCharacter::SetCharacterName_Implementation(const FName& NewName)
{
	CharacterName = NewName;

	//UpdateUI(NewName, HP);
}

void AMyTutorTestCharacter::OnInteractiveInput()
{
	InteractionEvent.ExecuteIfBound();
}


void AMyTutorTestCharacter::OpenQuestUI()
{
	AMyPlayerController* MyController = Cast<AMyPlayerController>(Controller);
	if (MyController)
	{
		MyController->SwitchQuestMain(this, EActivateQuest::Self);
	}
}


void AMyTutorTestCharacter::SetNPCPtr(const AMyCharacterBase* Character)
{
	InteraCharacter = const_cast<AMyCharacterBase*>(Character);
}


bool AMyTutorTestCharacter::ExistItem(UClass* ItemClass)
{
	for (AActor* Item : Inventory)
	{
		if (Item->GetClass() == ItemClass)
		{
			return true;
		}
	}
	return false;
}


void AMyTutorTestCharacter::AddItem(AActor* Item)
{
	Inventory.Emplace(Item);
}


void AMyTutorTestCharacter::DeleteItem(AActor* Item)
{
	for (AActor* It : Inventory)
	{
		if (It == Item)
		{
			Inventory.Remove(It);
		}
	}
}

void AMyTutorTestCharacter::AddQuestAsObject(EQuestTarget QuestTarget, TSubclassOf<AActor> TargetObject, int32 Count)
{
	if (QuestComp)
	{
		QuestComp->UpdateRequest(QuestTarget, TargetObject, Count);
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			PlayerController->ShowOnProgressQuest();
		}
	}
}

void AMyTutorTestCharacter::NotifyQuestReachPos(FVector TargetPosition, bool bReach)
{
	if (QuestComp)
	{
		QuestComp->UpdateRequest(EQuestTarget::GoToArea, TargetPosition, bReach);
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			PlayerController->ShowOnProgressQuest();
		}
	}
}

void AMyTutorTestCharacter::ServerSwitchState_Implementation()
{
	bFightState = !bFightState;
	if (CurrentWeaponType == EWeaponType::None)
	{
		ServerSwitchWeapon(EWeaponType::MainHandAndShied);
	}
}

void AMyTutorTestCharacter::ServerSwitchWeapon_Implementation(EWeaponType WeaponType)
{
	if (bFightState)
	{
		NetMulSwitchWeapon(WeaponType);
	}
}

void AMyTutorTestCharacter::NetMulSwitchWeapon_Implementation(EWeaponType WeaponType)
{
	if (Map_WeaponClassPaths.Num())
	{
		switch (WeaponType)
		{
		case EWeaponType::Fists:
			break;
		case EWeaponType::MainHand:
			break;
		case EWeaponType::MainHandAndShied:
		{
			if (RightWeaponComp && LeftWeaponComp)
			{
				UClass* LeftWeaponClass = Map_WeaponClassPaths[WeaponType].WeaponSoftArray[0].TryLoadClass<AEquipObject>();
				LeftWeaponComp->SetChildActorClass(LeftWeaponClass);
				LeftWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LHand_Shield"));

				if (LeftWeaponComp->GetChildActor())
				{
					LeftWeaponComp->GetChildActor()->SetOwner(this);
				}

				UClass* RightWeaponClass = Map_WeaponClassPaths[WeaponType].WeaponSoftArray[1].TryLoadClass<AEquipObject>();
				RightWeaponComp->SetChildActorClass(RightWeaponClass);
				RightWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));

				if (RightWeaponComp->GetChildActor())
				{
					RightWeaponComp->GetChildActor()->SetOwner(this);
				}

			}
		}
		break;
		case EWeaponType::Bow:
		{
			if (LeftWeaponComp)
			{
				UClass* LeftWeaponClass = Map_WeaponClassPaths[WeaponType].WeaponSoftArray[0].TryLoadClass<AEquipObject>();
				LeftWeaponComp->SetChildActorClass(LeftWeaponClass);
				LeftWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LHand_Bow"));

				if (RightWeaponComp)
				{
					RightWeaponComp->DestroyChildActor();
				}

				if (LeftWeaponComp->GetChildActor())
				{
					LeftWeaponComp->GetChildActor()->SetOwner(this);
				}
			}
		}
		break;
		case EWeaponType::Magic:
			break;
		default:
			break;
		}

		CurrentWeaponType = WeaponType;

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s(),EquipClassPaths is empty,please fill in the blueprint "), *FString(__FUNCTION__));
		return;
	}
}

void AMyTutorTestCharacter::LoopSwitchWeapon()
{
	if (bFightState)
	{
		if (CurrentWeaponType == EWeaponType::MainHandAndShied)
		{
			CurrentWeaponType = EWeaponType::Bow;
		}
		else if (CurrentWeaponType == EWeaponType::Bow)
		{
			CurrentWeaponType = EWeaponType::MainHandAndShied;
		}
		ServerSwitchWeapon(CurrentWeaponType);
	}
}

void AMyTutorTestCharacter::AttackBtn_Down(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Normal:
	{
		NormalAttack();
	}
	break;
	default:
		break;
	}

	ServerAttackNotify(AttackType);
}

void AMyTutorTestCharacter::ServerAttackNotify_Implementation(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Normal:
	{
		NormalAttack();
	}
	break;
	default:
		break;
	}
}

void AMyTutorTestCharacter::AttackBtn_Release(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Normal:
	{
		NormalAttackBtn_Release();
	}
	break;
	default:
		break;
	}

	Server_AttackBtn_Release(AttackType);
}

void AMyTutorTestCharacter::Server_AttackBtn_Release_Implementation(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Normal:
	{
		NormalAttackBtn_Release();
	}
	break;
	default:
		break;
	}
}

bool AMyTutorTestCharacter::SetRWeaponFinishFrame(bool bFishFrame)
{
	AEquipObject* EquipObject = RightWeaponComp ? Cast<AEquipObject>(RightWeaponComp->GetChildActor()) : nullptr;
	if (EquipObject)
	{
		EquipObject->SetbHitFrameFinish(bFishFrame);
		return true;
	}
	return false;
}

AEquipObject* AMyTutorTestCharacter::GetRightWeaponInst()
{
	return RightWeaponComp ? Cast<AEquipObject>(RightWeaponComp->GetChildActor()) : nullptr;
}

AEquipObject* AMyTutorTestCharacter::GetLeftWeaponInst()
{
	LeftEquipObject = LeftWeaponComp ? Cast<AEquipObject>(LeftWeaponComp->GetChildActor()) : nullptr;
	return LeftEquipObject;
}

EWeaponType AMyTutorTestCharacter::GetCurrentWeaponOutput()
{
	return CurrentWeaponType;
}

void AMyTutorTestCharacter::SetCurrentWeaponType(EWeaponType NewWeaponType)
{
	CurrentWeaponType = NewWeaponType;
}

void AMyTutorTestCharacter::NormalAttack()
{
	if (bFightState)
	{
		AEquipObject* EquipObject = RightWeaponComp ? Cast<AEquipObject>(RightWeaponComp->GetChildActor()) : nullptr;
		if (EquipObject == nullptr)
		{
			EquipObject = LeftWeaponComp ? Cast<AEquipObject>(LeftWeaponComp->GetChildActor()) : nullptr;
		}

		if (EquipObject)
		{
			EquipObject->Excute_NormalAttack();
		}
	}
}

void AMyTutorTestCharacter::NormalAttackBtn_Release()
{
	if (bFightState)
	{
		AEquipObject* EquipObject = RightWeaponComp ? Cast<AEquipObject>(RightWeaponComp->GetChildActor()) : nullptr;
		if (EquipObject == nullptr)
		{
			EquipObject = LeftWeaponComp ? Cast<AEquipObject>(LeftWeaponComp->GetChildActor()) : nullptr;
		}

		if (EquipObject)
		{
			EquipObject->OnNormalBtn_Release();
		}
	}
}

//void AMyTutorTestCharacter::ServerLaunchProjectile_Implementation(UClass* SpawnClass)
//{
//	//Try and fire a projectile
//	if (SpawnClass != nullptr)
//	{
//		UWorld* const World = GetWorld();
//		if (World != nullptr)
//		{
//			//生成箭的位置和旋转：在弓箭前方生成，避免和角色以及弓箭误碰
//			FRotator CameraRotation = FollowCamera->GetComponentRotation();
//			const FRotator SpawnRotation = CameraRotation;
//			const FVector SpawnLocation = LeftWeaponComp->GetComponentLocation() + FollowCamera->GetForwardVector() * 50;
//
//			FActorSpawnParameters ActorSpawnParams;
//			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//			AProjectileItem* SpawnedArrow = World->SpawnActor<AProjectileItem>(SpawnClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
//			if (SpawnedArrow)
//			{
//				SpawnedArrow->SetOwner(this);
//			}
//
//			FHitResult OutHit;
//			TArray<FVector> OutPathPositions;
//			FVector OutLastTraceDestination;
//			FVector StartPos = GetCameraArrowLevel();
//			float Speed = 6000.f;
//			FVector LaunchVelocity = GetControlRotation().Vector() * Speed;
//			//预测箭的抛物线，获得最终到达的点位置，其中ECC_GameTraceChannel1对应的是项目设置的DamageTraceHit射线通道
//			bool bHit = UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), OutHit, OutPathPositions, OutLastTraceDestination,
//				StartPos, LaunchVelocity, true, 3.f, ECollisionChannel::ECC_GameTraceChannel2, 
//				false, {}, EDrawDebugTrace::ForDuration, 1.f, 15.f, 6.f, 0.f);
//
//			FVector ArrowEndLocation = bHit ? OutHit.ImpactPoint : OutLastTraceDestination;
//			FVector ArrowStartLocation = LeftWeaponComp->GetComponentLocation() + FollowCamera->GetForwardVector() * 50.f;
//			FVector ArrowVelocity = CalcArrowVelocity(ArrowStartLocation, ArrowEndLocation);
//			MultSetArrowVelocity(SpawnedArrow, ArrowVelocity);
//		}
//	}
//}

void AMyTutorTestCharacter::ServerLaunchProjectile_Implementation(UClass* SpawnClass, FRotator Rotation, FVector Location, bool bAimView /*= false*/)
{
	//Try and fire a projectile
	if (SpawnClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FHitResult OutHit;
			TArray<FVector> OutPathPositions;
			FVector OutLastTraceDestination;
			FVector StartPos = bAimView == false ? Location : GetCameraArrowLevel();
			float Speed = 4000.f;
			FVector LaunchVelocity = Rotation.Vector() * Speed;
			//预测箭的抛物线，获得最终到达的点位置，其中ECC_GameTraceChannel1对应的是项目设置的DamageTraceHit射线通道
			bool bHit = UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), OutHit, OutPathPositions, OutLastTraceDestination,
				StartPos, LaunchVelocity, true, 3.f, ECollisionChannel::ECC_GameTraceChannel2,
				false, {}, EDrawDebugTrace::ForDuration, 1.f, 15.f, 6.f, 0.f);

			FVector ArrowEndLocation = bHit ? OutHit.ImpactPoint : OutLastTraceDestination;
			FVector ArrowStartLocation = Location;
			FVector ArrowVelocity = CalcArrowVelocity(ArrowStartLocation, ArrowEndLocation, Speed);

			const FRotator SpawnRotation = Rotation;
			const FVector SpawnLocation = Location;

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//AProjectileItem* SpawnedArrow = World->SpawnActor<AProjectileItem>(SpawnClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			AProjectileItem* SpawnedArrow = World->SpawnActorDeferred<AProjectileItem>(SpawnClass, FTransform(ArrowVelocity.ToOrientationRotator(), SpawnLocation), this);
			if (SpawnedArrow)
			{
				SpawnedArrow->SetOwner(this);
			}

			if (SpawnedArrow && SpawnedArrow->ProjectileMove)
			{
				SpawnedArrow->ProjectileMove->Velocity = ArrowVelocity;
				//SpawnedArrow->SetVelocity(ArrowVelocity);
			}
			
			//这里有问题：当你创建一个新的Actor的同时（比如在一个函数内），你将这个Actor作为RPC的参数传到客户端去执行，这时候你会发现客户端的RPC函数的参数为NULL
			//因为RPC函数先执行，同步后执行
			//MultSetArrowVelocity(SpawnedArrow, ArrowVelocity);
			UGameplayStatics::FinishSpawningActor(SpawnedArrow, FTransform(ArrowVelocity.ToOrientationRotator(), SpawnLocation));
			//UGameplayStatics::FinishSpawningActor(SpawnedArrow, FTransform(SpawnRotation, SpawnLocation));
		}
	}
}

bool AMyTutorTestCharacter::TraceTarget(FVector& OutTarget, float MaxTraceLen /*= 2000.f*/)
{
	if (FollowCamera == nullptr || CameraBoom == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] ->TraceTarget failed"), *FString(__FUNCTION__));
		return false;
	}

	FVector StarTracetLoc = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * CameraBoom->TargetArmLength;
	FVector EndTraceLoc = StarTracetLoc + FollowCamera->GetForwardVector() * MaxTraceLen;

	TArray<FHitResult> OutHits;

	bool bHit = UKismetSystemLibrary::LineTraceMulti(GetWorld(), StarTracetLoc, EndTraceLoc, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {}, EDrawDebugTrace::None, OutHits, true);
	if (bHit)
	{
		float MinDistance = (EndTraceLoc - StarTracetLoc).Size();
		bool bValidPos = false;
		for (const auto& Hit : OutHits)
		{
			if (UMyGameplayStatic::IsTeam(Hit.GetActor(), this))
			{
				continue;
			}

			float TargetDistance = (Hit.Location - StarTracetLoc).Size();
			if (MinDistance >= TargetDistance)
			{
				MinDistance = TargetDistance;
				OutTarget = Hit.Location;
			}

			bValidPos = true;
		}

		if (bValidPos == false)
		{
			bHit = false;
			OutTarget = EndTraceLoc;
		}
	}
	else
	{
		OutTarget = EndTraceLoc;
	}

	return bHit;
}

void AMyTutorTestCharacter::BowLaunchProjectile(TSubclassOf<AProjectileItem> SpawnClass)
{
	UWorld* const World = GetWorld();
	if (World != nullptr && SpawnClass != nullptr)
	{
		FVector SpawnLocation = LeftWeaponComp->GetComponentLocation();
		FVector HitTarget;
		bool bHit = TraceTarget(HitTarget);
		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitTarget);
		UKismetSystemLibrary::PrintString(GetWorld(), SpawnRotation.ToString());
		
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectileItem* SpawnedArrow = World->SpawnActor<AProjectileItem>(SpawnClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		if (SpawnedArrow && SpawnedArrow->ProjectileMove)
		{
			SpawnedArrow->SetOwner(this);
			SpawnedArrow->ProjectileMove->Velocity = UKismetMathLibrary::Conv_RotatorToVector(SpawnRotation).GetSafeNormal() * 1000.f;
			SpawnedArrow->Init(!bHit, FollowCamera->GetComponentRotation(), HitTarget);
		}
	}
}

FVector AMyTutorTestCharacter::GetCameraArrowLevel()
{
	float ForwardLength = (LeftWeaponComp->GetComponentLocation() + FollowCamera->GetForwardVector() * 50.f - FollowCamera->GetComponentLocation()).Size();
	FVector ForwardVector = GetControlRotation().Vector() * ForwardLength;
	return ForwardVector + FollowCamera->GetComponentLocation();
}

FVector AMyTutorTestCharacter::CalcArrowVelocity(FVector StartLocation, FVector EndLocation, float Speed)
{
	FVector TossVelocity;
	UGameplayStatics::BlueprintSuggestProjectileVelocity(GetWorld(), TossVelocity, StartLocation, EndLocation, Speed, 0.f, ESuggestProjVelocityTraceOption::DoNotTrace, 0.f, false, false);
	return TossVelocity;
}

void AMyTutorTestCharacter::MultSetArrowVelocity_Implementation(AProjectileItem* SpawnedArrow, FVector TossVelocity)
{
	if (SpawnedArrow && SpawnedArrow->ProjectileMove)
	{
		SpawnedArrow->ProjectileMove->Velocity = TossVelocity;
	}
}

void AMyTutorTestCharacter::Died()
{
	if (UIDiedClass != nullptr)
	{
		UUserWidget* UIDied = CreateWidget<UUserWidget>(GetWorld(), UIDiedClass);
		if (UIDied)
		{
			UIDied->AddToViewport();
		}
	}
}

float AMyTutorTestCharacter::GetLevel()
{
	return Level;
}


void AMyTutorTestCharacter::SetLevel(const float NewLevel)
{
	Level = NewLevel;
}

const FName& AMyTutorTestCharacter::GetCharacterName()
{
	return CharacterName;
}


void AMyTutorTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMyTutorTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMyTutorTestCharacter::AcceleRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AMyTutorTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyTutorTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyTutorTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyTutorTestCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyTutorTestCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMyTutorTestCharacter, CharacterName, COND_None);
	DOREPLIFETIME_CONDITION(AMyTutorTestCharacter, bFightState, COND_None);
}
