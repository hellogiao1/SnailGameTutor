// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DetectHit.h"
#include "../MyTutorTestCharacter.h"

UAnimNotify_DetectHit::UAnimNotify_DetectHit()
	: Super()
{

}

FString UAnimNotify_DetectHit::GetNotifyName_Implementation() const
{
	return FString(TEXT("DetectHit"));
}

void UAnimNotify_DetectHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor)
	{
		Tutor->SetRWeaponGeneOverlap(bOpenDetect);
	}
}
