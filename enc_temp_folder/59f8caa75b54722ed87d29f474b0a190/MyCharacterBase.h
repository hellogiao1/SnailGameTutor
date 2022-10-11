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

	/** 属性复制 */
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

	/** https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/QuickStart/ 官方文档关于网络的信息*/
protected:
	/** 玩家的最大生命值。这是玩家的最高生命值，也是出生时的生命值。*/
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float MaxHealth;

	/** 玩家的当前生命值。降到0就表示死亡。*/
    UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
    float CurrentHealth;

	/** RepNotify，用于同步对当前生命值所做的更改。*/
    UFUNCTION()
    virtual void OnRep_CurrentHealth();

	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	virtual void OnHealthUpdate();

public:
	/** 最大生命值的取值函数。*/
    UFUNCTION(BlueprintPure, Category="Health")
    FORCEINLINE float GetMaxHealth() const { return MaxHealth; } 

	/** 当前生命值的取值函数。*/
    UFUNCTION(BlueprintPure, Category="Health")
    FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** 当前生命值的存值函数。将此值的范围限定在0到MaxHealth之间，并调用OnHealthUpdate。仅在服务器上调用。*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SetCurrentHealth(float healthValue);

	/** 承受伤害的事件。从APawn覆盖。*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
