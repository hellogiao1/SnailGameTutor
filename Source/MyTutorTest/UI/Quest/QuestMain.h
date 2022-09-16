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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUIQuestButton> BPQuestBtnClass;

	virtual void NativeConstruct() override;

public:
	//���ö����ָ��
	void SetTargetObject(AActor* NewChar, EActivateQuest NewWay);
	FORCEINLINE AActor* GetTargetObject() { return Char; }

private:
	void InteractionView();

	/** ��ť���¼� */

	//����ȡ����
	UFUNCTION()
	void OnUnAcceptBtn_Down();

	//����ȡ����
	UFUNCTION()
	void OnAcceptBtn_Down();

	//׷������
	UFUNCTION()
	void OnDoingBtn_Down();

	//�Ѿ���ɵ�����
	UFUNCTION()
	void OnCompleteBtn_Down();

	//��������ˢ��
	UFUNCTION()
	void FlushQuestDetail(const FQuestDetail& Quest);

	//�������б��������
	void FillTaskList(const TArray<FQuestDetail>& Quests);

	//����������ص�
	void AddQuest(const FQuestDetail& Quest);
	void CommitQuest(const FQuestDetail& Quest);

	//����������ص�

private:
	AActor* Char;
	EActivateQuest ActiveWay;
};
