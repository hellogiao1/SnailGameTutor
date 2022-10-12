// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "EquipObject.generated.h"

UCLASS()
class MYTUTORTEST_API AEquipObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AEquipObject();

	virtual void Tick(float DeltaTime) override;

	void SetGenerateOverlap(bool bInGeneOverlap);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageProperty")
	float AttackValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackMontage")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackMontage", meta = (AllowedClasses = "UAnimMontage"))
	TArray<FSoftObjectPath> ObjectsPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackMontage", meta = (AllowedClasses = "UStaticMesh"))
	FSoftObjectPath ObjectsPath1;


public:
	FORCEINLINE bool GetCanCombo() const { return CanCombo; }
	void SetCanCombo(bool canCombo);

	FORCEINLINE bool GetbHitFrameFinish() const { return bHitFrameFinish; }
	void SetbHitFrameFinish(bool NewValue);

	virtual void Excute_NormalAttack();

protected:
	FOnMontageEnded MontageEndedDelegate;

	/** 蒙太奇被打断或结束的时候被调用，在不同的子类进行相关的重写 */
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** 普通攻击需要连击的不同逻辑，可以在子类重写需要的逻辑 */
	virtual void NormalCombo() {};

protected:
	virtual void BeginPlay() override;

	//碰撞事件，空实现，在子类重写
	UFUNCTION()
	virtual void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayMontage_Internal(int32 Index);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMult_PlayMontage(UAnimInstance* AnimInst);

	virtual void ResetAttackMontValue();

	UAnimInstance* GetAnimInst();

protected:
	//是否可以连击
	bool CanCombo;

	//是否在攻击
	bool IsAttacking = false;

	int32 CurrPlayAnimMont_Index = -1;

	//攻击帧是否结束
	bool bHitFrameFinish;
};
