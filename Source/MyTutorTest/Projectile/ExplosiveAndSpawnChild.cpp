// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveAndSpawnChild.h"
#include "../MyTutorTestCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileItem.h"
#include "Components/SphereComponent.h"

void AExplosiveAndSpawnChild::TriggerHit(const FHitResult& OutHit)
{
	OnActorDestroyed();
}

void AExplosiveAndSpawnChild::OnActorDestroyed()
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(GetOwner());
	for (const FChildSpawn& It : ChildSpawns)
	{
		if (Tutor)
		{
			FRotator spawnRotation = FRotator(GetActorRotation().Pitch + 180.f, GetActorRotation().Yaw + It.OffsetYaw, GetActorRotation().Roll + It.OffsetRoll);
			Tutor->ServerLaunchProjectile(It.ProjectileChild, spawnRotation, GetActorLocation());
		}
	}

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &AProjectileItem::K2_DestroyActor, 0.2f, false);
	}
}
