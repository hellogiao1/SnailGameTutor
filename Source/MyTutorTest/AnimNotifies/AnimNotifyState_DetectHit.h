// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_DetectHit.generated.h"

UENUM()
enum class EAttackArea : uint8
{
	Sector,
	Circle,
};

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "DetectHit"))
class MYTUTORTEST_API UAnimNotifyState_DetectHit : public UAnimNotifyState
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
	UPROPERTY(EditAnywhere)
	EAttackArea AttackArea = EAttackArea::Circle;

	UPROPERTY(EditAnywhere)
	float SphereRadius = 200.f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "AttackArea == EAttackArea::Sector", EditConditionHides))
	float Angle = 45.f;

private:
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
};
