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

	/** �ж��Ƿ�Ϊͬһ������ */
	UFUNCTION(BlueprintPure, Category="Game")
	static bool IsTeam(AActor* Src, AActor* Dest);
};
