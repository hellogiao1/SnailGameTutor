// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UIAcceptQuest.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIAcceptQuest : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Interaction;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_NPCName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Name;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher_74;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnInterBtn_Down();

	UFUNCTION()
	void OnNPCNameBtn_Down();

public:
	void SetWidgetName(const FText& NewName);
	const FText& GetWidgetName();

private:
	FText MyName;
};
