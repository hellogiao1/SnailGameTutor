// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileItem.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AArrow : public AProjectileItem
{
	GENERATED_BODY()
	
public:
	AArrow();

	virtual void BeginPlay() override;

protected:
	virtual void TriggerHit() override;

private:
	bool IsArrowStuckInWall(const FVector& ImpactNormal);

	void RemoveCollisionAttach(USceneComponent* Parent);

	void AttachtoEnemyHit(USceneComponent* Parent, const FName& SocketName);

	void CalculateDamage(const FName& HitBoneName);

private:
	UPROPERTY(EditAnywhere, Category = "Arrow|BodyBoneName", meta = (AllowPrivateAccess))
	TArray<FName> BodyBones;
};
