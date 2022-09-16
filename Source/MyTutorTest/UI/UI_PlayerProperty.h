// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerProperty.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIPlayerProperty : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Name;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HP;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetCharacterName(const FText& NewName);

	UFUNCTION()
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION()
	void SetCharProperty(float Percentage, const FText& NewName);
};
