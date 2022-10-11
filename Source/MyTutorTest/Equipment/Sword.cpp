// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Components/CapsuleComponent.h"
#include "../Enemy/EnemyBase.h"

ASword::ASword()
{
	Attack = 20.f;
}

void ASword::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor))
	{
		
	}
}
