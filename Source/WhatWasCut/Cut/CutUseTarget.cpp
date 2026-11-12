#include "CutUseTarget.h"
#include "CutPickup.h"
#include "WhatWasCutCharacter.h"

ACutUseTarget::ACutUseTarget()
{
	PromptWord = TEXT("Look");
}

FString ACutUseTarget::GetPrompt(const AWhatWasCutCharacter* Player) const
{
	if (!bSolved && !AcceptItemId.IsNone() && Player && Player->GetHeldItem())
	{
		return TEXT("Use");
	}
	return PromptWord;
}

void ACutUseTarget::Interact(AWhatWasCutCharacter* Player)
{
	ACutPickup* Held = Player ? Player->GetHeldItem() : nullptr;
	if (bSolved || AcceptItemId.IsNone() || !Held)
	{
		ShowText(TextId);
		return;
	}

	if (Held->ItemId == AcceptItemId)
	{
		bSolved = true;
		if (bConsumeItem)
		{
			Player->SetHeldItem(nullptr);
			Held->Consume();
		}
		ShowText(SuccessTextId);
		OnSolved();
	}
	else
	{
		ShowText(WrongTextId);
	}
}
