// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Functionality/MyGameplayStatic.h"

AExplosive::AExplosive()
{
	SphereRadius = 100.f;
}

void AExplosive::TriggerHit(const FHitResult& OutHit)
{
	OnActorDestroyed();
}

void AExplosive::OnActorDestroyed()
{
	if (HasAuthority())
	{
		TArray<class AActor*> OutActors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SphereRadius, { EObjectTypeQuery::ObjectTypeQuery3 }, nullptr, { this }, OutActors);

		for (AActor* It : OutActors)
		{
			if (UMyGameplayStatic::IsTeam(GetOwner(), It) == false)
			{
				It->TakeDamage(ProjectileDamage, FDamageEvent(), nullptr, GetOwner());
			}
		}
	}
	
	MultSpawnEffect();

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &AProjectileItem::K2_DestroyActor, 0.2f, false);
	}
}

void AExplosive::MultSpawnEffect_Implementation()
{
	if (!HasAuthority() && ExplosiveEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosiveEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(int32(SphereRadius) / 100));
	}
}
