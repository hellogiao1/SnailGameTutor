// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_HUD.generated.h"

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

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCharInfoBtn_Down();
};
