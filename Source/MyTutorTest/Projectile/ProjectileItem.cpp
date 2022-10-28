// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectileItem::AProjectileItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileItem::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileItem::OnHitOverlap);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CollisionComp);

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

// Called when the game starts or when spawned
void AProjectileItem::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(DestroyTimeHandle, this, &AProjectileItem::OnActorDestroyed, DestroyTime, false);
	}
	
}

void AProjectileItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(DestroyTimeHandle);
	}
	TriggerHit();
}

void AProjectileItem::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AProjectileItem::TriggerHit()
{

}

void AProjectileItem::OnActorDestroyed()
{
	Destroy();
}

bool AProjectileItem::IsTeam(AActor* OtherActor)
{
	AActor* MyInstigator = GetOwner();
	if (MyInstigator && OtherActor)
	{
		for (const auto& Name : MyInstigator->Tags)
		{
			if (OtherActor->Tags.Contains(Name))
			{
				return true;
			}
		}
	}
	return false;
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

// Called every frame
void AProjectileItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

