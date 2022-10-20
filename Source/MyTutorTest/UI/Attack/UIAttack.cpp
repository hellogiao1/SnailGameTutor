// Fill out your copyright notice in the Description page of Project Settings.


#include "UIAttack.h"
#include "../../MyTutorTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UUIAttack::NativeConstruct()
{
	if (Btn_Attack)
	{
		Btn_Attack->OnPressed.AddDynamic(this, &UUIAttack::OnAttackBtn_Down);
		Btn_Attack->OnReleased.AddDynamic(this, &UUIAttack::OnAttackBtn_Release);
	}
}

void UUIAttack::OnAttackBtn_Down()
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Tutor)
	{
		Tutor->AttackBtn_Down(EAttackType::Normal);
	}
}

void UUIAttack::OnAttackBtn_Release()
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Tutor)
	{
		Tutor->AttackBtn_Release(EAttackType::Normal);
	}
}
