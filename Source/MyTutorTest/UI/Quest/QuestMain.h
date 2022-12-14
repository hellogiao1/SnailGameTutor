// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTutorTest/Data/QuestData.h"
#include "QuestMain.generated.h"

class UButton;
class UScrollBox;
class UUIQuestButton;

enum class EActivateQuest : uint8
{
	Self,
	Other,
};

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UQuestMain : public UUserWidget
{
	GENERATED_BODY()

public:
	//UPROPERTY(meta = (BindWidget))
	UButton* Btn_QuestUnAccept;

	//UPROPERTY(meta = (BindWidget))
	UButton* Btn_QuestAccept;

	//UPROPERTY(meta = (BindWidget))
	UButton* Btn_QuestDoing;

	//UPROPERTY(meta = (BindWidget))
	UButton* Btn_QuestComplete;

	//UPROPERTY(meta = (BindWidget))
	class UUIQuestInfo* QuestDetail;

	//UPROPERTY(meta = (BindWidget))
	UScrollBox* SB_QuestList;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CloseQuest;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUIQuestButton> BPQuestBtnClass;

	virtual void NativeConstruct() override;

public:
	//设置对象的指针
	void SetTargetObject(AActor* NewChar, EActivateQuest NewWay);
	FORCEINLINE AActor* GetTargetObject() { return Char; }

private:
	void InteractionView();

	/** 按钮绑定事件 */

	UFUNCTION()
	void OnCloseBtn_Down();

	//可领取任务
	UFUNCTION()
	void OnUnAcceptBtn_Down();

	//已领取任务
	UFUNCTION()
	void OnAcceptBtn_Down();

	//追踪任务
	UFUNCTION()
	void OnDoingBtn_Down();

	//已经完成的任务
	UFUNCTION()
	void OnCompleteBtn_Down();

	//任务详情刷新
	UFUNCTION()
	void FlushQuestDetail(const FQuestDetail& Quest);

	//给任务列表填充任务
	void FillTaskList(const TArray<FQuestDetail>& Quests);

	//DetailInfo 相关回调函数
	void AddQuest(const FQuestDetail& Quest);
	void CommitQuest(const FQuestDetail& Quest);
	void OnProgressQuest(const FQuestDetail& Quest);
	void StopProgressQuest(const FQuestDetail& Quest);

public:
	//quest pool
	UUIQuestButton* GetQuestBtnByPool();
	int32 PoolCount = 0;

private:

	UPROPERTY()
	TArray<UUIQuestButton*> QuestPool;

private:
	AActor* Char;
	EActivateQuest ActiveWay;
};
