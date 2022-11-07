// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (DTBaseStatus)
	{
		DTBaseStatus->ForeachRow<FBasicProperty>(TEXT(""), [&](const FName Name, const FBasicProperty& Property) {
			BasicPropertyMap.Emplace(Property.BasicName, Property);
			});
	}

	if (DTDetailStatus)
	{
		DTDetailStatus->ForeachRow<FDetailProperty>(TEXT(""), [&](const FName Name, const FDetailProperty& Property) {
			DetailPropertyMap.Emplace(Property.DetailName, Property);
			});
	}

	if (DTAttrMappingStatus)
	{
		DTAttrMappingStatus->GetAllRows<FAttributeMapping>(TEXT(""), AttributeMappings);
	}

	InitAttribute();
}

void UStatusComponent::InitAttribute()
{
	if (BasicPropertyMap.IsEmpty() || DetailPropertyMap.IsEmpty() || AttributeMappings.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] -> Init Attribute failed"), *FString(__FUNCTION__));
		return;
	}

	for (const auto& Mapping : AttributeMappings)
	{
		FBasicProperty* Basic = Mapping->DataTableInfo.GetRow<FBasicProperty>(TEXT(""));
		for (const auto& Detail : Mapping->Details)
		{
			SetDetailMaxProperty(Detail.DetailName, Detail.MaxValue * Basic->Value, true);
		}
	}
}

// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	FBasicProperty TestBasic(EBasicStatus::Toughness, 1.f);
	if (DTBaseStatus)
	{
		DTBaseStatus->AddRow(FName("2"), TestBasic);
	}
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const FBasicProperty& UStatusComponent::GetBasicPropertyByName(EBasicStatus BasicStatus) const
{
	if (BasicPropertyMap.Contains(BasicStatus))
	{
		return *BasicPropertyMap.Find(BasicStatus);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] -> Not Find Strtuct"), *FString(__FUNCTION__));
		return FBasicProperty();
	}

}

const FBasicProperty& UStatusComponent::GetBasicPropertyByName(FName BasicStatus) const
{
	EBasicStatus BasicEnum = static_cast<EBasicStatus>(StaticEnum<EBasicStatus>()->GetValueByName(BasicStatus));
	GetBasicPropertyByName(BasicEnum);
}

const FDetailProperty& UStatusComponent::GetDetailPropertyByName(EDetailStatus DetailStatus) const
{
	if (DetailPropertyMap.Contains(DetailStatus))
	{
		return *DetailPropertyMap.Find(DetailStatus);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] -> Not Find Strtuct"), *FString(__FUNCTION__));
		return FDetailProperty();
	}
}

const FDetailProperty& UStatusComponent::GetDetailPropertyByName(FName DetailStatus) const
{
	EDetailStatus DetailEnum = static_cast<EDetailStatus>(StaticEnum<EDetailStatus>()->GetValueByName(DetailStatus));
	GetDetailPropertyByName(DetailEnum);
}

void UStatusComponent::SetBasicProperty(EBasicStatus BasicStatus, float NewValue)
{
	if (BasicPropertyMap.Contains(BasicStatus) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] -> %d Not Find Property"), *FString(__FUNCTION__), BasicStatus);
		return;
	}

	FBasicProperty* Basic = BasicPropertyMap.Find(BasicStatus);
	if (Basic)
	{
		Basic->Value = NewValue;
	}
}

void UStatusComponent::SetDetailProperty(EDetailStatus DetailStatus, float NewValue)
{
	if (DetailPropertyMap.Contains(DetailStatus) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] -> %d Not Find Property"), *FString(__FUNCTION__), DetailStatus);
		return;
	}

	FDetailProperty* Detail = DetailPropertyMap.Find(DetailStatus);
	if (Detail)
	{
		Detail->CurrValue = NewValue;
	}
}

void UStatusComponent::SetDetailMaxProperty(EDetailStatus DetailStatus, float NewValue, bool bResetCurrValue /*= false*/)
{
	if (DetailPropertyMap.Contains(DetailStatus) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Func[%s] -> %d Not Find Property"), *FString(__FUNCTION__), DetailStatus);
		return;
	}

	FDetailProperty* Detail = DetailPropertyMap.Find(DetailStatus);
	if (Detail)
	{
		Detail->MaxValue = NewValue;
		if (bResetCurrValue)
		{
			Detail->CurrValue = NewValue;
		}
	}
}

