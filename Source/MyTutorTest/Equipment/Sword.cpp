// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Components/CapsuleComponent.h"
#include "../Enemy/EnemyBase.h"

ASword::ASword()
{
	AttackValue = 20.f;
}

void ASword::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor))
	{
		
	}
}

void ASword::NormalCombo()
{
	//可以连击的时候，取消计时器
	GetWorld()->GetTimerManager().ClearTimer(DelayAttackHandle);
}

void ASword::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking && CanCombo)
	{
		//在攻击帧后的时候被打断或者结束动画，延迟1s去重置
		GetWorld()->GetTimerManager().SetTimer(DelayAttackHandle, this, &ASword::ResetAttackMontValue, 1.f, false);
	}
	else
	{
		//在攻击帧前打断动画的话，直接重置
		ResetAttackMontValue();
	}
}
