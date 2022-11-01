// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileItem.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AExplosive : public AProjectileItem
{
	GENERATED_BODY()
	
public:
	AExplosive();

protected:
	virtual void TriggerHit(const FHitResult& OutHit) override;

	virtual void OnActorDestroyed() override;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void MultSpawnEffect();

private:
	UPROPERTY(EditAnywhere, Category="Projectile|Show", meta = (AllowPrivateAccess))
	class UParticleSystem* ExplosiveEffect;

	UPROPERTY(EditAnywhere, Category="Projectile|Explosive", meta = (AllowPrivateAccess))
	float SphereRadius;
};
