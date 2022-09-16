// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTutorTest/Data/QuestData.h"
#include "UIQuestButton.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FButtonClickSignature, const FQuestDetail&);

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIQuestButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quest;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_BtnName;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FadeOut;

	FButtonClickSignature OnQuestBtnDown;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnQuestBtn_Down();

	int32 GetUniqueID() const;
	void SetUniqueID(int32 ID);

	const FQuestDetail& GetQuestDetail();
	void SetQuestDetail(const FQuestDetail& Detail);

	void SetButtonName(const FText& Name);

	void PlayAnimFadeOut();

private:
	int32 UniqueID;
	FQuestDetail QuestDetail;
};
