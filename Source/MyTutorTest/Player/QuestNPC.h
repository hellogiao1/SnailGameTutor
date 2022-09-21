// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "../Data/QuestData.h"
#include "../Interface/Interaction.h"
#include "QuestNPC.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AQuestNPC : public AMyCharacterBase, public IInteraction
{
	GENERATED_BODY()

	/** Follow Head Tip */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HeadTipWidgetComp;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collish", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RenderText", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* TextRender;
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Quest")
	TArray<FQuestDetail> NPCQuests;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "NPC")
	FText NPCName;

public:
	AQuestNPC();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void InitInteraction(AActor* Target) override;

	virtual void EndInteraction(AActor* Target) override;

	UFUNCTION()
	void InteractionEvent();

	void GetAcceptUnFinishQuests(TArray<FQuestDetail>& OutArray);
	void GetUnAcceptQuests(TArray<FQuestDetail>& OutArray);
	void GetFinishQuests(TArray<FQuestDetail>& OutArray);

	//通过对比任务显示tip提示
	void CompareQuest();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "UIClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUIAcceptQuest> BPAcceptQuestClass;

	UPROPERTY()
	class UUIAcceptQuest* AcceptUI;

};
