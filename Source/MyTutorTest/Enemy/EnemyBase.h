// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTutorTest/Player/MyCharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AEnemyBase : public AMyCharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

	void ApplyDamage(float NewDamge);

	UFUNCTION()
	void OnRep_CurrHP();

	UFUNCTION()
	void OnRep_MaxHP();

	

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurrHP, EditAnywhere, BlueprintReadWrite, Category = "Property")
	float CurrHP;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHP, EditAnywhere, BlueprintReadWrite, Category = "Property")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HeadTipComp;

protected:
	virtual void BeginPlay() override;


private:
	UFUNCTION(NetMulticast, Reliable)
	void DiedPhysicsEffect();

private:
	UPROPERTY()
	class UUIHeadTipBar* HeadUI;
};
