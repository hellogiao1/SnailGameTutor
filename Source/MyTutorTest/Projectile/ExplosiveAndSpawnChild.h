// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileItem.h"
#include "ExplosiveAndSpawnChild.generated.h"

USTRUCT(BlueprintType)
struct FChildSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectileItem> ProjectileChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandowDirection == false", EditConditionHides, ClampMax = 360.f, ClampMin = -360.f))
	float OffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandowDirection == false", EditConditionHides, ClampMax = 360.f, ClampMin = -360.f))
	float OffsetRoll;

	UPROPERTY(EditAnywhere)
	bool bRandowDirection = true;
};

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AExplosiveAndSpawnChild : public AProjectileItem
{
	GENERATED_BODY()
	
public:


protected:
	virtual void TriggerHit(const FHitResult& OutHit) override;

	virtual void OnActorDestroyed() override;

private:
	UPROPERTY(EditAnywhere, Category="Projectile|SpawnChild", meta=(AllowPrivateAccess))
	TArray<FChildSpawn> ChildSpawns; 
};
