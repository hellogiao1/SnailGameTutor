// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerProperty.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

void UUIPlayerProperty::NativeConstruct()
{
	Super::NativeConstruct();


}

void UUIPlayerProperty::SetCharacterName(const FText& NewName)
{
	if (Txt_Name)
	{
		Txt_Name->SetText(NewName);
	}
}

void UUIPlayerProperty::SetHealthPercentage(float HealthPercentage)
{
	if (ProgressBar_HP)
	{
		ProgressBar_HP->SetPercent(HealthPercentage);
	}
}

void UUIPlayerProperty::SetCharProperty(float Percentage, const FText& NewName)
{
	SetCharacterName(NewName);
	SetHealthPercentage(Percentage);
}
