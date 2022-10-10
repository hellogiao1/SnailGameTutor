// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_DetectCanCombo.h"
#include "../MyTutorTestCharacter.h"


void UAnimNotifyState_DetectCanCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

}

void UAnimNotifyState_DetectCanCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority() && Tutor->GetbComboClick() == false)
	{
		Tutor->SetPlayAttackMode(2);
		Tutor->SetbComboClick(true);
	}
}

void UAnimNotifyState_DetectCanCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority() && Tutor->GetbComboClick() == false)
	{
		Tutor->SetPlayAttackMode(-1);
		Tutor->SetbComboClick(false);
	}
}

FString UAnimNotifyState_DetectCanCombo::GetNotifyName_Implementation() const
{
	return FString(TEXT("CanCombo"));
}
