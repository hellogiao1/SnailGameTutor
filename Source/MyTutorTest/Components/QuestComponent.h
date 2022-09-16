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
	
	//�������
	void AddQuest(const FQuestDetail& Quest);
	
	//ɾ������
	bool DeleteQuestForID(int32 ID);

	//ͨ��ID�����Ƿ��������
	bool ExistQuest(int32 ID);
	
	//ͨ��ID��������
	FQuestDetail* GetQuestForID(int32 ID);

	//�������������
	void FindQuestForComplete(TArray<FQuestDetail>& OutArray);

	//�����Ѿ���ȡ��δ��ɵ�����
	void FindQuestForNoComplete(TArray<FQuestDetail>& OutArray);

	//���δ��ȡ������
	bool GetUnAcceptQuest(TArray<FQuestDetail>& OutUnAccepts, const TArray<FQuestDetail>& InFinderQuests);
	bool ExistUnAcceptQuest(const TArray<FQuestDetail>& InFinderQuests);

	//��ҽ�ȡ����δ��ɵ�����
	bool GetAcceptUnFinishQuest(TArray<FQuestDetail>& OutArray, const TArray<FQuestDetail>& InFinderQuests);
	bool ExistAcceptUnFinish(const TArray<FQuestDetail>& InFinderQuests);

	//�����NPC���Ѿ���ɵ�����
	bool GetFinishQuestsForNPC(TArray<FQuestDetail>& OutArray, const TArray<FQuestDetail>& InFinderQuests);
	bool ExistFinishQuestForNPC(const TArray<FQuestDetail>& InFinderQuests);

protected:
	//���������������
	TMap<int32, FQuestDetail> Quests;
	
};
