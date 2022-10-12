// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_DetectCanCombo.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UAnimNotifyState_DetectCanCombo : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	bool CheckIsTargetInSightCone(float AngleDegrees, FVector AxisLocation, FVector StartLocation, FVector TargetLocation);

	// Overridden from UAnimNotifyState to provide custom notify name.
	FString GetNotifyName_Implementation() const override;

public:
	//If false, the CanCombo variable is not reset
	UPROPERTY(EditAnywhere)
	bool bReset = true;
};
