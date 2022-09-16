// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPlayerInfoView.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIPlayerInfoView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ChangeName;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Close;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Health;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EditTexBx_Name;

	class AMyTutorTestCharacter* MyTestCharcter;

protected:
	FDelegateHandle TempFuncHandle;

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBtnChangeName_Down();

	UFUNCTION()
	void OnBtnClose_Down();

	void SetHealthPercentage(const float Health);

	void SetEditName(const FText& Text);

	UFUNCTION()
	void ChangeUIProperty(float Health, const FText& Text);
};
