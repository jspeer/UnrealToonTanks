// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

// Forward declarations
class ATank;

UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	ATower();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handle tower destruction
	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ATank* Tank;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRange = 300.f;

	FTimerHandle FireRateTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRate = 2.f;

	void CheckFireCondition();
	bool InFireRange();
};
