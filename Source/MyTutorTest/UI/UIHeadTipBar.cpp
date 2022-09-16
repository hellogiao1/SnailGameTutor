// Fill out your copyright notice in the Description page of Project Settings.


#include "UIHeadTipBar.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UUIHeadTipBar::SetHeadTipforName(const FText& TextName)
{
	Text_Name->SetText(TextName);
}

void UUIHeadTipBar::SetHeadTipforBloodPerctg(const float Percentage)
{
	ProgressBar_Blood->SetPercent(Percentage);
}
