// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include <LightWeightInstanceBlueprintFunctionLibrary.generated.h>

#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

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
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if (Position.Z > CameraBoundsMin.Y + WorldBoundsHeight/2)
	{
		Position.Z = CameraBoundsMin.Y + WorldBoundsHeight/2;
	}
	if (Position.X > CameraBoundsMax.X - WorldBoundsWidth/2)
	{
 		Position.X = CameraBoundsMax.X - WorldBoundsWidth/2;
	}
	if (Position.Z < CameraBoundsMax.Y - WorldBoundsHeight/2)
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




