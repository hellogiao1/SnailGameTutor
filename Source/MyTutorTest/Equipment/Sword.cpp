// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Components/CapsuleComponent.h"
#include "../Enemy/EnemyBase.h"

ASword::ASword()
{
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword06SM.Sword06SM'"));
	if (SwordMesh.Succeeded())
	{
		Weapon->SetStaticMesh(SwordMesh.Object);
	}*/
}

void ASword::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor))
	{
		Enemy->
	}
}
