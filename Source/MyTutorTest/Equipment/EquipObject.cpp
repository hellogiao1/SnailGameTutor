// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AEquipObject::AEquipObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	//设置重叠事件为false，在需要的子类中打开即可
	Capsule->SetGenerateOverlapEvents(false);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEquipObject::OnHitActor);

	Attack = 0.f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AEquipObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipObject::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// Called every frame
void AEquipObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipObject::SetGenerateOverlap(bool bInGeneOverlap)
{
	if (Capsule)
	{
		Capsule->SetGenerateOverlapEvents(bInGeneOverlap);
	}
}

