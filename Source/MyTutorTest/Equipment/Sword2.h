// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipObject.h"
#include "Sword2.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API ASword2 : public AEquipObject
{
	GENERATED_BODY()

protected:
	virtual void NormalCombo() override;

	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
		
};
