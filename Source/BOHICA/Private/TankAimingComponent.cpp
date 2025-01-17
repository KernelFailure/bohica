// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"



// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

 void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	 if (RoundsLeft <= 0) {
		 FiringState = EFiringState::OutOfAmmo;
	 } else if (FPlatformTime::Seconds() - LastFireTime < ReloadTimeInSeconds) {
		 FiringState = EFiringState::Reloading;
		 
	 } else if (IsBarrelMoving()) {
		 FiringState = EFiringState::Aiming;
		 
	 } else {
		 FiringState = EFiringState::Locked;
		 
	 }
 }

 void UTankAimingComponent::BeginPlay() {
	 LastFireTime = FPlatformTime::Seconds();
 }

bool UTankAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) {return false;}
	auto BarrelForwardVector = Barrel->GetForwardVector();
	
//	UE_LOG(LogTemp, Warning, TEXT("Barrel Forward Vector: %f - Aim Direction: %f"), *BarrelForwardVector, *AimDirection);
	return !BarrelForwardVector.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::AimAt(FVector HitLocation) {
	auto TankName = GetOwner()->GetName();
	auto BarrelLocation = Barrel->GetComponentLocation().ToString();
	

	if (!ensure(Barrel)) {return;}

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bHaveAimSolution) {
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}



void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	if (!ensure(Barrel && Turret)) {return;}
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	auto TurretRotator = Turret->GetForwardVector().Rotation();
	TurretRotator = AimAsRotator - TurretRotator;
	

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(TurretRotator.GetNormalized().Yaw);
	
}


void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::Fire() {
	
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
		//UE_LOG(LogTemp, Warning, TEXT("Called Firing!!"));
		if (!ensure(Barrel)) {return;}
		if (!ensure(ProjectileBlueprint)) {return;}
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
		);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const {
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const {
	return RoundsLeft;
}