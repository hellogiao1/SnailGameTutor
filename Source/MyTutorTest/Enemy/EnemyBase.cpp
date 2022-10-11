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
	//����ģ�����Ҫ�ߵĹ���
	if (GetLocalRole() != ROLE_Authority)
	{
		if (HeadUI)
		{
			HeadUI->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
		}

	}

	//�������ض��Ĺ���
	if (GetLocalRole() == ROLE_Authority)
	{

	}

	//�����л����϶�ִ�еĺ����� 
	/*
		���κ����˺������������������⹦�ܶ�Ӧ�������
	*/
	if (CurrentHealth <= 0)
	{
		OnCharcterDied();
	}
}

