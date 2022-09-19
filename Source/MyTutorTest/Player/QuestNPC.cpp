// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNPC.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../MyTutorTestCharacter.h"
#include "../Components/QuestComponent.h"
#include "../UI/Quest/UINPCTip.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "../MyPlayerController.h"
#include "../UI/Quest/QuestMain.h"


AQuestNPC::AQuestNPC()
{
	HeadTipWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadTip"));
	HeadTipWidgetComp->SetupAttachment(RootComponent);
	HeadTipWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HeadTipWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(RootComponent);
	TextRender->SetText(FText::FromString("Press E Interaction"));
	TextRender->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	TextRender->SetHorizontalAlignment(EHTA_Center);
	TextRender->SetVerticalAlignment(EVRTA_TextTop);
	TextRender->SetVisibility(false);

	/*BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AQuestNPC::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AQuestNPC::OnBoxEndOverlap);*/
}

void AQuestNPC::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyTutorTestCharacter>())
	{
		if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			TextRender->SetVisibility(true);
			Cast<AMyTutorTestCharacter>(OtherActor)->InteractionEvent.BindUObject(this, &AQuestNPC::InteractionEvent);
			Cast<AMyTutorTestCharacter>(OtherActor)->SetNPCPtr(this);
		}
	}

}

void AQuestNPC::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMyTutorTestCharacter>())
	{
		if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			TextRender->SetVisibility(false);
			Cast<AMyTutorTestCharacter>(OtherActor)->InteractionEvent.Unbind();
			Cast<AMyTutorTestCharacter>(OtherActor)->SetNPCPtr(nullptr);

			//关闭UI窗口
			AMyPlayerController* LocalController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (LocalController)
			{
				LocalController->CloseQuestMain();
			}
		}
	}

}

void AQuestNPC::InitInteraction(AActor* Target)
{
	if (Target->IsA<AMyTutorTestCharacter>())
	{
		if (Target == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			TextRender->SetVisibility(true);
			Cast<AMyTutorTestCharacter>(Target)->InteractionEvent.BindUObject(this, &AQuestNPC::InteractionEvent);
			Cast<AMyTutorTestCharacter>(Target)->SetNPCPtr(this);
		}
	}
}

void AQuestNPC::EndInteraction(AActor* Target)
{
	if (Target->IsA<AMyTutorTestCharacter>())
	{
		if (Target == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			TextRender->SetVisibility(false);
			Cast<AMyTutorTestCharacter>(Target)->InteractionEvent.Unbind();
			Cast<AMyTutorTestCharacter>(Target)->SetNPCPtr(nullptr);

			//关闭UI窗口
			/*AMyPlayerController* LocalController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (LocalController)
			{
				LocalController->CloseQuestMain();
			}*/
		}
	}
}

void AQuestNPC::BeginPlay()
{
	Super::BeginPlay();

	//if (NPCQuests.IsEmpty()) return;
	//NPCQuests.Reset();

	CompareQuest();

}

void AQuestNPC::InteractionEvent()
{
	AMyPlayerController* LocalController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (LocalController)
	{
		LocalController->SwitchQuestMain(this, EActivateQuest::Other);
	}
}

void AQuestNPC::GetAcceptUnFinishQuests(TArray<FQuestDetail>& OutArray)
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	QuestComp->GetAcceptUnFinishQuest(OutArray, NPCQuests);
}

void AQuestNPC::GetUnAcceptQuests(TArray<FQuestDetail>& OutArray)
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	QuestComp->GetUnAcceptQuest(OutArray, NPCQuests);
}

void AQuestNPC::GetFinishQuests(TArray<FQuestDetail>& OutArray)
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	QuestComp->GetFinishQuestsForNPC(OutArray, NPCQuests);
}

void AQuestNPC::CompareQuest()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) 
		return;
	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) 
		return;

	if (QuestComp->ExistAcceptUnFinish(NPCQuests))
	{
		//如果npc身上有玩家接取但是未完成的任务,显示问号图标
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUINPCTip* NPCTip = Cast<UUINPCTip>(HeadTipWidgetComp->GetWidget());
			if (NPCTip)
			{
				NPCTip->SwitchImageTip(ETip::Question);
			}
		}
	}
	else if (QuestComp->ExistUnAcceptQuest(NPCQuests))
	{
		//如果npc身上有玩家未接取的任务,显示感叹号图标
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUINPCTip* NPCTip = Cast<UUINPCTip>(HeadTipWidgetComp->GetWidget());
			if (NPCTip)
			{
				NPCTip->SwitchImageTip(ETip::Exclamation);
			}
		}

	}
	else
	{
		//都不存在的情况就把UI隐藏
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUINPCTip* NPCTip = Cast<UUINPCTip>(HeadTipWidgetComp->GetWidget());
			if (NPCTip)
			{
				NPCTip->HideImageTip();
			}
		}
	}
}
