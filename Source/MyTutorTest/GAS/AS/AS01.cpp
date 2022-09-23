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

	//���������޸ĵ�������Health���ԣ���ô������Clamp
	if (Attribute == HealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
		//Clamp��ʾ��ȡ��ĳ����Χ�ڣ�����������Ϥ���뿴�ٷ��ĵ���FMath�й������Ľ���

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

	//���ʩ�Ŵ˴��޸ĵ�Դͷ
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	//�˴��޸ĵ�������Health��
	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		AActor* TargetActor = nullptr;
		AController* TargetController = nullptr;
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

			//����������޸�Ŀ��TargetActor��ץȡ����
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

			//��������˷����޸ĵ�ԴͷSourceActor��ץȡ����
			UE_LOG(LogTemp, Warning, TEXT("The SourceActor Name is:%s"), *(SourceActor->GetName()));
		}

		//Clamp����ʵClamp���ǡ�ִ�к��¼�������Ҫ���߼�
		Health = FMath::Clamp(Health, 0.f, 100.f);

		//��ִ�к��¼�������Ҫ���߼��ǣ������ֵ̫�ͣ���ô���ִ����ض����飬��Health���㣬��ô������
		if (Health <= 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("Death Logic Here ִ�������߼�"));
			TargetActor->Destroy();
		}
	}
	return;
}
