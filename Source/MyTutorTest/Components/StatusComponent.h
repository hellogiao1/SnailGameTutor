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

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UDataTable* DTBaseStatus;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UDataTable* DTDetailStatus;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UDataTable* DTAttrMappingStatus;

	TArray<FBasicProperty*> BasicPropertys;
	TArray<FDetailProperty*> DetailPropertys;
	TArray<FAttributeMapping*> AttributeMappings;
};
