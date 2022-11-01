// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../MyTutorTestCharacter.h"
#include "../Projectile/ProjectileItem.h"
#include "Blueprint/WidgetLayoutLibrary.h"


ABow::ABow()
{
	bArrowLoaded = false;
	bFiringArrow = false;
	bBowAiming = false;
	CanFire = false;

	RateTime = 2.f;

	//角色旋转
	bMoveView = false;
	TurnSpeed = -5.f;
}

void ABow::Excute_NormalAttack()
{
	if (bBowAiming)
	{
		//避免释放动画还没播放完就可以蓄力拉弹弓
		return;
	}

	StartMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	bMoveView = true;

	//锁定视角
	SetControlView(true);

	bBowAiming = true;
	bArrowLoaded = true;
	bFiringArrow = false;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &ABow::SetCanFire, RateTime, false);
	}
}

void ABow::OnNormalBtn_Release()
{
	if (CanFire == false)
	{
		//ABow();
		//这里调用这个构造函数会奔溃

		ResetValue();
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
		}
	}
	else
	{
		bArrowLoaded = false;
		bFiringArrow = true;
		CanFire = false;
		
		if (HasAuthority())
		{
			//这里踩了一个坑，如果是调用的UGamePlayStatics::GetPlayerCharacter()函数，在服务器上调用，会发现不是自己想要的角色
			AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(GetOwner());
			if (Tutor)
			{
				Tutor->BowLaunchProjectile(ArrowClass);
			}
		}

		//加一个定时器等射箭动作完成后进行初始化
		FTimerHandle RestTimeHandle;
		GetWorld()->GetTimerManager().SetTimer(RestTimeHandle, [&]() { bBowAiming = false; }, 0.6f, false);
	}

	SetControlView(false);
	bMoveView = false;
}

void ABow::SetCanFire()
{
	CanFire = true;
}

void ABow::ResetValue()
{
	bArrowLoaded = false;
	bFiringArrow = false;
	bBowAiming = false;
	CanFire = false;
}

void ABow::SetControlView(bool LockView)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = LockView;
		Character->GetCharacterMovement()->bOrientRotationToMovement = !LockView;
	}
}

void ABow::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABow, bArrowLoaded, COND_None);
	DOREPLIFETIME_CONDITION(ABow, bFiringArrow, COND_None);
	DOREPLIFETIME_CONDITION(ABow, bBowAiming, COND_None);
	DOREPLIFETIME_CONDITION(ABow, CanFire, COND_None);
}

void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoveView)
	{
		AMyTutorTestCharacter* Tutor = Cast<AMyTutorTestCharacter>(GetOwner());
		if (Tutor && Tutor->GetLocalRole() == ROLE_AutonomousProxy)
		{
			FVector2D CurrPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
			Tutor->AddControllerPitchInput((StartMousePos - CurrPos).Y * TurnSpeed * DeltaTime);
			Tutor->AddControllerYawInput((StartMousePos - CurrPos).X * TurnSpeed * DeltaTime);

			StartMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		}
	}
}

