// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

AArrow::AArrow()
{
	DestroyTime = 5.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(DestroyTimeHandle, this, &AArrow::K2_DestroyActor, DestroyTime, false);
	}
}

void AArrow::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
