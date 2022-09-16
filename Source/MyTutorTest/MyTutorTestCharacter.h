// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/MyCharacterBase.h"
#include "MyTutorTestCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDamageSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FModifyPropSignature, float, const FText&);
DECLARE_DELEGATE(FInterativeSignature);


class UUIPlayerProperty;
class UUI_HUD;

UCLASS(config=Game)
class AMyTutorTestCharacter : public AMyCharacterBase
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

	/** 任务组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	class UQuestComponent* QuestComp;
public:
	AMyTutorTestCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

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

	//按E键进行交互，在需要交互的地方进行委托绑定
	void OnInteractiveInput();

	void OpenQuestUI();

	void SetNPCPtr(const AMyCharacterBase* Character);
	FORCEINLINE const AMyCharacterBase* GetNPCPtr() { return InteraCharacter; }

private:
	// TODO: ...ClampMax 如何设置为 ClampMax = MaxHP
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

	//保存一份使之交互NPC的对象指针，方便查询NPC身上的属性（在角色和NPC交互的的时候传入）
	AMyCharacterBase* InteraCharacter;
};

