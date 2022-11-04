// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestData.h"
#include "Player/MyCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "Animation/AnimInstance.h"
#include "UObject/SoftObjectPath.h"
#include "MyTutorTestCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDamageSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FModifyPropSignature, float, const FText&);
DECLARE_DELEGATE(FInterativeSignature);

UENUM()
enum class EAttackType : uint8
{
	Normal,

};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Fists,
	MainHand,
	MainHandAndShied,
	Bow,
	Magic,
};

USTRUCT(BlueprintType)
struct FWeaponSofts 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MetaClass = "EquipObject"))
	TArray<FSoftClassPath> WeaponSoftArray;
};

class UUIPlayerProperty;
class UUI_HUD;
class AEquipObject;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	class UStatusComponent* StatusComp;

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

#pragma region InputTouch
public:
	void OnTouchPressed(ETouchIndex::Type FingerIndex,  FVector Location);
	void OnTouchMoved(ETouchIndex::Type FingerIndex,  FVector Location);

private:
	FVector TouchStartPos;
	float TurnSpeed;
#pragma endregion InputTouch

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

	UFUNCTION(NetMulticast, Unreliable)
	void NetMult_PlayMontage(UAnimMontage* Montage);

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

	virtual void OnHealthUpdate() override;

	virtual void OnCharcterDied() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns QuestComponent **/
	FORCEINLINE class UQuestComponent* GetQuestComponent() const { return QuestComp; }

	void Died();

	float GetLevel();
	void SetLevel(const float NewLevel);

	const FName& GetCharacterName();

	UFUNCTION(Server, Reliable)
	void SetCharacterName(const FName& NewName);

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

#pragma region CombatSystem
public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "CombatSystem")
	void ServerSwitchState();

	//武器类的子类class软引用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatSystem")
	TMap<EWeaponType, FWeaponSofts> Map_WeaponClassPaths;

	/** 切换武器 */
	UFUNCTION(Server, Reliable)
	void ServerSwitchWeapon(EWeaponType WeaponType = EWeaponType::MainHandAndShied);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulSwitchWeapon(EWeaponType WeaponType);

	/** 循环切换武器 */
	void LoopSwitchWeapon();

	/** 客户端先执行攻击 */
	void AttackBtn_Down(EAttackType AttackType);
	/** 当攻击按键松开的时候 */
	void AttackBtn_Release(EAttackType AttackType);

	/** 同时通知服务器执行普通攻击 */
	UFUNCTION(Server, Reliable)
	void ServerAttackNotify(EAttackType AttackType);

	UFUNCTION(Server, Reliable)
	void Server_AttackBtn_Release(EAttackType AttackType);

	/** 通知修改攻击帧是否结束 */
	bool SetRWeaponFinishFrame(bool bFishFrame);

	/** 获取右手组件武器实例 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AEquipObject* GetRightWeaponInst();

	/** 获取左手组件武器实例 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AEquipObject* GetLeftWeaponInst();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	EWeaponType GetCurrentWeaponOutput();
	
	void SetCurrentWeaponType(EWeaponType NewWeaponType);

private:
	void NormalAttack();
	void NormalAttackBtn_Release();

	EWeaponType CurrentWeaponType;
	UPROPERTY()
	AEquipObject* LeftEquipObject;
#pragma endregion CombatSystem

#pragma region SpawnActor
public:
	bool TraceTarget(FVector& OutTarget, float MaxTraceLen = 2000.f);

	UFUNCTION(BlueprintCallable)
	void BowLaunchProjectile(TSubclassOf<AProjectileItem> SpawnClass);

	UFUNCTION(Server, Unreliable)
	void ServerLaunchProjectile(UClass* SpawnClass, FRotator Rotation, FVector Location, bool bAimView = false);

	/** 获得摄像机和箭的中间位置，从屏幕中间生成射线去检测 */
	FVector GetCameraArrowLevel();

	FVector CalcArrowVelocity(FVector StartLocation, FVector EndLocation, float Speed);

	UFUNCTION(NetMulticast, Unreliable)
	void MultSetArrowVelocity(AProjectileItem* SpawnedArrow, FVector TossVelocity);

#pragma endregion SpawnActor

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperty", meta = (AllowPrivateAccess = "true"), ReplicatedUsing = OnRep_CurrentHealth)
	FName CharacterName = "DarkLight";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerProperty", meta = (AllowPrivateAccess = "true"), ReplicatedUsing = OnRep_CurrentHealth)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> UIDiedClass;

	AMyCharacterBase* InteraCharacter;

	AActor* HitActor;

	//临时的背包
	TArray<AActor*> Inventory;

	FTimerHandle TraceTimerHandle;

};

