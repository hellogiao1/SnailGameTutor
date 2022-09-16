 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Player/MyCharacterBase.h"
#include "QuestData.generated.h"

UENUM(BlueprintType)
enum class EQuestTarget : uint8
{
	PickUpItem,
	Kill,
	GoToArea
};

UENUM(BlueprintType)
enum class EQuestRecCond : uint8
{
	LevelRequired,
	PreQuest,
	ItemOwn
};

// Struct只能传引用，不能传指针
USTRUCT(BlueprintType)
struct FQuestAcceptCond : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestRecCond QuestCond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestCond == EQuestRecCond::LevelRequired", EditConditionHides, ClampMin = 0))
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestCond == EQuestRecCond::PreQuest", EditConditionHides))
	int32 PreQuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestCond == EQuestRecCond::ItemOwn", EditConditionHides))
	TSubclassOf<AActor> Item;
};

USTRUCT(BlueprintType)
struct FObjective : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestTarget QuestTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestTarget == EQuestTarget::PickUpItem", EditConditionHides))
	TSubclassOf<AActor> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestTarget == EQuestTarget::Kill", EditConditionHides))
	TSubclassOf<AMyCharacterBase> NPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestTarget == EQuestTarget::GoToArea", EditConditionHides))
	FVector TargetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestTarget != EQuestTarget::GoToArea", EditConditionHides, ClampMin = 1))
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct FQuestDetail : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 UniqueID = FMath::Rand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjective Objective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FQuestAcceptCond> QuestAcceptConds;

	UPROPERTY()
	bool bIsComplete = false;	//是否完成

	UPROPERTY()
	bool bIsProgress = false;	//是否在进行

	bool operator==(const FQuestDetail& Quest)
	{
		return this->UniqueID == Quest.UniqueID;
	}

	//构造函数，随机生成一个ID
	/*FQuestDetail()
	{
		UniqueID = FMath::Rand();
	}*/
};


