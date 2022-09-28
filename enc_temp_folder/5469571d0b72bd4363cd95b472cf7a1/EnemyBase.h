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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float CurrHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HeadTipComp;

protected:
	virtual void BeginPlay() override;

private:
	class UUIHeadTipBar* HeadUI;
};
