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

			Push(UIStatusBar);
		}

		if (UIHUDClass)
		{
			UIHUD = CreateWidget<UUI_HUD>(GetWorld(), UIHUDClass);
			if (UIHUD)
			{
				UIHUD->AddToViewport();
				
				Push(UIHUD, false);
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
	if (PlayerViewClass)
	{
		PlayerInfoView = CreateWidget<UUIPlayerInfoView>(GetWorld(), PlayerViewClass);
		if (PlayerInfoView)
		{
			PlayerInfoView->AddToViewport();
			
			Push(PlayerInfoView);
		}
	}
}

void AMyPlayerController::ClosePlayerInfoUI()
{
	if (PlayerInfoView && PlayerInfoView->IsInViewport())
	{
		Pop();
	}
}

void AMyPlayerController::ShowQuestMain(bool IsInteraction /*= false*/)
{
	if (BPQuestClass)
	{
		QuestUI = CreateWidget<UQuestMain>(GetWorld(), BPQuestClass);
		if (QuestUI)
		{
			QuestUI->AddToViewport();

			Push(QuestUI);
		}
	}
}

void AMyPlayerController::CloseQuestMain()
{
	if (QuestUI && QuestUI->IsInViewport())
	{
		Pop();
	}
}

void AMyPlayerController::SwitchQuestMain(AActor* Char, EActivateQuest NewWay)
{
	if (QuestUI && QuestUI->IsInViewport())
	{
		Pop();
	}
	else
	{
		if (BPQuestClass)
		{
			QuestUI = CreateWidget<UQuestMain>(GetWorld(), BPQuestClass);
			if (QuestUI)
			{
				QuestUI->AddToViewport();
				QuestUI->SetTargetObject(Char, NewWay);

				Push(QuestUI);
			}
		}
	}
}

void AMyPlayerController::Push(UUserWidget* Widget, bool Hidden, bool Throw)
{
	if (!UIStack.IsEmpty())
	{
		if (Throw)
		{
			UIStack.Pop()->RemoveFromParent();
		}
		else if (Hidden)
		{
			UIStack.Top()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (Widget != nullptr)
	{
		UIStack.Emplace(Widget);
	}
}

UUserWidget* AMyPlayerController::Pop()
{
	UUserWidget* TempWidget = nullptr;
	if (!UIStack.IsEmpty())
	{
		TempWidget = UIStack.Top();
		UIStack.Pop()->RemoveFromParent();
		if (UIStack.Num() != 0)
		{
			UIStack.Top()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	return TempWidget;
}

void AMyPlayerController::CloseAllWidget(int32 IndexFromTheStart /* = -1*/)
{
	int32 Index = IndexFromTheStart <= -1 ? 0 : IndexFromTheStart + 1;

	for (; Index < UIStack.Num(); ++Index)
	{
		UIStack[Index]->RemoveFromParent();
		UIStack.RemoveAt(Index);
	}
}

