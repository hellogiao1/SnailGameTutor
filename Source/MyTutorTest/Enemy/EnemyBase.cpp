// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "MyTutorTest/UI/UIHeadTipBar.h"
#include "Net/UnrealNetwork.h"

AEnemyBase::AEnemyBase()
{
	CurrHP = 100.f;
	MaxHP = 100.f;

	HeadTipComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadInfo"));
	HeadTipComp->SetupAttachment(RootComponent);
}

void AEnemyBase::ApplyDamage_Implementation(float NewDamge)
{
	 
}

void AEnemyBase::OnRep_CurrHP()
{

}

void AEnemyBase::OnRep_MaxHP()
{

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

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEnemyBase, CurrHP, COND_None);
	DOREPLIFETIME_CONDITION(AEnemyBase, MaxHP, COND_None);
}
