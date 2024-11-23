// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include <LightWeightInstanceBlueprintFunctionLibrary.generated.h>

#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Materials/MaterialExpressionSmoothStep.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundActor = FindCameraBoundsActor();
	if (CameraBoundActor != nullptr)
	{
		InitCameraBounds(CameraBoundActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain == nullptr) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();

	float InverseLerp = (GreatestDistanceBetweenTargets - CameraZoomDistanceBetweenTargetsMin) /
						(CameraZoomDistanceBetweenTargetsMax - CameraZoomDistanceBetweenTargetsMin);

	InverseLerp = FMath::Clamp(InverseLerp, 0.0f, 1.0f);

	FVector newLocation = CameraMain->GetComponentLocation();
	newLocation.Y = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, InverseLerp);
	CameraMain->SetWorldLocation(newLocation);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector newLocation = CameraMain->GetComponentLocation();
	newLocation.X = CalculateAveragePositionBetweenTargets().X;
	newLocation.Z = CalculateAveragePositionBetweenTargets().Z;
	ClampPositionIntoCameraBounds(newLocation);
	CameraMain->SetWorldLocation(newLocation);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector sumVector = FVector::ZeroVector;
	for (UObject* object : FollowTargets)
	{
		ICameraFollowTarget* FollowTarget = Cast<ICameraFollowTarget>(object);
		if (!FollowTarget) continue;
		if (!FollowTarget->IsFollowable()) continue;
		sumVector += FollowTarget->GetFollowPosition();
	}
	return sumVector / FollowTargets.Num();
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0;
	float DistanceHolder;
	for (int i = 0; i < FollowTargets.Num(); i++)
	{
		for (int e = i+1; e < FollowTargets.Num(); e++)
		{
			ICameraFollowTarget* FollowTargetI = Cast<ICameraFollowTarget>(FollowTargets[i]);
			if (!FollowTargetI) continue;
			ICameraFollowTarget* FollowTargetE = Cast<ICameraFollowTarget>(FollowTargets[e]);
			if (!FollowTargetE) continue;
			DistanceHolder = FVector::Dist(FollowTargetI->GetFollowPosition(), FollowTargetE->GetFollowPosition());

			if (DistanceHolder > GreatestDistance) GreatestDistance = DistanceHolder;
		}
	}
	return GreatestDistance;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(this->GetWorld(), Tag, FoundActors);

	return FoundActors[0] -> FindComponentByClass<UCameraComponent>();
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	for (TActorIterator<AActor> It(this->GetWorld()); It; ++It )
	{
		AActor* Actor = *It;
		if (Actor != nullptr && Actor->Tags.Contains("CameraBounds"))
		{
			return Actor;
		}
	}
	return nullptr;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	CameraBoundActor->GetActorBounds(false, BoundsCenter, BoundsExtents);

	CameraBoundsMin = FVector2d(BoundsCenter.X - BoundsExtents.X , BoundsCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2d(BoundsCenter.X + BoundsExtents.X , BoundsCenter.Z + BoundsExtents.Z);
	CameraBoundsYProjectionCenter = BoundsCenter.Y;	
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	float WorldBoundsWidth = abs(WorldBoundsMax.X - WorldBoundsMin.X);
	float WorldBoundsHeight = abs(WorldBoundsMax.Z - WorldBoundsMin.Z);
	
	if (Position.X < CameraBoundsMin.X + WorldBoundsWidth/2)
	{
		Position.X = CameraBoundsMin.X + WorldBoundsWidth/2;
	}
	if (Position.Z < CameraBoundsMin.Y + WorldBoundsHeight/2)
	{
		Position.Z = CameraBoundsMin.Y + WorldBoundsHeight/2;
	}
	
	if (Position.X > CameraBoundsMax.X - WorldBoundsWidth/2)
	{
 		Position.X = CameraBoundsMax.X - WorldBoundsWidth/2;
	}
	if (Position.Z > CameraBoundsMax.Y - WorldBoundsHeight/2)
	{
		Position.Z = CameraBoundsMax.Y - WorldBoundsHeight/2;
	}
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
		);
	FIntRect ViewPortRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportBoundsMin.X = ViewPortRect.Min.X;
	OutViewportBoundsMin.Y = ViewPortRect.Min.Y;

	OutViewportBoundsMax.X = ViewPortRect.Max.X;
	OutViewportBoundsMax.Y = ViewPortRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
		);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	for (TActorIterator<AActor> It(this->GetWorld()); It; ++It )
	{
		AActor* Actor = *It;
		if (Actor != nullptr) {
			if (Actor->Tags.Contains("CameraDistanceMin"))
			{
				CameraZoomYMin = Actor->GetActorLocation().Y;
			}
			if (Actor->Tags.Contains("CameraDistanceMax"))
			{
				CameraZoomYMax = Actor->GetActorLocation().Y;
			}
		}
	}
}




