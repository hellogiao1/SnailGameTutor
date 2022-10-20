// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileItem.h"

// Sets default values
AProjectileItem::AProjectileItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*ProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMove"));*/
}

// Called when the game starts or when spawned
void AProjectileItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

