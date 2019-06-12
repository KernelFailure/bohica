// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"


class ATank;

UCLASS()
class BOHICA_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:
	float AcceptanceRadius = 3000;

	
};
