// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkSplineVFX.h"

int ALinkSplineVFX::nextSplineID = 0;

// Sets default values
ALinkSplineVFX::ALinkSplineVFX()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALinkSplineVFX::BeginPlay()
{
	ThisSplineID = nextSplineID;
	nextSplineID++;
	Super::BeginPlay();
}

// Called every frame
void ALinkSplineVFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALinkSplineVFX::SetTarget1(AActor* target)
{
	Target1 = target;
	UpdateTarget1.Broadcast(Target1);
}

void ALinkSplineVFX::SetTarget2(AActor* target)
{
	Target2 = target;
	UpdateTarget2.Broadcast(Target2);
}

void ALinkSplineVFX::ResetID()
{
	nextSplineID = 0;
}

