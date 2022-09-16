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
		Btn_QuestComplete->OnClicked.AddDynamic(this, &UQuestMain::OnCompleteBtn_Down);
	}

	QuestDetail = Cast<UUIQuestInfo>(GetWidgetFromName(TEXT("QuestDetail")));
	if (QuestDetail)
	{
		QuestDetail->AddQuestEvent.BindUObject(this, &UQuestMain::AddQuest);
		QuestDetail->CommitQuestEvent.BindUObject(this, &UQuestMain::CommitQuest);
	}
	SB_QuestList = Cast<UScrollBox>(GetWidgetFromName(TEXT("SB_QuestList")));
}

void UQuestMain::InteractionView()
{
	//显示可领取任务按钮
	Btn_QuestUnAccept = Cast<UButton>(GetWidgetFromName(TEXT("Btn_QuestUnAccept")));
	if (Btn_QuestUnAccept)
	{
		Btn_QuestUnAccept->SetVisibility(ESlateVisibility::Visible);
	}

	////同时隐藏其他按键
	//if (Btn_QuestAccept)
	//{
	//	Btn_QuestAccept->SetVisibility(ESlateVisibility::Hidden);
	//}
	//
	//if (Btn_QuestDoing)
	//{
	//	Btn_QuestDoing->SetVisibility(ESlateVisibility::Hidden);
	//}

	//if (Btn_QuestComplete)
	//{
	//	Btn_QuestComplete->SetVisibility(ESlateVisibility::Hidden);
	//}
}

void UQuestMain::OnUnAcceptBtn_Down()
{
	UWidget* VerticalList = GetWidgetFromName(TEXT("VB_List"));

	if (Cast<AQuestNPC>(Char))
	{
		if (VerticalList)
		{
			VerticalList->SetVisibility(ESlateVisibility::Visible);

			TArray<FQuestDetail> UnAcceptQuests;
			Cast<AQuestNPC>(Char)->GetUnAcceptQuests(UnAcceptQuests);
			FillTaskList(UnAcceptQuests);
		}
	}
}

void UQuestMain::OnAcceptBtn_Down()
{
	UWidget* VerticalList = GetWidgetFromName(TEXT("VB_List"));
	if (VerticalList)
	{
		VerticalList->SetVisibility(ESlateVisibility::Visible);
	}

	switch (ActiveWay)
	{
	case EActivateQuest::Self:
	{
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
	break;
	case EActivateQuest::Other:
	{
		TArray<FQuestDetail> AcceptUnCompletes;
		if (AQuestNPC* Npc = Cast<AQuestNPC>(Char))
		{
			Npc->GetAcceptUnFinishQuests(AcceptUnCompletes);
		}
		FillTaskList(AcceptUnCompletes);
	}
	break;
	default:
		break;
	}
}

void UQuestMain::OnDoingBtn_Down()
{

}

void UQuestMain::OnCompleteBtn_Down()
{
	UWidget* VerticalList = GetWidgetFromName(TEXT("VB_List"));
	if (VerticalList == nullptr) return;
	//显示任务列表
	VerticalList->SetVisibility(ESlateVisibility::Visible);

	switch (ActiveWay)
	{
	case EActivateQuest::Self:
	{
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
	break;
	case EActivateQuest::Other:
	{
		TArray<FQuestDetail> CompleteQuests;
		if (AQuestNPC* Npc = Cast<AQuestNPC>(Char))
		{
			Npc->GetFinishQuests(CompleteQuests);
		}
		FillTaskList(CompleteQuests);
	}
	break;
	default:
		break;
	}
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

	//TODO 未存在任务的时候提示

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

				//把button任务添加到ScrollBox中
				SB_QuestList->AddChild(QuestBtn);
			}
		}
	}
}

void UQuestMain::AddQuest(const FQuestDetail& Quest)
{
	//刷新一下任务列表，播放按钮动画
	if (SB_QuestList)
	{
		for (UWidget* It : SB_QuestList->GetAllChildren())
		{
			UUIQuestButton* CurrButton = Cast<UUIQuestButton>(It);
			if (CurrButton && CurrButton->GetUniqueID() == Quest.UniqueID)
			{
				CurrButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				CurrButton->PlayAnimFadeOut();
			}
		}
	}

	//通知角色
	if (ActiveWay == EActivateQuest::Other)
	{
		if (AQuestNPC* NPC = Cast<AQuestNPC>(Char))
		{
			NPC->CompareQuest();
		}
	}
}

void UQuestMain::CommitQuest(const FQuestDetail& Quest)
{

}

void UQuestMain::SetTargetObject(AActor* NewChar, EActivateQuest NewWay)
{
	Char = NewChar;
	ActiveWay = NewWay;
	//如果是NPC打开，则显示“可以领取任务”按钮
	if (Cast<AQuestNPC>(Char) && ActiveWay == EActivateQuest::Other)
	{
		InteractionView();
	}
}
