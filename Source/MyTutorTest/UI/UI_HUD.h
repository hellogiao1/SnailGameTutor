// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_HUD.generated.h"

struct FQuestDetail;
/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUI_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PlayerInfo;

	UPROPERTY(meta = (BindWidget))
	class UUIOnProgressTipBar* WB_OnProgress;

	virtual void NativeConstruct() override;

	void ShowProgressView(int32 ID);

	void CloseProgressView();

private:
	UFUNCTION()
	void OnCharInfoBtn_Down();
};
