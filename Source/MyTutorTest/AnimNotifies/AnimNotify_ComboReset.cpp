// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ComboReset.h"
#include "../MyTutorTestCharacter.h"

FString UAnimNotify_ComboReset::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_ComboReset::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->OnAttackMontEnd_CallBack();
	}
}
