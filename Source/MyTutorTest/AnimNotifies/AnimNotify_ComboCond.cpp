// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ComboCond.h"
#include "../MyTutorTestCharacter.h"

FString UAnimNotify_ComboCond::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_ComboCond::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->SetCanCombo(false);
	}
}
