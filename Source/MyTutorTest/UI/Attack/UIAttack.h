// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUIAttack : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Attack;

private:
	UFUNCTION()
	void OnAttackBtn_Down();
};
