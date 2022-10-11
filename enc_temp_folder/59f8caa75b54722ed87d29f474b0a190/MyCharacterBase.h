// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacterBase.generated.h"

class UChildActorComponent;

UCLASS()
class MYTUTORTEST_API AMyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacterBase();

	/** ���Ը��� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	UChildActorComponent* RightWeaponComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	UChildActorComponent* LeftWeaponComp;

protected:
	virtual void BeginPlay() override;

	/** https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/QuickStart/ �ٷ��ĵ������������Ϣ*/
protected:
	/** ��ҵ��������ֵ��������ҵ��������ֵ��Ҳ�ǳ���ʱ������ֵ��*/
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float MaxHealth;

	/** ��ҵĵ�ǰ����ֵ������0�ͱ�ʾ������*/
    UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
    float CurrentHealth;

	/** RepNotify������ͬ���Ե�ǰ����ֵ�����ĸ��ġ�*/
    UFUNCTION()
    virtual void OnRep_CurrentHealth();

	/** ��ӦҪ���µ�����ֵ���޸ĺ������ڷ������ϵ��ã����ڿͻ����ϵ�������ӦRepNotify*/
	virtual void OnHealthUpdate();

public:
	/** �������ֵ��ȡֵ������*/
    UFUNCTION(BlueprintPure, Category="Health")
    FORCEINLINE float GetMaxHealth() const { return MaxHealth; } 

	/** ��ǰ����ֵ��ȡֵ������*/
    UFUNCTION(BlueprintPure, Category="Health")
    FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ��ǰ����ֵ�Ĵ�ֵ����������ֵ�ķ�Χ�޶���0��MaxHealth֮�䣬������OnHealthUpdate�����ڷ������ϵ��á�*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SetCurrentHealth(float healthValue);

	/** �����˺����¼�����APawn���ǡ�*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
