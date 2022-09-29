// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterBase.h"
#include "Components/ChildActorComponent.h"
#include "Components/SceneComponent.h"
#include "../Equipment/Sword.h"

// Sets default values
AMyCharacterBase::AMyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建childactor组件
	RightWeaponComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightWeaponComp"));
	RightWeaponComp->SetupAttachment(GetMesh(), FName("hand_rSocket"));
	LeftWeaponComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftWeaponComp"));
	LeftWeaponComp->SetupAttachment(GetMesh(), FName("hand_lSocket"));

	//设置组件插槽和位置
	/*RightWeaponComp->SetRelativeLocation(FVector(-10.916758, 2.920606, -2.822717));
	RightWeaponComp->SetRelativeRotation(FRotator(-0.000275, 0.000000, -89.999939));*/

	LeftWeaponComp->SetRelativeLocation(FVector(15.611130, 1.656690, -1.495595));
	LeftWeaponComp->SetRelativeRotation(FRotator(1.059581, -13.419556, 89.936722));

	RightWeaponComp->SetHiddenInGame(true);
	LeftWeaponComp->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//RightWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("hand_rSocket"));
	//UClass* SwordClass = LoadClass<ASword>(nullptr, TEXT("Blueprint'/Game/ThirdPerson/Blueprints/WeaponActor/BP_Sword.BP_Sword_C'"));
	//RightWeaponComp->SetChildActorClass(SwordClass);
	//LeftWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("hand_lSocket"));
}

// Called every frame
void AMyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

