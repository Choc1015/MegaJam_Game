#include "CutTV.h"
#include "CutWorldSubsystem.h"
#include "WhatWasCutCharacter.h"
#include "WhatWasCut.h"

ACutTV::ACutTV()
{
	PromptWord = TEXT("Touch");
}

void ACutTV::Interact(AWhatWasCutCharacter* Player)
{
	if (!Player)
	{
		return;
	}
	if (!Partner)
	{
		UE_LOG(LogWhatWasCut, Warning, TEXT("%s has no Partner TV"), *GetName());
		return;
	}
	if (UCutWorldSubsystem* Sub = UCutWorldSubsystem::Get(this))
	{
		Sub->BeginTransition(GetActorTransform(), Partner->GetActorTransform(), Player);
	}
}
