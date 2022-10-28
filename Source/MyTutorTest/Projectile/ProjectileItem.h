// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileItem.generated.h"

class UProjectileMovementComponent;

UCLASS()
class MYTUTORTEST_API AProjectileItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void TriggerHit();

	UFUNCTION()
	virtual void OnActorDestroyed();

	virtual bool IsTeam(AActor* OtherActor);

	virtual float ApplyDamage(AActor* HitActor, float DamageAmount);

protected:
	FTimerHandle DestroyTimeHandle;

	UPROPERTY(EditAnywhere, Category = "Projectile|Life", meta = (ClampMin = 0.f))
	float DestroyTime;

	UPROPERTY(EditAnywhere, Category = "Projectile|UI")
	TSubclassOf<UUserWidget> HitMarkerClass;

	UPROPERTY(EditAnywhere, Category = "Projectile|Property", meta = (ClampMin = 0.f))
	float ProjectileDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
