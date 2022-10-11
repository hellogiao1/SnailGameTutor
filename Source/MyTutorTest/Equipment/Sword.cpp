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
	//����������ʱ��ȡ����ʱ��
	GetWorld()->GetTimerManager().ClearTimer(DelayAttackHandle);
}

void ASword::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking && CanCombo)
	{
		//�ڹ���֡���ʱ�򱻴�ϻ��߽����������ӳ�1sȥ����
		GetWorld()->GetTimerManager().SetTimer(DelayAttackHandle, this, &ASword::ResetAttackMontValue, 1.f, false);
	}
	else
	{
		//�ڹ���֡ǰ��϶����Ļ���ֱ������
		ResetAttackMontValue();
	}
}
