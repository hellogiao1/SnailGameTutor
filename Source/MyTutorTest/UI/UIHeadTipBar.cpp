// Fill out your copyright notice in the Description page of Project Settings.


#include "UIHeadTipBar.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UUIHeadTipBar::SetHeadTipforName(const FText& TextName)
{
	if (Text_Name)
	{
		if (Text_Name->GetVisibility() != ESlateVisibility::Visible)
		{
			Text_Name->SetVisibility(ESlateVisibility::Visible);
		}

		Text_Name->SetText(TextName);
	}
}

void UUIHeadTipBar::SetHeadTipforBloodPerctg(const float Percentage)
{
	if (ProgressBar_Blood->GetVisibility() != ESlateVisibility::Visible)
	{
		ProgressBar_Blood->SetVisibility(ESlateVisibility::Visible);
	}

	ProgressBar_Blood->SetPercent(FMath::Clamp(Percentage, 0.f, 1.f));
}
