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

	/** ��̫�汻��ϻ������ʱ�򱻵��ã��ڲ�ͬ�����������ص���д */
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** ��ͨ������Ҫ�����Ĳ�ͬ�߼���������������д��Ҫ���߼� */
	virtual void NormalCombo() {};

protected:
	virtual void BeginPlay() override;

	//��ײ�¼�����ʵ�֣���������д
	UFUNCTION()
	virtual void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayMontage_Internal(int32 Index);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMult_PlayMontage(UAnimInstance* AnimInst);

	virtual void ResetAttackMontValue();

	UAnimInstance* GetAnimInst();

protected:
	//�Ƿ��������
	bool CanCombo;

	//�Ƿ��ڹ���
	bool IsAttacking = false;

	int32 CurrPlayAnimMont_Index = -1;

	//����֡�Ƿ����
	bool bHitFrameFinish;
};
