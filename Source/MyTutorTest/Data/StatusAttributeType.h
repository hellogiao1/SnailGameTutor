// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTableEditorUtils.h"
#include "UObject/ReflectedTypeAccessors.h"
#include "StatusAttributeType.generated.h"

UENUM(BlueprintType)
enum class EBasicStatus : uint8
{
	Strength,
	Aglility,
	Toughness,
	Stamina,
	Intelligence
};

UENUM(BlueprintType)
enum class EDetailStatus : uint8
{
	HP,
	MP,
	Speed,
	Attack,
	Defence,
	WeightBear
};

USTRUCT(BlueprintType)
struct FBasicProperty : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBasicStatus BasicName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	FBasicProperty() = default;

	FBasicProperty(EBasicStatus NewBasicName, float NewValue) {
		BasicName = NewBasicName;
		Value = NewValue;
	}

	//当datatable选定以后，rowname参数会自动改变，不过比较耗资源，暂时不使用
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		/*if (InDataTable == nullptr) return;

		FBasicProperty* InStruct = InDataTable->FindRow<FBasicProperty>(InRowName, TEXT(""));
		FName CurrName = StaticEnum<EBasicStatus>()->GetNameByIndex((int32)BasicName);
		if (InStruct && CurrName != InRowName)
		{
			const_cast<UDataTable*>(InDataTable)->AddRow(CurrName, *InStruct);
		}*/
		/*UDataTable* DataTable = const_cast<UDataTable*>(InDataTable);
		if (!DataTable) return;

		TArray<TArray<FString>> TableData = DataTable->GetTableData();
#if WITH_EDITOR
		for (int i = 1; i < TableData.Num(); i++)
		{
			FDataTableEditorUtils::RenameRow(DataTable, *TableData[i][0], *TableData[i][1]);
		}
#endif

		DataTable->Modify();*/
	}
};

USTRUCT(BlueprintType)
struct FDetailProperty : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDetailStatus DetailName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = "float/s"))
	float RecoverSpeed;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoverTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (SliderExponent))
	float RecoverQuantity;
};

USTRUCT(BlueprintType)
struct FAttributeMapping : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DataTableInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDetailProperty> Details;
};
