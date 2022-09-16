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

	FAddQuestSignature AddQuestEvent;
	FCommitQuestSignature CommitQuestEvent;

private:
	void ResetDetail();

	//��ȡ����
	UFUNCTION()
	void OnAcceptBtn_Down();

	//�������
	UFUNCTION()
	void OnCompleteBtn_Down();

	//����ö�ٸ��� ����Ŀ����������
	void ObjectiveAndProgress(const FQuestDetail& Quest);

	//������ȡ����
	void QuestCondition(const FQuestDetail& Quest);

private:
	const FQuestDetail* MyQuest;
};
