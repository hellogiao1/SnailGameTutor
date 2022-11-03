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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TriggerHit(const FHitResult& OutHit);

	UFUNCTION()
	virtual void OnActorDestroyed();

	virtual float ApplyDamage(AActor* HitActor, float DamageAmount);

	void TraceObject();

	UFUNCTION(NetMulticast, Unreliable)
	void MultStopMovement();

private:
	bool IsArrowStuckInWall(const FVector& ImpactNormal);

	UFUNCTION(NetMulticast, Unreliable)
	void RemoveCollisionAttach(USceneComponent* Parent);

	UFUNCTION(NetMulticast, Unreliable)
	void AttachtoEnemyHit(USceneComponent* Parent, const FName& SocketName);

	UFUNCTION()
	void OnRep_ProjectileVelocity();

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

	void SetVelocity(FVector NewVelocity);

private:
	bool bHitActor = false;

	bool bReachMaxPoint = false;

	FRotator RequireRotation;

	UPROPERTY(ReplicatedUsing = "OnRep_ProjectileVelocity")
	FVector ProjectileVelocity;
public:
	void Init(bool bMaxPath, FRotator EndRotation);
};
