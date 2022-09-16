// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HUD.h"
#include "Components/Button.h"
#include "UIPlayerInfoView.h"
#include "Kismet/GameplayStatics.h"
#include "../MyPlayerController.h"

void UUI_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_PlayerInfo->OnClicked.AddDynamic(this, &UUI_HUD::OnCharInfoBtn_Down);
}

void UUI_HUD::OnCharInfoBtn_Down()
{
	AMyPlayerController* MyPlayController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayController)
	{
		MyPlayController->ShowPlayerInfoUI();
	}
}
