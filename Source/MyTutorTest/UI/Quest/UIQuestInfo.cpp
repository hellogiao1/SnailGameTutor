// Fill out your copyright notice in the Description page of Project Settings.


#include "UIQuestInfo.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "MyTutorTest/Data/QuestData.h"
#include "Kismet/GameplayStatics.h"
#include "MyTutorTest/MyTutorTestCharacter.h"
#include "MyTutorTest/Components/QuestComponent.h"
#include "MyTutorTest/Player/QuestNPC.h"
#include "Blueprint/UserWidget.h"


void UUIQuestInfo::NativeConstruct()
{
	Super::NativeConstruct();


}

void UUIQuestInfo::ShowQuestDetail(const FQuestDetail& QuestDetail)
{
	ResetDetail();
	SetVisibility(ESlateVisibility::Visible);
	MyQuest = QuestDetail;

	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	//任务名
	Text_Name->SetText(FText::FromString(QuestDetail.QuestName));

	//根据枚举更新 任务目标和任务进度
	FString ObjectiveStr = TEXT("任务目标：");
	switch (QuestDetail.Objective.QuestTarget)
	{
	case EQuestTarget::PickUpItem:
	{
		FString ObjectName(TEXT("\"未填写\""));
		if (QuestDetail.Objective.Item)
		{
			QuestDetail.Objective.Item->GetName(ObjectName);
		}
		ObjectiveStr += FString::Printf(TEXT("拾取 %d 个 %s 物品"), QuestDetail.Objective.Count, *ObjectName);
		break;
	}
	case EQuestTarget::Kill:
	{
		FString ObjectName(TEXT("\"未填写\""));
		if (QuestDetail.Objective.NPC)
		{
			QuestDetail.Objective.NPC->GetName(ObjectName);
		}

		ObjectiveStr += FString::Printf(TEXT("击杀%d个 %s NPC"), QuestDetail.Objective.Count, *ObjectName);
		break;
	}
	case EQuestTarget::GoToArea:
	{
		FString PosStr = QuestDetail.Objective.TargetPosition.ToString();
		ObjectiveStr += FString::Printf(TEXT("走到指定坐标点%s"), *PosStr);
		break;
	}
	default:
		break;
	}
	Text_Objective->SetText(FText::FromString(ObjectiveStr));

	//任务奖励
	Text_Reward->SetText(FText::FromString(TEXT("任务奖励：") + QuestDetail.QuestReward));

//////////////////////////////////////////////////////////////////////////
	/** 任务领取条件 */

	//是否显示领取按钮
	bool bShowAcceptBtn = true;
	int8 Num = 1;

	for (const FQuestAcceptCond& Cond : QuestDetail.QuestAcceptConds)
	{
		FString CondInfo(FString::FromInt(Num++) + TEXT("、"));
		UTextBlock* Text_AcceptCond = NewObject<UTextBlock>(this, UTextBlock::StaticClass());;
		//UTextBlock* Text_AcceptCond = CreateWidget<UTextBlock>(GetWorld(), TestClass);
		//WidgetTree::ConstructWidget()

		switch (Cond.QuestCond)
		{
		case EQuestRecCond::LevelRequired:
		{
			CondInfo += FString::Printf(TEXT("玩家等级需要 %d 级"), Cond.Level);
			Text_AcceptCond->SetText(FText::FromString(CondInfo));
			Text_AcceptCond->SetColorAndOpacity(MyCharacter->GetLevel() >= Cond.Level ? FColor::Green : FColor::Red);
			if (MyCharacter->GetLevel() < Cond.Level)
			{
				bShowAcceptBtn = false;
			}
			break;
		}
		case EQuestRecCond::PreQuest:
		{
			if (Cond.PreQuestID == -1) break;

			const FQuestDetail* TempQuest = nullptr;
			if (QuestComp->ExistQuest(Cond.PreQuestID))
			{
				//从角色身上查找
				TempQuest = QuestComp->GetQuestForID(Cond.PreQuestID);
			}
			else if (Cast<AQuestNPC>(MyCharacter->GetNPCPtr()))
			{
				//从NPC任务中查找
				auto& QuestArray = Cast<AQuestNPC>(MyCharacter->GetNPCPtr())->NPCQuests;
				for (const FQuestDetail& quest : QuestArray)
				{
					if (quest.UniqueID == Cond.PreQuestID)
					{
						TempQuest = &quest;
						break;
					}
				}
			}
			if (TempQuest)
			{
				CondInfo += FString::Printf(TEXT("需要先完成“ %s ”任务"), *(TempQuest->QuestName));
			}
			Text_AcceptCond->SetText(FText::FromString(CondInfo));
			Text_AcceptCond->SetColorAndOpacity(TempQuest->bIsComplete ? FColor::Green : FColor::Red);
			if (TempQuest->bIsComplete == false)
			{
				bShowAcceptBtn = false;
			}
			break;
		}
		case EQuestRecCond::ItemOwn:
		{
			FString ItemName;
			Cond.Item->GetName(ItemName);
			CondInfo += FString::Printf(TEXT("需要先获取“ %s ”物品"), *ItemName);
			break;
		}
		default:
			break;
		}

		ScrollBox_Cond->AddChild(Text_AcceptCond);
	}

	if (!bIsActiveOpen && bShowAcceptBtn)
	{
		Btn_Complete->SetVisibility(ESlateVisibility::Hidden);

		Btn_Accept->SetVisibility(ESlateVisibility::Visible);
		Btn_Accept->OnClicked.AddDynamic(this, &UUIQuestInfo::OnAcceptBtn_Down);
	}
}

void UUIQuestInfo::ResetDetail()
{
	ScrollBox_Cond->ClearChildren();
	Btn_Complete->OnClicked.RemoveAll(this);
	Btn_Accept->OnClicked.RemoveAll(this);
}

void UUIQuestInfo::OnAcceptBtn_Down()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) 
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) 
		return;

	if (QuestComp->ExistQuest(MyQuest.UniqueID) == false)
	{
		QuestComp->AddQuest(MyQuest);
		//任务名后添加已领取
		Text_Name->SetText(FText::FromString(MyQuest.QuestName + TEXT("(已领取)")));
		Btn_Accept->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUIQuestInfo::OnCompleteBtn_Down()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) 
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) 
		return;

	if (QuestComp->ExistQuest(MyQuest.UniqueID))
	{
		QuestComp->GetQuestForID(MyQuest.UniqueID)->bIsComplete = true;

		Btn_Complete->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUIQuestInfo::IsActiveOpen(bool Active)
{
	bIsActiveOpen = Active;
}
