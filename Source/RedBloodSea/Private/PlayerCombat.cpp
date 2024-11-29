// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombat.h"

#include "Dweller.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerCombat::UPlayerCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlayerCombat::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* slashCollidersParent = static_cast<USceneComponent*>(slashCollidersParentReference.
		GetComponent(GetOwner()));

	// Check if the component is valid
	if (slashCollidersParent)
	{
		TArray<USceneComponent*> ChildComponents;
		slashCollidersParent->GetChildrenComponents(true, ChildComponents);
		// Get all child components of slashCollidersParent

		// Iterate through the child components
		for (USceneComponent* ChildComponent : ChildComponents)
		{
			UPrimitiveComponent* PrimitiveChild = Cast<UPrimitiveComponent>(ChildComponent);
			slashColliders.Add(PrimitiveChild);
		}
	}

	USceneComponent* thrustCollidersParent = static_cast<USceneComponent*>(thrustCollidersParentReference.
		GetComponent(GetOwner()));

	// Check if the component is valid
	if (thrustCollidersParent)
	{
		TArray<USceneComponent*> ChildComponents;
		thrustCollidersParent->GetChildrenComponents(true, ChildComponents);
		// Get all child components of slashCollidersParent

		// Iterate through the child components
		for (USceneComponent* ChildComponent : ChildComponents)
		{
			UPrimitiveComponent* PrimitiveChild = Cast<UPrimitiveComponent>(ChildComponent);
			thrustColliders.Add(PrimitiveChild);
		}
	}

	ToggleAttackCollider(BufferableAttack::Slash, false);
	ToggleAttackCollider(BufferableAttack::Thrust, false);

	PlayerData::MaxHPAmount = maxHPAmount;
	PlayerData::CurrentHPAmount = maxHPAmount;
	PlayerData::MaxAttackBufferCapacity = maxAttackBufferCapacity;
	PlayerData::SlashAttackStartupDelay = slashAttackStartupDelay;
	PlayerData::SlashAttackDuration = slashAttackDuration;
	PlayerData::SlashAttackCooldown = slashAttackCooldown;
	PlayerData::ThrustAttackStartupDelay = thrustAttackStartupDelay;
	PlayerData::ThrustAttackDuration = thrustAttackDuration;
	PlayerData::ThrustAttackCooldown = thrustAttackCooldown;
}

TArray<UPrimitiveComponent*> UPlayerCombat::GetAttackColliders(BufferableAttack attack)
{
	switch (attack)
	{
	case BufferableAttack::Slash:
		return slashColliders;
	case BufferableAttack::Thrust:
		return thrustColliders;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Tried to get colliders of invalid attack."));
		TArray<UPrimitiveComponent*> emptyArray;
		return emptyArray;
	}
}

float UPlayerCombat::GetAttackBufferCooldown(const BufferableAttack attack) const
{
	float attackCooldown = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	switch (attack)
	{
	case BufferableAttack::Slash:
		attackCooldown += slashInputBufferCooldown;
		break;
	case BufferableAttack::Thrust:
		attackCooldown += thrustInputBufferCooldown;
		break;
	}

	return attackCooldown;
}

void UPlayerCombat::CallAttackBlueprintCallback(const BufferableAttack attack)
{
	switch (attack)
	{
	case BufferableAttack::Slash:
		OnSlashStart.Broadcast();
		break;
	case BufferableAttack::Thrust:
		OnThrustStart.Broadcast();
		break;
	}
}

void UPlayerCombat::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	//We don't want the same attack hitting the same enemy twice
	// This is especially useful for attacks with multiple colliders
	if (currentAttackHitActors.Contains(OtherActor))
	{
		return;
	}

	currentAttackHitActors.Add(OtherActor);

	switch (PlayerData::CurrentAttack)
	{
	case BufferableAttack::Slash:
		OnSlashOverlap(HitComp, OtherActor);
		break;
	case BufferableAttack::Thrust:
		OnThrustOverlap(HitComp, OtherActor);
		break;
	}
}

void UPlayerCombat::OnSlashOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor)
{
	UWeakpointsManager* weakPointsManager = OtherActor->GetComponentByClass<UWeakpointsManager>();

	//An enemy was hit
	if (weakPointsManager)
	{
		weakPointsManager->RevealWeakpoints();
		OnSlashHitEnemy.Broadcast(OtherActor);
	}
	//The environment was hit
	else
	{
		OnSlashHitEnviro.Broadcast(OtherActor);
	}
}

