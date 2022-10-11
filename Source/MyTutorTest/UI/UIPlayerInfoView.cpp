// Fill out your copyright notice in the Description page of Project Settings.


#include "UIPlayerInfoView.h"
#include "Kismet/GameplayStatics.h"
#include "../MyTutorTestCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../MyPlayerController.h"

void UUIPlayerInfoView::NativeConstruct()
{
	Super::NativeConstruct();

	MyTestCharcter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyTestCharcter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, MyTestcharcter is nullptr"), __FUNCTION__);
		return;
	}

	ProgressBar_Health->SetPercent(MyTestCharcter->GetCurrentHealth() / MyTestCharcter->GetMaxHealth());
	EditTexBx_Name->SetText(FText::FromName(MyTestCharcter->GetCharacterName()));

	Btn_ChangeName->OnClicked.AddDynamic(this, &UUIPlayerInfoView::OnBtnChangeName_Down);
	Btn_Close->OnClicked.AddDynamic(this, &UUIPlayerInfoView::OnBtnClose_Down);

	MyTestCharcter->OnPropertyChange.AddUObject(this, &UUIPlayerInfoView::ChangeUIProperty);
}

void UUIPlayerInfoView::OnBtnChangeName_Down()
{
	if (!EditTexBx_Name->GetText().IsEmpty())
	{
		MyTestCharcter->SetCharacterName(*EditTexBx_Name->GetText().ToString());
	}
	OnBtnClose_Down();
}

void UUIPlayerInfoView::OnBtnClose_Down()
{
	AMyPlayerController* MyPlayController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayController)
	{
		MyPlayController->ClosePlayerInfoUI();
	}
}

void UUIPlayerInfoView::SetHealthPercentage(const float Health)
{
	ProgressBar_Health->SetPercent(Health);
}

void UUIPlayerInfoView::SetEditName(const FText& Text)
{
	EditTexBx_Name->SetText(Text);
}

void UUIPlayerInfoView::ChangeUIProperty(float Health, const FText& Text)
{
	SetHealthPercentage(Health);
	SetEditName(Text);
}
