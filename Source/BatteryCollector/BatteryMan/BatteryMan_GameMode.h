// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryMan_GameMode.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ABatteryMan_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	ABatteryMan_GameMode();
public:
	// Public Functions Declaration
	//---------------------------------------------------------
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SpawnPlayerRecharge();

	//variables
	//---------------------------------------------------------
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PlayerRecharge;

	float Spawn_Z = 500.0f;

	UPROPERTY(EditAnywhere)
	float Spawn_X_Min;

	UPROPERTY(EditAnywhere)
	float Spawn_X_Max;

	UPROPERTY(EditAnywhere)
	float Spawn_Y_Min;

	UPROPERTY(EditAnywhere)
	float Spawn_Y_Max;
};