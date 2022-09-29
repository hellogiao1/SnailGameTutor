// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_DetectHit.h"
#include "../MyTutorTestCharacter.h"

void UAnimNotifyState_DetectHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->SetRWeaponGeneOverlap(true);
	}
}

void UAnimNotifyState_DetectHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

void UAnimNotifyState_DetectHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->SetRWeaponGeneOverlap(false);
	}
}

FString UAnimNotifyState_DetectHit::GetNotifyName_Implementation() const
{
	return FString(TEXT("DetectHit"));
}
