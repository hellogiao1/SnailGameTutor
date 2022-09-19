// Fill out your copyright notice in the Description page of Project Settings.


#include "UIOnProgressTipBar.h"
#include "Components/VerticalBox.h"
#include "MytutorTest/MyTutorTestCharacter.h"
#include "MyTutorTest/Components/QuestComponent.h"
#include "MyTutorTest/Data/QuestData.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UUIOnProgressTipBar::InitQuestDetail(const FQuestDetail& Quest)
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
		UTextBlock* Text_Objective = NewObject<UTextBlock>(this, UTextBlock::StaticClass());;
		//UTextBlock* Text_Progress = NewObject<UTextBlock>(this, UTextBlock::StaticClass());;

		switch (Objective.QuestTarget)
		{
		case EQuestTarget::PickUpItem:
		{
			FString ObjectName(TEXT("\"未填写\""));
			if (Objective.Item)
			{
				Objective.Item->GetName(ObjectName);
			}

			ObjectiveStr += FString::Printf(TEXT("拾取 %d 个 %s 物品  %d / %%d"), Objective.Count, *ObjectName, Objective.CurrCount, Objective.Count);
			//ProgressStr += FString::Printf(TEXT("目前已经拾取 %d 个 %s 物品"), Objective.CurrCount, *ObjectName);

			break;
		}
		case EQuestTarget::Kill:
		{
			FString ObjectName(TEXT("\"未填写\""));
			if (Objective.NPC)
			{
				Objective.NPC->GetName(ObjectName);
			}

			ObjectiveStr += FString::Printf(TEXT("击杀%d个 %s NPC  %d / %%d"), Objective.Count, *ObjectName, Objective.CurrCount, Objective.Count);
			//ProgressStr += FString::Printf(TEXT("目前已经击杀%d个 %s NPC"), Objective.CurrCount, *ObjectName);

			break;
		}
		case EQuestTarget::GoToArea:
		{
			FString PosStr = Objective.TargetPosition.ToString();
			FString Progress = Objective.bReach ? TEXT("已经走到指定坐标点") : TEXT("未走到指定坐标点");
			ObjectiveStr += FString::Printf(TEXT("走到指定坐标点%s   %s"), *PosStr, *Progress);
			//ProgressStr += Objective.bReach ? TEXT("已经走到指定坐标点") : TEXT("未走到指定坐标点");

			break;
		}
		default:
			break;
		}
		
		Text_Objective->SetText(FText::FromString(ObjectiveStr));
		
		VB_OnProgress->AddChild(Text_Objective);
	}
}
