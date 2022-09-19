// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/QuestData.h"
#include "UI/Quest/QuestMain.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	class UUIPlayerProperty* GetUIStatusBar();

	/* �����Ϣ����*/
	void ShowPlayerInfoUI();
	void ClosePlayerInfoUI();

	/*���������غ���*/
	void ShowQuestMain(bool IsInteraction = false);
	void CloseQuestMain();
	void SwitchQuestMain(AActor* Char, EActivateQuest NewWay);

	/** ����׷�ٽ���*/
	void ShowOnProgressQuest(int32 ID = -1);
	void CloseProgressQuest();
	
public:
	void Push(UUserWidget* Widget = nullptr, bool Hidden = true, bool Throw = false);

	UUserWidget* Pop();

	void SwapUI(UUserWidget* Widget);

	void CloseAllWidget(int32 IndexFromTheStart = -1);

private:
	//TODO дһ��UI����ջ��ѹջ��ջ
	TArray<UUserWidget*> UIStack;

private:
	UPROPERTY(EditAnywhere, Category = "UIClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> UIHUDClass;

	UPROPERTY(EditAnywhere, Category = "UIClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> UIStatusClass;

	UPROPERTY(EditAnywhere, Category = "UIClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUIPlayerInfoView> PlayerViewClass;

	UPROPERTY(EditAnywhere, Category = "UIClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UQuestMain> BPQuestClass;

	UPROPERTY(EditAnywhere, Category = "UIClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUIOnProgressTipBar> BPProgressClass;

	class UUIPlayerInfoView* PlayerInfoView;

	class UUIPlayerProperty* UIStatusBar;

	class UUI_HUD* UIHUD;

	class AMyTutorTestCharacter* TutorChar;

	class UQuestMain* QuestUI;

	class UUIOnProgressTipBar* ProgressUI;
};
