// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestMain.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "UIQuestButton.h"
#include "UIQuestInfo.h"
#include "MyTutorTest/Data/QuestData.h"
#include "Kismet/GameplayStatics.h"
#include "MyTutorTest/MyTutorTestCharacter.h"
#include "MyTutorTest/Components/QuestComponent.h"
#include "MytutorTest/Player/QuestNPC.h"

void UQuestMain::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_QuestUnAccept = Cast<UButton>(GetWidgetFromName(TEXT("Btn_QuestUnAccept")));
	if (Btn_QuestUnAccept)
	{
		Btn_QuestUnAccept->OnClicked.AddDynamic(this, &UQuestMain::OnUnAcceptBtn_Down);
	}

	Btn_QuestAccept = Cast<UButton>(GetWidgetFromName(TEXT("Btn_QuestAccept")));
	if (Btn_QuestAccept)
	{
		Btn_QuestAccept->OnClicked.AddDynamic(this, &UQuestMain::OnAcceptBtn_Down);
	}

	Btn_QuestDoing = Cast<UButton>(GetWidgetFromName(TEXT("Btn_QuestDoing")));
	if (Btn_QuestDoing)
	{
		Btn_QuestDoing->OnClicked.AddDynamic(this, &UQuestMain::OnDoingBtn_Down);
	}

	Btn_QuestComplete = Cast<UButton>(GetWidgetFromName(TEXT("Btn_QuestComplete")));
	if (Btn_QuestComplete)
	{
		Btn_QuestComplete->OnClicked.AddDynamic(this, &UQuestMain::OnUnCompleteBtn_Down);
	}

	QuestDetail = Cast<UUIQuestInfo>(GetWidgetFromName(TEXT("QuestDetail")));
	SB_QuestList = Cast<UScrollBox>(GetWidgetFromName(TEXT("SB_QuestList")));
}

void UQuestMain::InteractionView()
{
	//��ʾ����ȡ����ť
	Btn_QuestUnAccept = Cast<UButton>(GetWidgetFromName(TEXT("Btn_QuestUnAccept")));
	if (Btn_QuestUnAccept)
	{
		Btn_QuestUnAccept->SetVisibility(ESlateVisibility::Visible);
	}
	
	//ͬʱ������������
	if (Btn_QuestAccept)
	{
		Btn_QuestAccept->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (Btn_QuestDoing)
	{
		Btn_QuestDoing->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Btn_QuestComplete)
	{
		Btn_QuestComplete->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuestMain::OnUnAcceptBtn_Down()
{
	UWidget* VerticalList = GetWidgetFromName(TEXT("VB_List"));
	
	if (Cast<AQuestNPC>(Char))
	{
		if (VerticalList)
		{
			VerticalList->SetVisibility(ESlateVisibility::Visible);

			if (QuestDetail)
			{
				QuestDetail->IsActiveOpen(false);
			}

			FillTaskList(Cast<AQuestNPC>(Char)->GetUnAcceptQuest());
		}
	}
}

void UQuestMain::OnAcceptBtn_Down()
{
	UWidget* VerticalList = GetWidgetFromName(TEXT("VB_List"));
	if (VerticalList == nullptr) return;
	//��ʾ�����б�
	VerticalList->SetVisibility(ESlateVisibility::Visible);
	if (QuestDetail)
	{
		QuestDetail->IsActiveOpen(true);
	}

	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(Char);
	if (MyCharacter == nullptr)
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) 
		return;

	TArray<FQuestDetail> AcceptUnCompletes;
	QuestComp->FindQuestForNoComplete(AcceptUnCompletes);
	FillTaskList(AcceptUnCompletes);
}

void UQuestMain::OnDoingBtn_Down()
{

}

void UQuestMain::OnUnCompleteBtn_Down()
{
	UWidget* VerticalList = GetWidgetFromName(TEXT("VB_List"));
	if (VerticalList == nullptr) return;
	//��ʾ�����б�
	VerticalList->SetVisibility(ESlateVisibility::Visible);
	if (QuestDetail)
	{
		QuestDetail->IsActiveOpen(true);
	}

	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(Char);
	if (MyCharacter == nullptr)
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr)
		return;

	TArray<FQuestDetail> CompleteQuests;
	QuestComp->FindQuestForComplete(CompleteQuests);
	FillTaskList(CompleteQuests);
}

void UQuestMain::FlushQuestDetail(const FQuestDetail& Quest)
{
	QuestDetail->ShowQuestDetail(Quest);
}

void UQuestMain::FillTaskList(const TArray<FQuestDetail>& Quests)
{
	if (SB_QuestList)
	{
		SB_QuestList->ClearChildren();
	}

	if (QuestDetail)
	{
		QuestDetail->SetVisibility(ESlateVisibility::Hidden);
	}

	//TODO δ���������ʱ����ʾ

	for (const auto& quest : Quests)
	{
		if (BPQuestBtnClass)
		{
			UUIQuestButton* QuestBtn = CreateWidget<UUIQuestButton>(GetWorld(), BPQuestBtnClass);
			if (QuestBtn)
			{
				QuestBtn->SetUniqueID(quest.UniqueID);
				QuestBtn->SetButtonName(FText::FromString(quest.QuestName));
				QuestBtn->SetQuestDetail(quest);
				QuestBtn->OnQuestBtnDown.AddUObject(this, &UQuestMain::FlushQuestDetail);

				//��button������ӵ�ScrollBox��
				SB_QuestList->AddChild(QuestBtn);
			}
		}
	}
}

void UQuestMain::SetTargetObject(AActor* NewChar)
{
	Char = NewChar;
}
