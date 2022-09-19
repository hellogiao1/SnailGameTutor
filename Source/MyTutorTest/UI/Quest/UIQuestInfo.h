// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTutorTest/Data/QuestData.h"
#include "UIQuestInfo.generated.h"

class UTextBlock;
class UScrollBox;
class UButton;

DECLARE_DELEGATE_OneParam(FAddQuestSignature, const FQuestDetail&);
DECLARE_DELEGATE_OneParam(FCommitQuestSignature, const FQuestDetail&);
DECLARE_DELEGATE_OneParam(FProgressSignature, const FQuestDetail&);
DECLARE_DELEGATE_OneParam(FStopProgressSignature, const FQuestDetail&);

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIQuestInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Objective;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Progress;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Reward;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox_Cond;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Accept;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Complete;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_OnProgress;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_StopProgress;

	virtual void NativeConstruct() override;

	void ShowQuestDetail(const FQuestDetail& QuestDetail);

	FAddQuestSignature AddQuestEvent;
	FCommitQuestSignature CommitQuestEvent;
	FProgressSignature OnProgressEvent;
	FStopProgressSignature StopProgressEvent;

private:
	void ResetDetail();

	//领取任务
	UFUNCTION()
	void OnAcceptBtn_Down();

	//完成任务
	UFUNCTION()
	void OnCompleteBtn_Down();

	//追踪任务
	UFUNCTION()
	void OnProgressBtn_Down();

	//取消追踪任务
	UFUNCTION()
	void OnStopProgressBtn_Down();

	//根据枚举更新 任务目标和任务进度
	void ObjectiveAndProgress(const FQuestDetail& Quest);

	//任务领取条件
	void QuestCondition(const FQuestDetail& Quest);

private:
	const FQuestDetail* MyQuest;
};
