// Fill out your copyright notice in the Description page of Project Settings.


#include "UIAcceptQuest.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UUIAcceptQuest::NativeConstruct()
{
	if (Btn_Interaction)
	{
		Btn_Interaction->OnClicked.AddDynamic(this, &UUIAcceptQuest::OnInterBtn_Down);
	}

	if (Btn_NPCName)
	{
		Btn_NPCName->OnClicked.AddDynamic(this, &UUIAcceptQuest::OnNPCNameBtn_Down);
	}
}

void UUIAcceptQuest::OnInterBtn_Down()
{
	/*if (WidgetSwitcher_74) {
		WidgetSwitcher_74->SetActiveWidgetIndex(0);
	}*/

	RemoveFromParent();
}

void UUIAcceptQuest::OnNPCNameBtn_Down()
{
	if (WidgetSwitcher_74) {
		WidgetSwitcher_74->SetActiveWidgetIndex(1);
	}
}

void UUIAcceptQuest::SetWidgetName(const FText& NewName)
{
	MyName = NewName;

	if (Text_Name)
	{
		Text_Name->SetText(MyName);
	}
}

const FText& UUIAcceptQuest::GetWidgetName()
{
	return MyName;
}
