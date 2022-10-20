// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TutorAnimInstace.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UTutorAnimInstace : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTutorAnimInstace();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAsset")
	bool bArrowLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAsset")
	bool bFiringArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedAsset")
	bool bBowAiming;
};
