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
	

	//���Health�������ͣ�����˵���Health����������ʶHealth��
	FGameplayAttribute HealthAttribute() const;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//ִ��ǰִ��
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	//ִ�к�ִ��
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};
