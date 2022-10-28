﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectileItem.h"
#include "../MyTutorTestCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AArrow::AArrow()
{
	DestroyTime = 5.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

}

void AArrow::TriggerHit()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(DestroyTimeHandle, this, &AArrow::OnActorDestroyed, DestroyTime, false);
	}

	FVector StartLoc = GetActorLocation();
	FVector EndLoc = StartLoc + GetActorForwardVector() * 40.f;
	FHitResult OutHit;

	//自己添加的ObjectChannel对应着的 ETraceTypeQuery 是从数字7开始的
	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {}, EDrawDebugTrace::None, OutHit, true);
	if (bHit)
	{
		if (OutHit.BoneName == TEXT("None"))
		{
			/*if (IsArrowStuckInWall(OutHit.ImpactNormal))
			{
				
			}*/
			RemoveCollisionAttach(OutHit.GetComponent());
		}
		else
		{
			AttachtoEnemyHit(OutHit.GetComponent(), OutHit.BoneName);
			
			if (HasAuthority())
			{
				CalculateDamage(OutHit.BoneName);
				AActor* HitActor = OutHit.GetHitObjectHandle().FetchActor();
				if (IsTeam(HitActor) == false)
				{
					ApplyDamage(HitActor, ProjectileDamage);
				}
			}
		}
	}
	
}

bool AArrow::IsArrowStuckInWall(const FVector& ImpactNormal)
{
	float DP = FVector::DotProduct(ImpactNormal, GetActorForwardVector());
	float AngleWall = (180.0) / DOUBLE_PI * FMath::Acos(DP) - 90.f;
	if (AngleWall >= 8.f)
	{
		if (ProjectileMove)
		{
			ProjectileMove->StopMovementImmediately();
		}
		return true;
	}
	else
	{
		return false;
	}
}

void AArrow::RemoveCollisionAttach(USceneComponent* Parent)
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
}

void AArrow::AttachtoEnemyHit(USceneComponent* Parent, const FName& SocketName)
{
	if (ProjectileMove)
	{
		ProjectileMove->StopMovementImmediately();
	}

	StaticMesh->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform, SocketName);

	//生成HitMarker动画
	AMyTutorTestCharacter* CurrChar = Cast<AMyTutorTestCharacter>(GetOwner());
	if (CurrChar && CurrChar->GetLocalRole() == ROLE_AutonomousProxy)
	{
		if (HitMarkerClass != nullptr)
		{
			UUserWidget* HitMarker = CreateWidget<UUserWidget>(GetWorld(), HitMarkerClass);
			if (HitMarker)
			{
				HitMarker->AddToViewport();
			}
		}
	}
}

void AArrow::CalculateDamage(const FName& HitBoneName)
{
	float DamageMultiplier = 1.f;
	if (HitBoneName == TEXT("head"))
	{
		DamageMultiplier = 3.f;
	}
	else
	{
		//If Not Head, Set Damage (x0.75)
		DamageMultiplier = 0.75f;
		
		//If Arrow Hits Chest, Change (x0.75) over to  (x1)
		for (const auto& BoneName : BodyBones)
		{
			if (HitBoneName == BoneName)
			{
				DamageMultiplier = 1.f;
			}
		}
	}

	//Set Damage
	ProjectileDamage *= DamageMultiplier;
}
