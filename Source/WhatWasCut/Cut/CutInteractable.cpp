#include "CutInteractable.h"
#include "CutVisibilityComponent.h"
#include "CutWorldSubsystem.h"
#include "Components/StaticMeshComponent.h"

ACutInteractable::ACutInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));

	CutVisibility = CreateDefaultSubobject<UCutVisibilityComponent>(TEXT("CutVisibility"));
}

void ACutInteractable::Interact(AWhatWasCutCharacter* Player)
{
	ShowText(TextId);
}

void ACutInteractable::ShowText(FName Id) const
{
	if (UCutWorldSubsystem* Sub = UCutWorldSubsystem::Get(this))
	{
		Sub->ShowSubtitle(Sub->GetText(Id));
	}
}
