// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATank::ATank()
{
	// Construct the spring arm and assign to the base pawn
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	// Construct the camera and assign to the spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the move forward so we can move the tank
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	if (TankPlayerController)
	{
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult
		);
				
		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// Access controller here!
	TankPlayerController = Cast<APlayerController>(GetController());

}

void ATank::Move(float Value)
{
	// Enable sweep in move
	bool bSweep = true;

	// Create a FVector
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value;

	// Use DeltaTime to scale for FPS
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	DeltaLocation.X *= DeltaTime;

	// Apply Speed multiplier
	DeltaLocation.X *= Speed;

	// Move tank
	AddActorLocalOffset(DeltaLocation, bSweep);
}

void ATank::Turn(float Value)
{
	// Enable sweep in rotation
	bool bSweep = true;

	// Create a FRotator
	FRotator RotatorLocation = FRotator::ZeroRotator;
	RotatorLocation.Yaw = Value;

	// Use DeltaTime to scale for FPS
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	RotatorLocation.Yaw *= DeltaTime;

	// Apply Speed multiplier
	RotatorLocation.Yaw *= TurnRate;

	// Rotate tank
	AddActorLocalRotation(RotatorLocation, bSweep);
}
