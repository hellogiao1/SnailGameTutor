// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipObject.generated.h"

UCLASS()
class MYTUTORTEST_API AEquipObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AEquipObject();

	virtual void Tick(float DeltaTime) override;

	void SetGenerateOverlap(bool bInGeneOverlap);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* Scene;

protected:
	virtual void BeginPlay() override;

	//碰撞事件，空实现，在子类重写
	UFUNCTION()
	virtual void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
