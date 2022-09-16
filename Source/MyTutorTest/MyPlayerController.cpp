// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyTutorTestCharacter.h"
#include "UI/UI_PlayerProperty.h"
#include "UI/UI_HUD.h"
#include "UI/UIPlayerInfoView.h"
#include "UI/Quest/QuestMain.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TutorChar = Cast<AMyTutorTestCharacter>(GetCharacter());
	if (TutorChar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TutorChar is nullptr"));
		return;
	}

	if (!HasAuthority())
	{
		if (UIStatusClass)
		{
			UIStatusBar = CreateWidget<UUIPlayerProperty>(GetWorld(), UIStatusClass);
			if (UIStatusBar)
			{
				UIStatusBar->SetCharacterName(FText::FromName(TutorChar->GetCharacterName()));
				UIStatusBar->SetHealthPercentage(TutorChar->GetHP() / TutorChar->GetMaxHP());
				UIStatusBar->AddToViewport();
			}
			TutorChar->OnCharHit.AddUObject(UIStatusBar, &UUIPlayerProperty::SetHealthPercentage);
			TutorChar->OnPropertyChange.AddUObject(UIStatusBar, &UUIPlayerProperty::SetCharProperty);
		}

		if (UIHUDClass)
		{
			UIHUD = CreateWidget<UUI_HUD>(GetWorld(), UIHUDClass);
			if (UIHUD)
			{
				UIHUD->AddToViewport();
			}
		}

		SetShowMouseCursor(true);
	}

}

UUIPlayerProperty* AMyPlayerController::GetUIStatusBar()
{
	return UIStatusBar;
}

void AMyPlayerController::ShowPlayerInfoUI()
{
	if (UIHUD && UIHUD->IsInViewport())
	{
		UIHUD->RemoveFromParent();
	}

	if (PlayerViewClass)
	{
		PlayerInfoView = CreateWidget<UUIPlayerInfoView>(GetWorld(), PlayerViewClass);
		if (PlayerInfoView)
		{
			PlayerInfoView->AddToViewport();
		}
	}
}

void AMyPlayerController::ClosePlayerInfoUI()
{
	if (PlayerInfoView && PlayerInfoView->IsInViewport())
	{
		PlayerInfoView->RemoveFromParent();
	}

	if (UIHUDClass)
	{
		UIHUD = CreateWidget<UUI_HUD>(GetWorld(), UIHUDClass);
		if (UIHUD)
		{
			UIHUD->AddToViewport();
		}
	}
}

void AMyPlayerController::ShowQuestMain(bool IsInteraction /*= false*/)
{
	if (BPQuestClass)
	{
		QuestUI = CreateWidget<UQuestMain>(GetWorld(), BPQuestClass);
		if (QuestUI)
		{
			if (IsInteraction)
			{
				QuestUI->InteractionView();
			}
			QuestUI->AddToViewport();
		}
	}
}

void AMyPlayerController::CloseQuestMain()
{
	if (QuestUI && QuestUI->IsInViewport())
	{
		QuestUI->RemoveFromParent();
	}
}

void AMyPlayerController::SwitchQuestMain(bool IsInteraction /*= false*/)
{
	if (QuestUI && QuestUI->IsInViewport())
	{
		QuestUI->RemoveFromParent();
	}
	else
	{
		ShowQuestMain(IsInteraction);
	}
}

void AMyPlayerController::InitQuestMain(const TArray<FQuestDetail>& UnAccept)
{
	if (QuestUI && QuestUI->IsInViewport())
	{
		QuestUI->RemoveFromParent();
	}
	else
	{
		if (BPQuestClass)
		{
			QuestUI = CreateWidget<UQuestMain>(GetWorld(), BPQuestClass);
			if (QuestUI)
			{
				QuestUI->AddToViewport();
				QuestUI->InteractionView();
				QuestUI->InitUnAcceptQuest(UnAccept);
			}
		}
	}
}