void UPlayerCombat::OnThrustOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor)
{
	AActor* AttachedParent = OtherActor->GetAttachParentActor();

	//The environment was hit
	if (!AttachedParent)
	{
		FOnThrustHitEnviro.Broadcast(OtherActor);
		return;
	}

	UWeakpointsManager* weakPointsManager = AttachedParent->GetComponentByClass<UWeakpointsManager>();

	if (!weakPointsManager)
		return;

	if (OtherActor->IsA(AWeakpoint::StaticClass()))
	{
		weakPointsManager->RemoveWeakpoint(Cast<AWeakpoint>(OtherActor));
		OnThrustHitWeakpoint.Broadcast(Cast<AWeakpoint>(OtherActor));
	}
}


void UPlayerCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerData::CurrentAttack != BufferableAttack::None)
	{
		OngoingAttackLogic();
	}
	if (PlayerData::CanAttack())
	{
		TryConsumeAttackBuffer();
	}
}

//-----------------------------------------------------------------Input Buffer Logic
void UPlayerCombat::TryConsumeAttackBuffer()
{
	if (PlayerData::AttackBuffer.Num() <= 0)
		return;

	PlayerData::CurrentAttackState = PlayerAttackState::Startup;
	PlayerData::CurrentAttack = PlayerData::AttackBuffer[0];
	PlayerData::AttackBuffer.RemoveAt(0);
	PlayerData::AttackStartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	CallAttackBlueprintCallback(PlayerData::CurrentAttack);
}

void UPlayerCombat::OngoingAttackLogic()
{
	switch (PlayerData::CurrentAttackState)
	{
	case PlayerAttackState::Startup:
		if (PlayerData::GetCurrentAttackStartupEndTime() <= UGameplayStatics::GetRealTimeSeconds(GetWorld()))
		{
			PlayerData::CurrentAttackState = PlayerAttackState::Attacking;
			ToggleAttackCollider(PlayerData::CurrentAttack, true);
			return;
		}
		break;
	case PlayerAttackState::Attacking:
		if (PlayerData::GetCurrentAttackColliderEndTime() <= UGameplayStatics::GetRealTimeSeconds(GetWorld()))
		{
			PlayerData::CurrentAttackState = PlayerAttackState::Cooldown;
			ToggleAttackCollider(PlayerData::CurrentAttack, false);
			return;
		}
		break;
	case PlayerAttackState::Cooldown:
		if (PlayerData::GetCurrentAttackCooldownEndTime() <= UGameplayStatics::GetRealTimeSeconds(GetWorld()))
		{
			PlayerData::CurrentAttackState = PlayerAttackState::None;
			ToggleAttackCollider(PlayerData::CurrentAttack, false);
			return;
		}
		break;
	}
}

void UPlayerCombat::ToggleAttackCollider(BufferableAttack attack, bool isToggled)
{
	currentAttackHitActors.Empty();

	for (UPrimitiveComponent* collider : GetAttackColliders(attack))
	{
		collider->SetGenerateOverlapEvents(isToggled);
		collider->SetVisibility(isToggled);

		if (isToggled)
			collider->OnComponentBeginOverlap.AddDynamic(this, &UPlayerCombat::OnOverlapBegin);

		else
			collider->OnComponentBeginOverlap.RemoveDynamic(this, &UPlayerCombat::OnOverlapBegin);

		collider->UpdateOverlaps();
	}
}

void UPlayerCombat::TryAddAttackToBuffer(BufferableAttack attackToAdd)
{
	if (PlayerData::AttackBuffer.Num() >= PlayerData::MaxAttackBufferCapacity
		|| PlayerData::NextAllowedInputBufferTime > UGameplayStatics::GetRealTimeSeconds(GetWorld()))
		return;

	PlayerData::AttackBuffer.Add(attackToAdd);
	PlayerData::NextAllowedInputBufferTime = GetAttackBufferCooldown(attackToAdd);
}

//-----------------------------------------------------------------Inputs
void UPlayerCombat::OnSlashInput()
{
	TryAddAttackToBuffer(BufferableAttack::Slash);
}

void UPlayerCombat::OnThrustInput()
{
	TryAddAttackToBuffer(BufferableAttack::Thrust);
}

void UPlayerCombat::DamagePlayer(int damageAmount)
{
	PlayerData::CurrentHPAmount -= damageAmount;

	OnPlayerHit.Broadcast((float)PlayerData::CurrentHPAmount/PlayerData::MaxHPAmount);
}
