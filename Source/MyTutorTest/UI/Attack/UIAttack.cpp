// Fill out your copyright notice in the Description page of Project Settings.


#include "UIAttack.h"
#include "../../MyTutorTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UUIAttack::NativeConstruct()
{
	if (Btn_Attack)
	{
		Btn_Attack->OnClicked.AddDynamic(this, &UUIAttack::OnAttackBtn_Down);
	}
}

void UUIAttack::OnAttackBtn_Down()
{
	AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Tutor)
	{
		Tutor->Server_AttackNotify(EAttackType::Normal);
	}
}
