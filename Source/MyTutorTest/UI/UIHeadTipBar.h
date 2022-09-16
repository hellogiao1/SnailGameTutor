// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHeadTipBar.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIHeadTipBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Blood;

	void SetHeadTipforName(const FText& TextName);

	void SetHeadTipforBloodPerctg(const float Percentage);
};
