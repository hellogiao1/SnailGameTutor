// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "MyTutorTest/UI/UIHeadTipBar.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyBase::AEnemyBase()
{
	CurrHP = 100.f;
	MaxHP = 100.f;

	HeadTipComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadInfo"));
	HeadTipComp->SetupAttachment(RootComponent);
}

void AEnemyBase::ApplyDamage(float NewDamge)
{
	if (NewDamge == 0 || CurrHP <= 0)
		return;

	CurrHP -= NewDamge;
	CurrHP = FMath::Clamp(CurrHP, 0, MaxHP);

	if (CurrHP <= 0)
	{
		DiedPhysicsEffect();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::K2_DestroyActor, 2.f);
	}
}

void AEnemyBase::OnRep_CurrHP()
{
	if (HeadUI)
	{
		HeadUI->SetHeadTipforBloodPerctg(CurrHP / MaxHP);
	}
}

void AEnemyBase::OnRep_MaxHP()
{
	if (HeadUI)
	{
		HeadUI->SetHeadTipforBloodPerctg(CurrHP / MaxHP);
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		if (HeadTipComp && HeadTipComp->GetWidget())
		{
			HeadUI = Cast<UUIHeadTipBar>(HeadTipComp->GetWidget());
			if (HeadUI)
			{
				HeadUI->SetHeadTipforBloodPerctg(CurrHP / MaxHP);
			}
		}
	}
	
}

void AEnemyBase::DiedPhysicsEffect_Implementation()
{
	if (GetMesh() == nullptr || HasAuthority())
		return;

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	UKismetSystemLibrary::PrintString(GetWorld());

}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEnemyBase, CurrHP, COND_None);
	DOREPLIFETIME_CONDITION(AEnemyBase, MaxHP, COND_None);
}
