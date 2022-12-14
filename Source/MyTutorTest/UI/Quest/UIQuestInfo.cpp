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
	MyQuest = &QuestDetail;

	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	//任务名
	Text_Name->SetText(FText::FromString(QuestDetail.QuestName));

	//根据枚举更新 任务目标和任务进度
	ObjectiveAndProgress(QuestDetail);

	/** 任务领取条件 */
	QuestCondition(QuestDetail);
	
}

void UUIQuestInfo::ObjectiveAndProgress(const FQuestDetail& Quest)
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	bool bShowFinishBtn = true;

	FString ObjectiveStr; //= TEXT("任务目标：");
	FString ProgressStr; //= TEXT("任务进度：");
	for (const auto& Objective : Quest.Objectives)
	{
		switch (Objective.QuestTarget)
		{
		case EQuestTarget::PickUpItem:
			{
				FString ObjectName(TEXT("\"未填写\""));
				if (Objective.Item)
				{
					Objective.Item->GetName(ObjectName);
				}

				ObjectiveStr += FString::Printf(TEXT("拾取 %d 个 %s 物品"), Objective.Count, *ObjectName);
				ProgressStr += FString::Printf(TEXT("目前已经拾取 %d 个 %s 物品"), Objective.CurrCount, *ObjectName);

				if (Objective.CurrCount < Objective.Count)
				{
					bShowFinishBtn = false;
				}

				break;
			}
		case EQuestTarget::Kill:
			{
				FString ObjectName(TEXT("\"未填写\""));
				if (Objective.NPC)
				{
					Objective.NPC->GetName(ObjectName);
				}

				ObjectiveStr += FString::Printf(TEXT("击杀%d个 %s NPC"), Objective.Count, *ObjectName);
				ProgressStr += FString::Printf(TEXT("目前已经击杀%d个 %s NPC"), Objective.CurrCount, *ObjectName);

				if (Objective.CurrCount < Objective.Count)
				{
					bShowFinishBtn = false;
				}

				break;
			}
		case EQuestTarget::GoToArea:
			{
				FString PosStr = Objective.TargetPosition.ToString();
				ObjectiveStr += FString::Printf(TEXT("走到指定坐标点%s"), *PosStr);
				ProgressStr += Objective.bReach ? TEXT("已经走到指定坐标点") : TEXT("未走到指定坐标点");

				if (Objective.bReach == false)
				{
					bShowFinishBtn = false;
				}

				break;
			}
		default:
			break;
		}
		ObjectiveStr += TEXT("\n");
		ProgressStr += TEXT("\n");
	}
	Text_Objective->SetText(FText::FromString(ObjectiveStr));
	Text_Progress->SetText(FText::FromString(ProgressStr));
	
	if (QuestComp->ExistQuest(Quest.UniqueID))
	{
		//达到任务要求，显示完成任务按钮
		if (bShowFinishBtn)
		{
			if (Quest.bIsComplete == false)
			{
				Btn_Complete->SetVisibility(ESlateVisibility::Visible);
				Btn_Complete->OnClicked.AddDynamic(this, &UUIQuestInfo::OnCompleteBtn_Down);
			}
		}
		else
		{
			if (Quest.bIsProgress)
			{
				Btn_StopProgress->SetVisibility(ESlateVisibility::Visible);
				Btn_StopProgress->OnClicked.AddDynamic(this, &UUIQuestInfo::OnStopProgressBtn_Down);
			}
			else
			{
				Btn_OnProgress->SetVisibility(ESlateVisibility::Visible);
				Btn_OnProgress->OnClicked.AddDynamic(this, &UUIQuestInfo::OnProgressBtn_Down);
			}
		}
		
	}

	//任务奖励
	Text_Reward->SetText(FText::FromString(TEXT("任务奖励：") + Quest.QuestReward));
}

