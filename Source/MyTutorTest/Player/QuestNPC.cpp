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

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AQuestNPC::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AQuestNPC::OnBoxEndOverlap);
}

void AQuestNPC::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyTutorTestCharacter>())
	{
		TextRender->SetVisibility(true);
		Cast<AMyTutorTestCharacter>(OtherActor)->InteractionEvent.BindUObject(this, &AQuestNPC::InteractionEvent);
		Cast<AMyTutorTestCharacter>(OtherActor)->SetNPCPtr(this);
	}

}

void AQuestNPC::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMyTutorTestCharacter>())
	{
		TextRender->SetVisibility(false);
		Cast<AMyTutorTestCharacter>(OtherActor)->InteractionEvent.Unbind();
		Cast<AMyTutorTestCharacter>(OtherActor)->SetNPCPtr(nullptr);

		//�ر�UI����
		AMyPlayerController* LocalController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (LocalController)
		{
			LocalController->CloseQuestMain();
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
		AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MyCharacter == nullptr) return;

		UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
		if (QuestComp == nullptr) return;

		//ÿ�ν�����ʱ��ȥ��������ϲ�ѯδ��ȡ������
		QuestComp->GetUnAcceptQuest(UnAcceptQuest, NPCQuests);
		LocalController->InitQuestMain(UnAcceptQuest);
	}
}

void AQuestNPC::CompareQuest()
{
	AMyTutorTestCharacter* MyCharacter = Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter == nullptr) return;

	UQuestComponent* QuestComp = MyCharacter->GetQuestComponent();
	if (QuestComp == nullptr) return;

	if (QuestComp->GetAcceptAndUnComplete(AcceptUnCompletes, NPCQuests))
	{
		//���npc��������ҽ�ȡ����δ��ɵ�����,��ʾ�ʺ�ͼ��
		if (HeadTipWidgetComp && HeadTipWidgetComp->GetWidget())
		{
			UUINPCTip* NPCTip = Cast<UUINPCTip>(HeadTipWidgetComp->GetWidget());
			if (NPCTip)
			{
				NPCTip->SwitchImageTip(ETip::Question);
			}
		}
	}
	else if (QuestComp->GetUnAcceptQuest(UnAcceptQuest, NPCQuests))
	{
		//���npc���������δ��ȡ������,��ʾ��̾��ͼ��
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
		//�������ڵ�����Ͱ�UI����
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
