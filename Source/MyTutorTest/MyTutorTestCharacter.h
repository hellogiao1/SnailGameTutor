// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestData.h"
#include "Player/MyCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "MyTutorTestCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDamageSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FModifyPropSignature, float, const FText&);
DECLARE_DELEGATE(FInterativeSignature);

UENUM()
enum class EAttackType : uint8
{
	Normal,

};

class UUIPlayerProperty;
class UUI_HUD;

UCLASS(config=Game)
class AMyTutorTestCharacter : public AMyCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Follow Head UI */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HeadTipWidgetComp;

	/** ??????? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	class UQuestComponent* QuestComp;

#pragma region GAS
private:
	/** 添加Ability组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayAbility>> MyAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSets")
		TArray<TSubclassOf<class UAttributeSet>> AttributeSets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		UDataTable* AttrDataTable;
#pragma endregion

public:
	AMyTutorTestCharacter();

	// 设置右边武器是否开启检测事件
	void SetRWeaponGeneOverlap(bool bInGeneraOverlap);

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere, Category = "DeBug")
	float TraceDistance = 1000.f;

	UPROPERTY(ReplicatedUsing = OnRep_FightState, EditAnywhere, BlueprintReadWrite, Category = "State|Fight")
	bool bFightState = false;

	UFUNCTION()
	void OnRep_FightState();

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void AcceleRun();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	void LineTraceInteraction();

public:
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns QuestComponent **/
	FORCEINLINE class UQuestComponent* GetQuestComponent() const { return QuestComp; }

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void OnCharacterDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Client, Reliable)
	void Died();

	float GetHP();
	void SetHP(const float NewHealth);

	float GetMaxHP();
	void SetMaxHP(const float NewMaxHP);

	float GetLevel();
	void SetLevel(const float NewLevel);

	const FName& GetCharacterName();

	UFUNCTION(Server, Reliable)
	void SetCharacterName(const FName& NewName);

	/*UFUNCTION()
	void OnRep_UpdateUI(const FName& NewName, const float Health);*/

	UFUNCTION()
	void OnRep_UpdateUI();

	FDamageSignature OnCharHit;

	FModifyPropSignature OnPropertyChange;

	FInterativeSignature InteractionEvent;

	// 响应交互事件委托调用
	void OnInteractiveInput();

	void OpenQuestUI();

	void SetNPCPtr(const AMyCharacterBase* Character);
	FORCEINLINE const AMyCharacterBase* GetNPCPtr() { return InteraCharacter; }

	/** Inventory Func */
	bool ExistItem(UClass* ItemClass);
	void AddItem(AActor* Item);
	void DeleteItem(AActor* Item);

	//
	UFUNCTION(BlueprintCallable, Category = "QuestFlush")
	void AddQuestAsObject(EQuestTarget QuestTarget, TSubclassOf<AActor> TargetObject, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "QuestFlush")
	void NotifyQuestReachPos(FVector TargetPosition, bool bReach);

	/** Fight */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Fight")
	void SwitchState();

	UFUNCTION(Server, Reliable)
	void Attack(EAttackType AttackType);

	UFUNCTION(NetMulticast, Unreliable)
	void NormalAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Attack")
	TArray<UAnimMontage*> AttackMontages;

	//攻击动画结束后的回调函数：重置初始化属性
	UFUNCTION(NetMulticast, Unreliable)
	void OnAttackMontEnd_CallBack();

	UFUNCTION(NetMulticast, Unreliable)
	void SetCanCombo(bool newCanCombo);

private:
	// TODO: ...ClampMax ???????? ClampMax = MaxHP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperty", meta = (AllowPrivateAccess = "true", ClampMax = 100.f, ClampMin = 0.f), ReplicatedUsing = OnRep_UpdateUI)
	float HP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperty", meta = (AllowPrivateAccess = "true"))
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperty", meta = (AllowPrivateAccess = "true"), ReplicatedUsing = OnRep_UpdateUI)
	FName CharacterName = "DarkLight";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperty", meta = (AllowPrivateAccess = "true"), ReplicatedUsing = OnRep_UpdateUI)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> UIDiedClass;

	AMyCharacterBase* InteraCharacter;

	AActor* HitActor;

	//临时的背包
	TArray<AActor*> Inventory;

	FTimerHandle TraceTimerHandle;

	/** 连招相关变量 */
	int32 CurrPlayAnimMont_Index;
	bool CanCombo = true;
	bool bDoOnce = true;
	bool bFirstMouseBtnDown = true;

};

