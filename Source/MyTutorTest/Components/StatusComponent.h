// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Data/StatusAttributeType.h"
#include "StatusComponent.generated.h"

class UDatatable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTUTORTEST_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** FuncInterface  */
	const FBasicProperty& GetBasicPropertyByName(EBasicStatus BasicStatus) const;
	const FBasicProperty& GetBasicPropertyByName(FName BasicStatus) const;

	const FDetailProperty& GetDetailPropertyByName(EDetailStatus DetailStatus) const;
	const FDetailProperty& GetDetailPropertyByName(FName DetailStatus) const;

	void SetBasicProperty(EBasicStatus BasicStatus, float NewValue);
	void SetDetailProperty(EDetailStatus DetailStatus, float NewValue);
	void SetDetailMaxProperty(EDetailStatus DetailStatus, float NewValue, bool bResetCurrValue = false);

private:
	void InitAttribute();

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UDataTable* DTBaseStatus;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UDataTable* DTDetailStatus;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UDataTable* DTAttrMappingStatus;

	TMap<EBasicStatus, FBasicProperty> BasicPropertyMap;
	TMap<EDetailStatus, FDetailProperty> DetailPropertyMap;
	TArray<FAttributeMapping*> AttributeMappings;
};
