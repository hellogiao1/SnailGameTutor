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
#include "Components/WidgetComponent.h"
#include "UI/UIHeadTipBar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/QuestComponent.h"
#include "MyPlayerController.h"
#include "Player/QuestNPC.h"

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

	// BindDamageEvent
	OnTakeAnyDamage.AddDynamic(this, &AMyTutorTestCharacter::OnCharacterDamage);

	HeadTipWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadTipWidgetComp"));
	HeadTipWidgetComp->SetupAttachment(RootComponent);
	HeadTipWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HeadTipWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	//????????????
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

	HitActor = nullptr;

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

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyTutorTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyTutorTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyTutorTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyTutorTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyTutorTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyTutorTestCharacter::TouchStopped);
}

void AMyTutorTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 2??GetplayerPawn
	if (!HasAuthority() )
	{

		//// ??????????UGamePlayerStatic
		//APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		//if (Pawn)
		//{
		//	FString str = FString::Printf(TEXT("LocalPawn: %s, self Pawn: %s"), *Pawn->GetName(), *GetName());
		//	UKismetSystemLibrary::PrintString(GetWorld(), str, true, true, FLinearColor::Blue, 5.f);
		//}
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUIHeadTipBar* UIHead = Cast<UUIHeadTipBar>(HeadTipWidgetComp->GetWidget());
			if (UIHead)
			{
				UIHead->SetHeadTipforName(FText::FromName(CharacterName));
				UIHead->SetHeadTipforBloodPerctg(HP / MaxHP);
			}
		}
	}
}

void AMyTutorTestCharacter::LineTraceInteraction()
{
	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + (FollowCamera->GetComponentRotation().Vector() * TraceDistance);

	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery4, false, {this}, EDrawDebugTrace::None, OutHit, true);

	if (OutHit.bBlockingHit)
	{
		if (OutHit.GetActor() != HitActor)
		{
			if (AQuestNPC* NPC = Cast<AQuestNPC>(HitActor))
			{
				NPC->EndInteraction(this);
			}

			HitActor = OutHit.GetActor();
			if (AQuestNPC* NPC = Cast<AQuestNPC>(HitActor))
			{
				NPC->InitInteraction(this);
			}
		}
	}
	else
	{
		if (AQuestNPC* NPC = Cast<AQuestNPC>(HitActor))
		{
			NPC->EndInteraction(this);
		}
		HitActor = nullptr;
	}
}


void AMyTutorTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceInteraction();
}

void AMyTutorTestCharacter::OnCharacterDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;
	if (HP <= 0.f) return;

	HP = (HP - Damage) <= 0.f ? 0.f : (HP - Damage);
	if (HP <= 0.f)
	{
		Died();
	}

	//UpdateUI(CharacterName, HP);
}

void AMyTutorTestCharacter::SetCharacterName_Implementation(const FName& NewName)
{
	CharacterName = NewName;

	//UpdateUI(NewName, HP);
}

//void AMyTutorTestCharacter::OnRep_UpdateUI(const FName& NewName, const float Health)
//{
//	if (NewName.IsValid())
//	{
//		OnPropertyChange.Broadcast(Health / MaxHP, FText::FromName(NewName));
//
//		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
//		{
//			UUIHeadTipBar* UIHead = Cast<UUIHeadTipBar>(HeadTipWidgetComp->GetWidget());
//			if (UIHead)
//			{
//				UIHead->SetHeadTipforName(FText::FromName(NewName));
//				UIHead->SetHeadTipforBloodPerctg(Health / MaxHP);
//			}
//		}	
//	}
//}

void AMyTutorTestCharacter::OnRep_UpdateUI()
{
	OnPropertyChange.Broadcast(HP / MaxHP, FText::FromName(CharacterName));

	if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
	{
		UUIHeadTipBar* UIHead = Cast<UUIHeadTipBar>(HeadTipWidgetComp->GetWidget());
		if (UIHead)
		{
			UIHead->SetHeadTipforName(FText::FromName(CharacterName));
			UIHead->SetHeadTipforBloodPerctg(HP / MaxHP);
		}
	}

	//?????????????
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), HP);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (HP <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//??????????????
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), HP);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//?????§Ý????????§Ö?????? 
	/*
		???¦Ê????????????????????????????????????
	*/
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

void AMyTutorTestCharacter::Died_Implementation()
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

float AMyTutorTestCharacter::GetHP()
{
	return HP;
}

void AMyTutorTestCharacter::SetHP(const float NewHealth)
{
	HP = NewHealth;
}

float AMyTutorTestCharacter::GetMaxHP()
{
	return MaxHP;
}

void AMyTutorTestCharacter::SetMaxHP(const float NewMaxHP)
{
	MaxHP = NewMaxHP;
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
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
	DOREPLIFETIME_CONDITION(AMyTutorTestCharacter, HP, COND_None);
}
