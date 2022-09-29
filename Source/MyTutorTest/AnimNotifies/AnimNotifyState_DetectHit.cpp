// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_DetectHit.h"
#include "../MyTutorTestCharacter.h"

void UAnimNotifyState_DetectHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	
}

void UAnimNotifyState_DetectHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->SetRWeaponGeneOverlap(true);
	}

	
}

void UAnimNotifyState_DetectHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	/*AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->SetRWeaponGeneOverlap(false);
	}*/
}

bool UAnimNotifyState_DetectHit::CheckIsTargetInSightCone(float AngleDegrees, FVector AxisLocation, FVector StartLocation, FVector TargetLocation)
{
	const FVector ActorToTarget = TargetLocation - StartLocation;
	float CosRadians = FMath::Cos(FMath::Clamp(AngleDegrees, 0.f, PI));	
	return FVector::DotProduct(ActorToTarget.GetUnsafeNormal(), AxisLocation) > CosRadians;
}

FString UAnimNotifyState_DetectHit::GetNotifyName_Implementation() const
{
	return FString(TEXT("DetectHit"));
}
