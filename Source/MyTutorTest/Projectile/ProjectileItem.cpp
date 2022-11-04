// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Functionality/MyGameplayStatic.h"
#include "Blueprint/UserWidget.h"
#include "../MyTutorTestCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/DamageType.h"

// Sets default values
AProjectileItem::AProjectileItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = CollisionComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CollisionComp);
	StaticMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	StaticMesh->SetRelativeLocation(FVector(-90.f, 0.f, 0.f));
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMove->SetUpdatedComponent(CollisionComp);
	ProjectileMove->InitialSpeed = 0.f;
	ProjectileMove->MaxSpeed = 0.f;
	ProjectileMove->ProjectileGravityScale = 0.5;
	ProjectileMove->bRotationFollowsVelocity = true;
	ProjectileMove->bShouldBounce = false;

	bReplicates = true;

	DestroyTime = 5.0f;
	ProjectileDamage = 20.f;
}

void AProjectileItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AProjectileItem, ProjectileVelocity, COND_None);
}

// Called when the game starts or when spawned
void AProjectileItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			World->GetTimerManager().SetTimer(DestroyTimeHandle, this, &AProjectileItem::OnActorDestroyed, DestroyTime, false);
		}
	}
}

// Called every frame
void AProjectileItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHitActor == false && HasAuthority())
	{
		TraceObject();
	}
}

void AProjectileItem::SetVelocity(FVector NewVelocity)
{
	ProjectileVelocity = NewVelocity;
	OnRep_ProjectileVelocity();
}

void AProjectileItem::Init(bool bMaxPath, FRotator EndRotation, FVector EndLocation)
{
	bReachMaxPoint = bMaxPath;
	RequireRotation = EndRotation;
	RequireLocation = EndLocation;
}

void AProjectileItem::TraceObject()
{
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = StartLoc + GetActorForwardVector() * 40.f;
	FHitResult OutHit;

	//自己添加的ObjectChannel对应着的 ETraceTypeQuery 是从数字7开始的
	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {GetOwner()}, EDrawDebugTrace::None, OutHit, true);
	if (bHit)
	{
		if (UMyGameplayStatic::IsTeam(GetOwner(), OutHit.GetActor()))
		{
			return;
		}

		if (OutHit.GetActor()->IsA<AProjectileItem>())
		{
			return;
		}

		if (OutHit.BoneName == TEXT("None"))
		{
			RemoveCollisionAttach(OutHit.GetComponent());
		}
		else
		{
			AttachtoEnemyHit(OutHit.GetComponent(), OutHit.BoneName);
		}

		if (ProjectileMove)
		{
			ProjectileMove->StopMovementImmediately();
		}

		TriggerHit(OutHit);

		bHitActor = true;
	}
	else
	{
		//达到最远的点，则旋转抛射物为瞄准的方向
		if (bReachMaxPoint && GetActorLocation().Equals(RequireLocation, 20.f))
		{
			if (ProjectileMove)
			{
				ProjectileMove->ProjectileGravityScale = 0.3;
				ProjectileMove->Velocity = RequireRotation.Vector() * ProjectileMove->Velocity.Size();
			}
			MultiAdjustment(0.3, RequireRotation.Vector() * ProjectileMove->Velocity.Size());
		}
	}
}

void AProjectileItem::MultStopMovement_Implementation()
{
	if (ProjectileMove)
	{
		ProjectileMove->StopMovementImmediately();
	}
}

void AProjectileItem::TriggerHit(const FHitResult& OutHit)
{

}

void AProjectileItem::OnActorDestroyed()
{
	Destroy();
}

float AProjectileItem::ApplyDamage(AActor* HitActor, float DamageAmount)
{
	if (HitActor && (DamageAmount != 0.f))
	{
		// make sure we have a good damage type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		return HitActor->TakeDamage(DamageAmount, DamageEvent, nullptr, GetOwner());
	}

	return 0.f;
}

bool AProjectileItem::IsArrowStuckInWall(const FVector& ImpactNormal)
{
	float DP = FVector::DotProduct(ImpactNormal, GetActorForwardVector());
	float AngleWall = (180.0) / DOUBLE_PI * FMath::Acos(DP) - 90.f;
	if (AngleWall >= 8.f)
	{
		if (ProjectileMove)
		{
			ProjectileMove->StopMovementImmediately();
		}
		return true;
	}
	else
	{
		return false;
	}
}

void AProjectileItem::RemoveCollisionAttach_Implementation(USceneComponent* Parent)
{
	if (ProjectileMove)
	{
		ProjectileMove->StopMovementImmediately();
	}
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
}

void AProjectileItem::AttachtoEnemyHit_Implementation(USceneComponent* Parent, const FName& SocketName)
{
	if (ProjectileMove)
	{
		ProjectileMove->StopMovementImmediately();
	}

	StaticMesh->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform, SocketName);

	//生成HitMarker动画
	AMyTutorTestCharacter* CurrChar = Cast<AMyTutorTestCharacter>(GetOwner());
	if (CurrChar && CurrChar->GetLocalRole() == ROLE_AutonomousProxy)
	{
		if (HitMarkerClass != nullptr)
		{
			UUserWidget* HitMarker = CreateWidget<UUserWidget>(GetWorld(), HitMarkerClass);
			if (HitMarker)
			{
				HitMarker->AddToViewport();
			}
		}
	}
}

void AProjectileItem::OnRep_ProjectileVelocity()
{
	if (ProjectileMove)
	{
		ProjectileMove->Velocity = ProjectileVelocity;
	}
}

void AProjectileItem::MultiAdjustment_Implementation(float Gravity, FVector Velocity)
{
	if (ProjectileMove)
	{
		ProjectileMove->ProjectileGravityScale = Gravity;
		ProjectileMove->Velocity = Velocity;
	}
}

