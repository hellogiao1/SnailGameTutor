// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "MyTutorTest/UI/UIHeadTipBar.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyBase::AEnemyBase()
{
	HeadTipComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadInfo"));
	HeadTipComp->SetupAttachment(RootComponent);
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
				HeadUI->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
			}
		}
	}

}

void AEnemyBase::OnCharcterDied()
{
	Super::OnCharcterDied();
}

void AEnemyBase::OnHealthUpdate()
{
	//所有模拟端需要走的功能
	if (GetLocalRole() != ROLE_Authority)
	{
		if (HeadUI)
		{
			HeadUI->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
		}

	}

	//服务器特定的功能
	if (GetLocalRole() == ROLE_Authority)
	{

	}

	//在所有机器上都执行的函数。 
	/*
		因任何因伤害或死亡而产生的特殊功能都应放在这里。
	*/
	if (CurrentHealth <= 0)
	{
		OnCharcterDied();
	}
}

