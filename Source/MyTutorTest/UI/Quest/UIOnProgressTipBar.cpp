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

	FString ObjectiveStr; //= TEXT("����Ŀ�꣺");
	FString ProgressStr; //= TEXT("������ȣ�");
	for (const auto& Objective : Quest.Objectives)
	{
		UTextBlock* Text_Objective = NewObject<UTextBlock>(this, UTextBlock::StaticClass());;
		//UTextBlock* Text_Progress = NewObject<UTextBlock>(this, UTextBlock::StaticClass());;

		switch (Objective.QuestTarget)
		{
		case EQuestTarget::PickUpItem:
		{
			FString ObjectName(TEXT("\"δ��д\""));
			if (Objective.Item)
			{
				Objective.Item->GetName(ObjectName);
			}

			ObjectiveStr += FString::Printf(TEXT("ʰȡ %d �� %s ��Ʒ  %d / %%d"), Objective.Count, *ObjectName, Objective.CurrCount, Objective.Count);
			//ProgressStr += FString::Printf(TEXT("Ŀǰ�Ѿ�ʰȡ %d �� %s ��Ʒ"), Objective.CurrCount, *ObjectName);

			break;
		}
		case EQuestTarget::Kill:
		{
			FString ObjectName(TEXT("\"δ��д\""));
			if (Objective.NPC)
			{
				Objective.NPC->GetName(ObjectName);
			}

			ObjectiveStr += FString::Printf(TEXT("��ɱ%d�� %s NPC  %d / %%d"), Objective.Count, *ObjectName, Objective.CurrCount, Objective.Count);
			//ProgressStr += FString::Printf(TEXT("Ŀǰ�Ѿ���ɱ%d�� %s NPC"), Objective.CurrCount, *ObjectName);

			break;
		}
		case EQuestTarget::GoToArea:
		{
			FString PosStr = Objective.TargetPosition.ToString();
			FString Progress = Objective.bReach ? TEXT("�Ѿ��ߵ�ָ�������") : TEXT("δ�ߵ�ָ�������");
			ObjectiveStr += FString::Printf(TEXT("�ߵ�ָ�������%s   %s"), *PosStr, *Progress);
			//ProgressStr += Objective.bReach ? TEXT("�Ѿ��ߵ�ָ�������") : TEXT("δ�ߵ�ָ�������");

			break;
		}
		default:
			break;
		}
		
		Text_Objective->SetText(FText::FromString(ObjectiveStr));
		
		VB_OnProgress->AddChild(Text_Objective);
	}
}
