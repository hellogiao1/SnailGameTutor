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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (ClampMin = 0.f))
	float DestroyTime;

protected:
	virtual void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FTimerHandle DestroyTimeHandle;
};
