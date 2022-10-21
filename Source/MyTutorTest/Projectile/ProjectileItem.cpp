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
	ProjectileMove->InitialSpeed = 3000.f;
	ProjectileMove->MaxSpeed = 3000.f;
	ProjectileMove->bRotationFollowsVelocity = true;
	ProjectileMove->bShouldBounce = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AProjectileItem::BeginPlay()
{
	Super::BeginPlay();


	
}

void AProjectileItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void AProjectileItem::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// Called every frame
void AProjectileItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

