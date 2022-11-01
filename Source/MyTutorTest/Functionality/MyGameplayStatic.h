// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyGameplayStatic.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UMyGameplayStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** 判断是否为同一个队伍 */
	UFUNCTION(BlueprintPure, Category="Game")
	static bool IsTeam(AActor* Src, AActor* Dest);
};
