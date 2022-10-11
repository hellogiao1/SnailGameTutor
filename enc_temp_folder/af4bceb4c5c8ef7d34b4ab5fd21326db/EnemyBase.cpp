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

void AEnemyBase::Destroyed()
{
	if (GetMesh() == nullptr || HasAuthority())
		return;

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

}

void AEnemyBase::OnRep_CurrentHealth()
{
	UKismetSystemLibrary::PrintString(GetWorld());
	OnHealthUpdate();
}

void AEnemyBase::OnHealthUpdate()
{
	//�ͻ����ض��Ĺ���
	if (IsLocallyControlled())
	{
		if (HeadUI)
		{
			HeadUI->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
		}
	}

	//�������ض��Ĺ���
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			Destroy();
		}
	}

	//�����л����϶�ִ�еĺ����� 
	/*
		���κ����˺������������������⹦�ܶ�Ӧ�������
	*/
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