void UUIQuestInfo::QuestCondition(const FQuestDetail& Quest)
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	//是否显示领取按钮
	bool bShowAcceptBtn = true;
	int8 Num = 1;

	for (const FQuestAcceptCond& Cond : Quest.QuestAcceptConds)
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
				Text_AcceptCond->SetColorAndOpacity(TempQuest->bIsComplete ? FColor::Green : FColor::Red);
				Text_AcceptCond->SetText(FText::FromString(CondInfo));

				if (TempQuest->bIsComplete == false)
				{
					bShowAcceptBtn = false;
				}
			}
			else if (TempQuest == nullptr)
			{
				CondInfo += TEXT("未查到任务，请确认任务ID是否输入正确");

				Text_AcceptCond->SetText(FText::FromString(CondInfo));
				bShowAcceptBtn = false;
			}
			
			break;
		}
		case EQuestRecCond::ItemOwn:
		{
			FString ItemName;
			if (Cond.Item)
			{
				Cond.Item->GetName(ItemName);
				Text_AcceptCond->SetColorAndOpacity(MyCharacter->ExistItem(Cond.Item->GetClass()) ? FColor::Green : FColor::Red);
				bShowAcceptBtn = MyCharacter->ExistItem(Cond.Item->GetClass());
			}
			CondInfo += FString::Printf(TEXT("需要先获取“ %s ”物品"), *ItemName);
			
			Text_AcceptCond->SetText(FText::FromString(CondInfo));
			break;
		}
		default:
			break;
		}

		ScrollBox_Cond->AddChild(Text_AcceptCond);
	}

	//当任务未领取，且符合领取条件则显示领取按钮
	if ((QuestComp->ExistQuest(Quest.UniqueID) == false) && bShowAcceptBtn)
	{
		Btn_Accept->SetVisibility(ESlateVisibility::Visible);
		Btn_Accept->OnClicked.AddDynamic(this, &UUIQuestInfo::OnAcceptBtn_Down);
	}
}

void UUIQuestInfo::OnAcceptBtn_Down()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr)
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr)
		return;

	if (MyQuest == nullptr) return;

	if (QuestComp->ExistQuest(MyQuest->UniqueID) == false)
	{
		QuestComp->AddQuest(*MyQuest);
		//任务名后添加"已领取"标识
		Text_Name->SetText(FText::FromString(MyQuest->QuestName + TEXT("(已领取)")));
		Btn_Accept->SetVisibility(ESlateVisibility::Hidden);

		//通知事件
		AddQuestEvent.ExecuteIfBound(*MyQuest);
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

	if (MyQuest == nullptr) return;

	if (QuestComp->ExistQuest(MyQuest->UniqueID))
	{
		QuestComp->GetQuestForID(MyQuest->UniqueID)->bIsComplete = true;
		Btn_Complete->SetVisibility(ESlateVisibility::Hidden);
		CommitQuestEvent.ExecuteIfBound(*MyQuest);
	}
}

void UUIQuestInfo::OnProgressBtn_Down()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr)
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr)
		return;

	if (MyQuest == nullptr) return;

	if (QuestComp->ExistQuest(MyQuest->UniqueID))
	{
		QuestComp->GetQuestForID(MyQuest->UniqueID)->bIsProgress = true;
		Btn_OnProgress->SetVisibility(ESlateVisibility::Hidden);
		OnProgressEvent.ExecuteIfBound(*MyQuest);
	}
}

void UUIQuestInfo::OnStopProgressBtn_Down()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr)
		return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr)
		return;

	if (MyQuest == nullptr) return;

	if (QuestComp->ExistQuest(MyQuest->UniqueID))
	{
		QuestComp->GetQuestForID(MyQuest->UniqueID)->bIsProgress = false;
		Btn_StopProgress->SetVisibility(ESlateVisibility::Hidden);
		StopProgressEvent.ExecuteIfBound(*MyQuest);
	}
}

void UUIQuestInfo::ResetDetail()
{
	ScrollBox_Cond->ClearChildren();

	Btn_Accept->SetVisibility(ESlateVisibility::Hidden);
	Btn_Complete->SetVisibility(ESlateVisibility::Hidden);
	Btn_OnProgress->SetVisibility(ESlateVisibility::Hidden);
	Btn_StopProgress->SetVisibility(ESlateVisibility::Hidden);

	Btn_Complete->OnClicked.RemoveAll(this);
	Btn_Accept->OnClicked.RemoveAll(this);
	Btn_OnProgress->OnClicked.RemoveAll(this);
	Btn_StopProgress->OnClicked.RemoveAll(this);
}
