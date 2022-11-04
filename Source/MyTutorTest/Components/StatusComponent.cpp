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
		DTBaseStatus->GetAllRows<FBasicProperty>(TEXT(""), BasicPropertys);
	}

	if (DTDetailStatus)
	{
		DTDetailStatus->GetAllRows<FDetailProperty>(TEXT(""), DetailPropertys);
	}

	if (DTAttrMappingStatus)
	{
		DTAttrMappingStatus->GetAllRows<FAttributeMapping>(TEXT(""), AttributeMappings);
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

