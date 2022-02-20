// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"

#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATower::ATower()
{

}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if in range, rotate turrent towards tank
	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(
		FireRateTimerHandle,
		this,
		&ATower::CheckFireCondition,
		FireRate,
		true
	);
}

void ATower::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}

	// if in range, FIRE FIRE FIRE
	if (InFireRange() && Tank->bAlive)
	{
		Fire();
	}
}

bool ATower::InFireRange()
{
	if (Tank)
	{
		// find distance to tank
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		// check to see if tank is in range
		return Distance <= FireRange ? true : false;
	}

	// tank doesn't exist
	return false;
}
