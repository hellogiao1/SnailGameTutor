// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectileItem.h"
#include "../MyTutorTestCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../Functionality/MyGameplayStatic.h"

AArrow::AArrow()
{
	DestroyTime = 5.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

}

void AArrow::TriggerHit(const FHitResult& OutHit)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(DestroyTimeHandle, this, &AArrow::OnActorDestroyed, DestroyTime, false);
	}

	if (OutHit.BoneName != TEXT("None"))
	{
		if (HasAuthority())
		{
			CalculateDamage(OutHit.BoneName);
			AActor* HitActor = OutHit.GetHitObjectHandle().FetchActor();
			if (UMyGameplayStatic::IsTeam(this, HitActor) == false)
			{
				ApplyDamage(HitActor, ProjectileDamage);
			}
		}
	}
	
}

void AArrow::CalculateDamage(const FName& HitBoneName)
{
	float DamageMultiplier = 1.f;
	if (HitBoneName == TEXT("head"))
	{
		DamageMultiplier = 3.f;
	}
	else
	{
		//If Not Head, Set Damage (x0.75)
		DamageMultiplier = 0.75f;
		
		//If Arrow Hits Chest, Change (x0.75) over to  (x1)
		for (const auto& BoneName : BodyBones)
		{
			if (HitBoneName == BoneName)
			{
				DamageMultiplier = 1.f;
			}
		}
	}

	//Set Damage
	ProjectileDamage *= DamageMultiplier;
}
