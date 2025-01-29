// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"

#include "Components/BoxComponent.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Collider->SetCollisionEnabled(ECollisionEnabled::T);

}

void AGate::Toggle(bool bState)
{
	this->SetActorHiddenInGame(bState);

}


// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

