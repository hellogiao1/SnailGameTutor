// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MytutorTest/Data/QuestData.h"
#include "UIOnProgressTipBar.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIOnProgressTipBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VB_OnProgress;

	void InitQuestDetail(int32 ID);

	void UpdateQuestDetail();

private:
	int32 MyID;
};
