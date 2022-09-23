// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AS01.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UAS01 : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "Wizard Attributes | Health", EditAnywhere, BlueprintReadWrite)
	float Health;
	

	//获得Health属性类型（或者说获得Health键，用来标识Health）
	FGameplayAttribute HealthAttribute() const;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//执行前执行
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	//执行后执行
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};
