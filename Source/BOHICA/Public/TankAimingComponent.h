// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOHICA_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

private:	
	UStaticMeshComponent* Barrel = nullptr;


public:	

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AimAt(FVector HitLocation, float LaunchSpeed);

	// Sets default values for this component's properties
	UTankAimingComponent();

	void setBarrelReference(UStaticMeshComponent* BarrelToSet);
		
};