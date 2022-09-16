// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTutorTest/Data/QuestData.h"
#include "UIQuestInfo.generated.h"

class UTextBlock;
class UScrollBox;
class UButton;
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

	virtual void NativeConstruct() override;

	void ShowQuestDetail(const FQuestDetail& QuestDetail);

	void ResetDetail();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TestClass;

	//领取任务
	UFUNCTION()
	void OnAcceptBtn_Down();

	//完成任务
	UFUNCTION()
	void OnCompleteBtn_Down();

	//设置打开任务的方式
	void IsActiveOpen(bool Active);

private:
	FQuestDetail MyQuest;

	//是否通过交互打开任务界面
	bool bIsActiveOpen = false;
};
