// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AExplosive::AExplosive()
{
	SphereRadius = 100.f;
}

void AExplosive::TriggerHit()
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
			It->TakeDamage(ProjectileDamage, FDamageEvent(), nullptr, GetOwner());
		}
	}

	if (ExplosiveEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosiveEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(int32(SphereRadius) / 100));
	}

	Destroy();
}
