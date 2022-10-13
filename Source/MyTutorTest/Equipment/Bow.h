// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipObject.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API ABow : public AEquipObject
{
	GENERATED_BODY()
	
protected:
	virtual void Excute_NormalAttack() override;

};
