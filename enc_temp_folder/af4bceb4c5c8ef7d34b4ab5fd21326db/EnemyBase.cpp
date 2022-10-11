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
	//客户端特定的功能
	if (IsLocallyControlled())
	{
		if (HeadUI)
		{
			HeadUI->SetHeadTipforBloodPerctg(CurrentHealth / MaxHealth);
		}
	}

	//服务器特定的功能
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			Destroy();
		}
	}

	//在所有机器上都执行的函数。 
	/*
		因任何因伤害或死亡而产生的特殊功能都应放在这里。
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
