// Fill out your copyright notice in the Description page of Project Settings.


#include "AS01.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayEffectExtension.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

/*void UAS01::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
    return;
}*/

FGameplayAttribute UAS01::HealthAttribute() const
{
	static FProperty* Property = FindFieldChecked<FProperty>(UAS01::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UAS01, Health));
	return FGameplayAttribute(Property);
}

void UAS01::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("PreAttributeChagne AS01"));

	//如果这个待修改的属性是Health属性，那么就这样Clamp
	if (Attribute == HealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
		//Clamp表示截取在某个范围内，如果这个不熟悉，请看官方文档中FMath中关于它的介绍

		UE_LOG(LogTemp, Warning, TEXT("%d"), NewValue);
	}
}

bool UAS01::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("57 AS01 PreGPEE"));

	return true;
}

void UAS01::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("This AS is ready to be changed"));

	//获得施放此次修改的源头
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	//此次修改的属性是Health吗？
	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		AActor* TargetActor = nullptr;
		AController* TargetController = nullptr;
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

			//这里给出了修改目标TargetActor的抓取方法
			UE_LOG(LogTemp, Warning, TEXT("The TargetActor Name is:%s"), *(TargetActor->GetName()));
		}

		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AController* SourcePlayerController = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid()
			&& Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();

			//这里给出了发起修改的源头SourceActor的抓取方法
			UE_LOG(LogTemp, Warning, TEXT("The SourceActor Name is:%s"), *(SourceActor->GetName()));
		}

		//Clamp，其实Clamp不是“执行后事件”最重要的逻辑
		Health = FMath::Clamp(Health, 0.f, 100.f);

		//“执行后事件”最重要的逻辑是：如果数值太低，那么表现触发特定事情，如Health触零，那么死亡。
		if (Health <= 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("Death Logic Here 执行死亡逻辑"));
			TargetActor->Destroy();
		}
	}
	return;
}
