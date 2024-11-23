﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"

class UCameraComponent;
/**
 * 
 */
UCLASS()
class SMASHUE_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#pragma region Subsystem Overrides
	
public:
	virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;
	
#pragma endregion

#pragma region Main Camera

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	void TickUpdateCameraZoom(float DeltaTime);
	
	void TickUpdateCameraPosition(float DeltaTime);

	UCameraComponent* FindCameraByTag(const FName& Tag) const;
	
#pragma endregion

#pragma region Follow Targets
	
public:
	void AddFollowTarget(UObject* FollowTarget);

	void RemoveFollowTarget(UObject* FollowTarget);

protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;

	FVector CalculateAveragePositionBetweenTargets();

	float CalculateGreatestDistanceBetweenTargets();
	
#pragma endregion

#pragma region Misc

public:
	virtual TStatId GetStatId() const {return TStatId(); };
	
#pragma endregion

#pragma region Bounds

protected:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	float CameraBoundsYProjectionCenter;

	AActor* FindCameraBoundsActor();

	void InitCameraBounds(AActor* CameraBoundActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);
	
#pragma endregion 

#pragma region Zoom

	protected:
	UPROPERTY()
	float CameraZoomYMin = 0.0f;

	UPROPERTY()
	float CameraZoomYMax = 0.0f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMin = 300.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMax = 1500.f;

	UFUNCTION()
	void InitCameraZoomParameters();

#pragma endregion
};


