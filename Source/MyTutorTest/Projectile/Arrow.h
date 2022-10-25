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

	UPROPERTY(EditAnywhere, Category = "Arrow|UI")
	TSubclassOf<UUserWidget> HitMarkerClass;

	UPROPERTY(EditAnywhere, Category = "Arrow|Property", meta = (ClampMin = 0.f))
	float ArrowDamage;

protected:
	virtual void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	void TriggerHit();

	bool IsArrowStuckInWall(const FVector& ImpactNormal);

	void RemoveCollisionAttach(USceneComponent* Parent);

	void AttachtoEnemyHit(USceneComponent* Parent, const FName& SocketName);

	void CalculateDamage(const FName& HitBoneName);

	bool IsTeam(AActor* OtherActor);

	float ApplyDamage(AActor* HitActor);

private:
	FTimerHandle DestroyTimeHandle;

	UPROPERTY(EditAnywhere, Category = "Arrow|BodyBoneName", meta = (AllowPrivateAccess))
	TArray<FName> BodyBones;
};
