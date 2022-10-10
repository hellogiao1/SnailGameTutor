// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillComboData.generated.h"

USTRUCT(BlueprintType)
struct FMontageArray : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackMonts;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillComboTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillComboIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FMontageArray> AttackMontageMap;

	//------------------------

	/*void GetMontageArrByIndex(int32 Index, FMontageArray& OutArray);*/

	FORCEINLINE FMontageArray* MontageArray()
	{
		if (AttackMontageMap.Contains(SkillComboIndex))
		{
			return AttackMontageMap.Find(SkillComboIndex);
		}
		return nullptr;
	}
};

// void FSkillComboTable::GetMontageArrByIndex(int32 Index, FMontageArray& OutArray)
// {
// 	if (AttackMontageMap.Contains(Index))
// 	{
// 		OutArray = *AttackMontageMap.Find(Index);
// 	}
// }
