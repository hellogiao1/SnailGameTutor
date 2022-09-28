// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipObject.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API ASword : public AEquipObject
{
	GENERATED_BODY()

public:
	ASword();

	virtual void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
							const FHitResult& SweepResult) override;
};
