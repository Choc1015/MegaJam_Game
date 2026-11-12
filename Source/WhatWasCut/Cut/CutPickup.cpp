#include "CutPickup.h"
#include "CutVisibilityComponent.h"
#include "WhatWasCutCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

ACutPickup::ACutPickup()
{
	PromptWord = TEXT("Take");
}

void ACutPickup::BeginPlay()
{
	Super::BeginPlay();
	HomeTransform = GetActorTransform();
}

void ACutPickup::Interact(AWhatWasCutCharacter* Player)
{
	if (!Player || bHeld)
	{
		return;
	}
	if (ACutPickup* Old = Player->GetHeldItem())
	{
		Old->ReturnHome();
	}
	Take(Player);
	ShowText(TextId);
}

void ACutPickup::Take(AWhatWasCutCharacter* Player)
{
	bHeld = true;
	CutVisibility->SetCarried(true);
	Mesh->SetSimulatePhysics(false);
	AttachToComponent(Player->GetFirstPersonCameraComponent(), FAttachmentTransformRules::KeepWorldTransform);
	SetActorRelativeLocation(FVector(55.f, 22.f, -22.f));
	SetActorRelativeRotation(FRotator::ZeroRotator);
	Player->SetHeldItem(this);
}

void ACutPickup::ReturnHome()
{
	bHeld = false;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorTransform(HomeTransform);
	CutVisibility->SetCarried(false);
}

void ACutPickup::Consume()
{
	bHeld = false;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Destroy();
}
