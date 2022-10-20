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

public:
	ABow();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Excute_NormalAttack() override;

	virtual void OnNormalBtn_Release() override;

private:
	UFUNCTION()
	void SetCanFire();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMult_LaunchProjectile();

	void ResetValue();

	void SetControlView(bool LockView);

private:
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bArrowLoaded;

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bFiringArrow;

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bBowAiming;

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool CanFire;

	FTimerHandle DelayHandle;

	//–Ó¡¶ ±º‰
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	float RateTime;
};
