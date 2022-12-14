// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "../Data/QuestData.h"
#include "../Player/MyCharacterBase.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::GetQuests(TArray<FQuestDetail>& OutArray)
{
	Quests.GenerateValueArray(OutArray);
}

void UQuestComponent::AddQuest(const FQuestDetail& NewQuest)
{
	Quests.Emplace(NewQuest.UniqueID, NewQuest);
}

bool UQuestComponent::DeleteQuestForID(int32 ID)
{
	if (Quests.Contains(ID))
	{
		Quests.Remove(ID);
		return true;
	}
	return false;
}

bool UQuestComponent::ExistQuest(int32 ID)
{
	return Quests.Contains(ID);
}

FQuestDetail* UQuestComponent::GetQuestForID(int32 ID)
{
	if (ExistQuest(ID))
	{
		return Quests.Find(ID);
	}
	return nullptr;
}

void UQuestComponent::FindQuestForComplete(TArray<FQuestDetail>& OutArray)
{
	OutArray.Reset();
	for (const auto& quest : Quests)
	{
		if (quest.Value.bIsComplete == true)
		{
			OutArray.Emplace(quest.Value);
		}
	}
}

void UQuestComponent::FindQuestForNoComplete(TArray<FQuestDetail>& OutArray)
{
	OutArray.Reset();
	for (const auto& quest : Quests)
	{
		if (quest.Value.bIsComplete == false)
		{
			OutArray.Emplace(quest.Value);
		}
	}
}

void UQuestComponent::FindOnProgresQuests(TArray<FQuestDetail>& OutArray)
{
	OutArray.Reset();
	for (const auto& quest : Quests)
	{
		if (quest.Value.bIsComplete == false && quest.Value.bIsProgress == true)
		{
			OutArray.Emplace(quest.Value);
		}
	}
}

bool UQuestComponent::GetUnAcceptQuest(TArray<FQuestDetail>& OutUnAccepts, const TArray<FQuestDetail>& InFinderQuests)
{
	OutUnAccepts.Reset();
	bool flag = false;
	for (const auto& quest : InFinderQuests)
	{
		if (ExistQuest(quest.UniqueID) == false)
		{
			OutUnAccepts.Emplace(quest);
			flag = true;
		}
	}
	return flag;
}

bool UQuestComponent::ExistUnAcceptQuest(const TArray<FQuestDetail>& InFinderQuests)
{
	for (const auto& quest : InFinderQuests)
	{
		if (ExistQuest(quest.UniqueID) == false)
		{
			return true;
		}
	}
	return false;
}

bool UQuestComponent::GetAcceptUnFinishQuest(TArray<FQuestDetail>& OutArray, const TArray<FQuestDetail>& InFinderQuests)
{
	OutArray.Reset();
	bool flag = false;
	for (const auto& quest : InFinderQuests)
	{
		if (ExistQuest(quest.UniqueID) && !GetQuestForID(quest.UniqueID)->bIsComplete)
		{
			OutArray.Emplace(*GetQuestForID(quest.UniqueID));
			flag = true;
		}
	}
	return flag;
}

bool UQuestComponent::ExistAcceptUnFinish(const TArray<FQuestDetail>& InFinderQuests)
{
	for (const auto& quest : InFinderQuests)
	{
		if (ExistQuest(quest.UniqueID) && !GetQuestForID(quest.UniqueID)->bIsComplete)
		{
			return true;
		}
	}
	return false;
}

bool UQuestComponent::GetFinishQuestsForNPC(TArray<FQuestDetail>& OutArray, const TArray<FQuestDetail>& InFinderQuests)
{
	OutArray.Reset();
	bool flag = false;
	for (const auto& quest : InFinderQuests)
	{
		if (ExistQuest(quest.UniqueID) && GetQuestForID(quest.UniqueID)->bIsComplete)
		{
			OutArray.Emplace(*GetQuestForID(quest.UniqueID));
			flag = true;
		}
	}
	return flag;
}

bool UQuestComponent::ExistFinishQuestForNPC(const TArray<FQuestDetail>& InFinderQuests)
{
	for (const auto& quest : InFinderQuests)
	{
		if (ExistQuest(quest.UniqueID) && GetQuestForID(quest.UniqueID)->bIsComplete)
		{
			return true;
		}
	}
	return false;
}

void UQuestComponent::UpdateRequest(EQuestTarget QuestTarget, TSubclassOf<AActor> TargetObject, int32 Count)
{
	switch (QuestTarget)
	{
	case EQuestTarget::PickUpItem:
		{
			for (auto& It : Quests)
			{
				if (It.Value.bIsComplete == false)
				{
					for (auto& quest : It.Value.Objectives)
					{
						if (quest.QuestTarget == QuestTarget && quest.Item == TargetObject)
						{
							quest.CurrCount += Count;
						}
					}
				}

			}
			break;
		}
	case EQuestTarget::Kill:
		{
			for (auto& It : Quests)
			{
				if (It.Value.bIsComplete == false)
				{
					for (auto& quest : It.Value.Objectives)
					{
						if (quest.QuestTarget == QuestTarget && quest.NPC == TargetObject)
						{
							quest.CurrCount += Count;
						}
					}
				}
				
			}
			break;	
		}
	case EQuestTarget::GoToArea:
		{
			
			break;
		}
	default: ;
	}
}

void UQuestComponent::UpdateRequest(EQuestTarget QuestTarget, FVector TargetPosition, bool bReach)
{
	if (QuestTarget == EQuestTarget::GoToArea)
	{
		for (auto& It : Quests)
		{
			if (It.Value.bIsComplete == false)
			{
				for (auto& quest : It.Value.Objectives)
				{
					if (quest.QuestTarget == QuestTarget && quest.TargetPosition == TargetPosition)
					{
						quest.bReach = bReach;
					}
				}
			}
			
		}
	}
}

