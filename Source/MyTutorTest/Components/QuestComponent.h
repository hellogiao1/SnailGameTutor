// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Data/QuestData.h"
#include "QuestComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTUTORTEST_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void GetQuests(TArray<FQuestDetail>& OutArray);
	
	//添加任务
	void AddQuest(const FQuestDetail& Quest);
	
	//删除任务
	bool DeleteQuestForID(int32 ID);

	//通过ID查找是否存在任务
	bool ExistQuest(int32 ID);
	
	//通过ID查找任务
	FQuestDetail* GetQuestForID(int32 ID);

	//查找已完成任务
	void FindQuestForComplete(TArray<FQuestDetail>& OutArray);

	//查找已经领取但未完成的任务
	void FindQuestForNoComplete(TArray<FQuestDetail>& OutArray);

	//玩家未接取的任务
	bool GetUnAcceptQuest(TArray<FQuestDetail>& OutUnAccepts, const TArray<FQuestDetail>& InFinderQuests);
	bool ExistUnAcceptQuest(const TArray<FQuestDetail>& InFinderQuests);

	//玩家接取但是未完成的任务
	bool GetAcceptUnFinishQuest(TArray<FQuestDetail>& OutArray, const TArray<FQuestDetail>& InFinderQuests);
	bool ExistAcceptUnFinish(const TArray<FQuestDetail>& InFinderQuests);

	//玩家在NPC上已经完成的任务
	bool GetFinishQuestsForNPC(TArray<FQuestDetail>& OutArray, const TArray<FQuestDetail>& InFinderQuests);
	bool ExistFinishQuestForNPC(const TArray<FQuestDetail>& InFinderQuests);

protected:
	//玩家身上所有任务
	TMap<int32, FQuestDetail> Quests;
	
};
