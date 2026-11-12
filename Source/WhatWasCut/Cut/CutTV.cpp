#include "CutTV.h"
#include "CutWorldSubsystem.h"
#include "WhatWasCutCharacter.h"
#include "WhatWasCut.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "Engine/Light.h"
#include "Materials/MaterialInstanceDynamic.h"

ACutTV::ACutTV()
{
	PromptWord = TEXT("Touch");
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ACutTV::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh && Mesh->GetNumMaterials() > ScreenSlot)
	{
		ScreenMID = Mesh->CreateAndSetMaterialInstanceDynamic(ScreenSlot);
		if (ScreenMID)
		{
			float Value = 1.f;
			if (ScreenMID->GetScalarParameterValue(FMaterialParameterInfo(BrightnessParam), Value))
			{
				BaseBrightness = Value;
			}
		}
	}
	if (GlowLight && GlowLight->GetLightComponent())
	{
		BaseLightIntensity = GlowLight->GetLightComponent()->Intensity;
	}
}

void ACutTV::ApplyLevel(float InLevel)
{
	if (ScreenMID)
	{
		ScreenMID->SetScalarParameterValue(BrightnessParam, BaseBrightness * InLevel);
	}
	if (GlowLight && GlowLight->GetLightComponent())
	{
		GlowLight->GetLightComponent()->SetIntensity(BaseLightIntensity * InLevel);
	}
}

void ACutTV::SetScreenOn(bool bOn)
{
	bScreenOn = bOn;
	SetActorTickEnabled(true);
}

void ACutTV::AllowTurnOff()
{
	bTurnOffAllowed = true;
}

void ACutTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Target = bScreenOn ? 1.f : 0.f;
	Level = FMath::FInterpConstantTo(Level, Target, DeltaTime, 1.f / FMath::Max(FadeTime, KINDA_SMALL_NUMBER));
	ApplyLevel(Level);
	if (FMath::IsNearlyEqual(Level, Target))
	{
		SetActorTickEnabled(false);
	}
}

FString ACutTV::GetPrompt(const AWhatWasCutCharacter* Player) const
{
	if (bTurnOffAllowed && bScreenOn)
	{
		return TEXT("Turn off");
	}
	return Super::GetPrompt(Player);
}

void ACutTV::Interact(AWhatWasCutCharacter* Player)
{
	if (!Player)
	{
		return;
	}
	// 결말: 건너가는 대신 화면을 끈다.
	if (bTurnOffAllowed)
	{
		if (bScreenOn)
		{
			SetScreenOn(false);
			if (Partner)
			{
				Partner->SetScreenOn(false);
			}
		}
		return;
	}
	if (!bScreenOn)
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
