// Fill out your copyright notice in the Description page of Project Settings.


#include "UIQuestButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UUIQuestButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Quest)
	{
		Btn_Quest->OnClicked.RemoveAll(this);
		Btn_Quest->OnClicked.AddDynamic(this, &UUIQuestButton::OnQuestBtn_Down);
	}
}


void UUIQuestButton::OnQuestBtn_Down()
{
	OnQuestBtnDown.ExecuteIfBound(QuestDetail);
}

int32 UUIQuestButton::GetUniqueID() const
{
	return UniqueID;
}

void UUIQuestButton::SetUniqueID(int32 ID)
{
	UniqueID = ID;
}

const FQuestDetail& UUIQuestButton::GetQuestDetail()
{
	return QuestDetail;
}

void UUIQuestButton::SetQuestDetail(const FQuestDetail& Detail)
{
	QuestDetail = Detail;
	ResetCustomization();
}

void UUIQuestButton::SetButtonName(const FText& Name)
{
	if (Text_BtnName)
	{
		Text_BtnName->SetText(Name);
	}
}

void UUIQuestButton::PlayAnimFadeOut()
{
	if (FadeOut == nullptr) return;

	FWidgetAnimationDynamicEvent Delegate;
	Delegate.BindDynamic(this, &UUIQuestButton::RemoveFromParent);
	BindToAnimationEvent(FadeOut, Delegate, EWidgetAnimationEvent::Finished);
	PlayAnimation(FadeOut);
}

void UUIQuestButton::ResetCustomization()
{
	SetVisibility(ESlateVisibility::Visible);
	if (UVerticalBox* VB = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VB_Btn"))))
	{
		VB->SetRenderScale(FVector2D(1.f, 1.f));
	}
}
