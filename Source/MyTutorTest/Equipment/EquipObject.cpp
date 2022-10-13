// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "../Player/MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "../MyTutorTestCharacter.h"

// Sets default values
AEquipObject::AEquipObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	//设置重叠事件为false，在需要的子类中打开即可
	Capsule->SetGenerateOverlapEvents(false);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEquipObject::OnHitActor);

	AttackValue = 0.f;
	IsAttacking = false;
	CanCombo = false;
	bHitFrameFinish = false;

	//bReplicates = true;
}

// Called when the game starts or when spawned
void AEquipObject::BeginPlay()
{
	Super::BeginPlay();
	

}

void AEquipObject::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AEquipObject::PlayMontage_Internal(int32 Index)
{
	if (UAnimInstance* AnimInstance = GetAnimInst())
	{
		
		if (AnimInstance && AttackMontages.IsValidIndex(Index) && !AnimInstance->Montage_IsPlaying(AttackMontages[Index]))
		{
			Cast<AMyTutorTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->NetMult_PlayMontage(AttackMontages[Index]);
			//NetMult_PlayMontage(AnimInstance);

			MontageEndedDelegate.BindUObject(this, &AEquipObject::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontages[Index]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Play Attack Montage Failed"));
	}
	
}

void AEquipObject::NetMult_PlayMontage_Implementation(UAnimInstance* AnimInst)
{
	if (AnimInst)
	{
		AnimInst->Montage_Play(AttackMontages[CurrPlayAnimMont_Index]);
	}
}

// Called every frame
void AEquipObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipObject::SetGenerateOverlap(bool bInGeneOverlap)
{
	if (Capsule)
	{
		Capsule->SetGenerateOverlapEvents(bInGeneOverlap);
	}
}

void AEquipObject::SetCanCombo(bool canCombo)
{
	CanCombo = canCombo;
}

void AEquipObject::SetbHitFrameFinish(bool NewValue)
{
	bHitFrameFinish = NewValue;
}

void AEquipObject::Excute_NormalAttack()
{
	if (AttackMontages.IsEmpty() || GetAnimInst() == nullptr)
	{
		return;
	}

	if (!IsAttacking)
	{
		IsAttacking = true;

		CurrPlayAnimMont_Index = 0;

		PlayMontage_Internal(CurrPlayAnimMont_Index);
	}
	else if (bHitFrameFinish)
	{
		//当攻击帧结束，且可以连击的时候
		if (CanCombo)
		{
			CanCombo = false;


			++CurrPlayAnimMont_Index;
			if (CurrPlayAnimMont_Index >= AttackMontages.Num())
			{
				CurrPlayAnimMont_Index = 0;
			}

			NormalCombo();

			PlayMontage_Internal(CurrPlayAnimMont_Index);
		}
	}
}

void AEquipObject::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ResetAttackMontValue();
}

void AEquipObject::ResetAttackMontValue()
{
	CanCombo = false;
	IsAttacking = false;
	CurrPlayAnimMont_Index = 0;
	bHitFrameFinish = false;
}

UAnimInstance* AEquipObject::GetAnimInst()
{
	USkeletalMeshComponent* Mesh = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) ? UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh() : nullptr;
	return Mesh ? Mesh->GetAnimInstance() : nullptr;
}

