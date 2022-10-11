// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_DetectHit.h"
#include "../MyTutorTestCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Enemy/EnemyBase.h"

void UAnimNotifyState_DetectHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

 	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
 	if (Tutor && Tutor->HasAuthority())
 	{
		ActorsToIgnore.Reset();
		ActorsToIgnore.Add(Tutor);
	}
}

void UAnimNotifyState_DetectHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	//return;
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		TArray<class AActor*> OutActors;
		UKismetSystemLibrary::SphereOverlapActors(Tutor, Tutor->GetActorLocation(), SphereRadius, { EObjectTypeQuery::ObjectTypeQuery3 }, nullptr, ActorsToIgnore, OutActors);

		switch (AttackArea)
		{
		case EAttackArea::Sector:
		{
			for (AActor* It : OutActors)
			{
				bool InSightCone = CheckIsTargetInSightCone(Angle, Tutor->GetActorForwardVector(), Tutor->GetActorLocation(), It->GetActorLocation());
				if (InSightCone)
				{
					It->TakeDamage(20.f, FDamageEvent(), Tutor->GetController(), Tutor);
					ActorsToIgnore.Add(It);
				}
			}
		}
		break;
		case EAttackArea::Circle:
		{
			for (AActor* It : OutActors)
			{
				It->TakeDamage(20.f, FDamageEvent(), Tutor->GetController(), Tutor);
				ActorsToIgnore.Add(It);
			}
		}
		break;
		default:
			break;
		}
		
	}

	
}

void UAnimNotifyState_DetectHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(MeshComp->GetOwner());
	if (Tutor && Tutor->HasAuthority())
	{
		Tutor->SetRWeaponFinishFrame(true);
	}
}

bool UAnimNotifyState_DetectHit::CheckIsTargetInSightCone(float AngleDegrees, FVector AxisLocation, FVector StartLocation, FVector TargetLocation)
{
	const FVector ActorToTarget = TargetLocation - StartLocation;
	float CosRadians = FMath::Cos(FMath::Clamp(FMath::DegreesToRadians(AngleDegrees), 0.f, PI));
	return FVector::DotProduct(ActorToTarget.GetUnsafeNormal(), AxisLocation) > CosRadians;
}

FString UAnimNotifyState_DetectHit::GetNotifyName_Implementation() const
{
	return FString(TEXT("DetectHit"));
}
