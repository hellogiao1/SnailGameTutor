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

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	UChildActorComponent* RightWeaponComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	UChildActorComponent* LeftWeaponComp;

protected:
	virtual void BeginPlay() override;

};
