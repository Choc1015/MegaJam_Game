#include "CutVisibilityComponent.h"
#include "CutWorldSubsystem.h"
#include "GameFramework/Actor.h"

UCutVisibilityComponent::UCutVisibilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCutVisibilityComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UCutWorldSubsystem* Sub = UCutWorldSubsystem::Get(this))
	{
		Sub->RegisterVisibility(this);
		ApplyWorld(Sub->GetCurrentWorld());
	}
}

void UCutVisibilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UCutWorldSubsystem* Sub = UCutWorldSubsystem::Get(this))
	{
		Sub->UnregisterVisibility(this);
	}
	Super::EndPlay(EndPlayReason);
}

void UCutVisibilityComponent::SetCarried(bool bInCarried)
{
	bCarried = bInCarried;
	ApplyWorld(LastWorld);
}

void UCutVisibilityComponent::ApplyWorld(ECutWorld World)
{
	LastWorld = World;
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const bool bInThisWorld =
		Presence == ECutPresence::Both ||
		(Presence == ECutPresence::TheatricalOnly && World == ECutWorld::Theatrical) ||
		(Presence == ECutPresence::DirectorsOnly && World == ECutWorld::Directors);

	const bool bVisible = bInThisWorld || bCarried;
	Owner->SetActorHiddenInGame(!bVisible);
	Owner->SetActorEnableCollision(bInThisWorld && !bCarried);
}
