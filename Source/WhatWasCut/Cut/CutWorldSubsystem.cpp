#include "CutWorldSubsystem.h"
#include "CutVisibilityComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "WhatWasCut.h"

UCutWorldSubsystem* UCutWorldSubsystem::Get(const UObject* WorldContext)
{
	const UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
	return World ? World->GetSubsystem<UCutWorldSubsystem>() : nullptr;
}

bool UCutWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

TStatId UCutWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCutWorldSubsystem, STATGROUP_Tickables);
}

void UCutWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadTextData();
}

void UCutWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	ApplyAll();
}

// ---- 글 데이터 -------------------------------------------------------------

void UCutWorldSubsystem::LoadTextData()
{
	TextData.Reset();
	const FString Path = FPaths::ProjectContentDir() / TEXT("Data/CutText.csv");
	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *Path))
	{
		UE_LOG(LogWhatWasCut, Warning, TEXT("CutText.csv not found: %s"), *Path);
		return;
	}

	for (const FString& Raw : Lines)
	{
		FString Line = Raw.TrimStartAndEnd();
		if (Line.IsEmpty() || Line.StartsWith(TEXT("#")))
		{
			continue;
		}
		FString Id, Text;
		if (!Line.Split(TEXT(","), &Id, &Text))
		{
			continue;
		}
		Id.TrimStartAndEndInline();
		if (Id.Equals(TEXT("ID"), ESearchCase::IgnoreCase))
		{
			continue; // header
		}
		Text.TrimStartAndEndInline();
		if (Text.Len() >= 2 && Text.StartsWith(TEXT("\"")) && Text.EndsWith(TEXT("\"")))
		{
			Text = Text.Mid(1, Text.Len() - 2).Replace(TEXT("\"\""), TEXT("\""));
		}
		Text = Text.Replace(TEXT("\\n"), TEXT("\n"));
		TextData.Add(FName(*Id), Text);
	}
	UE_LOG(LogWhatWasCut, Log, TEXT("CutText: %d rows"), TextData.Num());
}

FString UCutWorldSubsystem::GetText(FName Id) const
{
	if (Id.IsNone())
	{
		return FString();
	}
	const FName Suffixed(*(Id.ToString() + (CurrentWorld == ECutWorld::Theatrical ? TEXT(".T") : TEXT(".D"))));
	if (const FString* Found = TextData.Find(Suffixed))
	{
		return *Found;
	}
	if (const FString* Found = TextData.Find(Id))
	{
		return *Found;
	}
	return FString();
}

// ---- 판 상태 ---------------------------------------------------------------

void UCutWorldSubsystem::RegisterVisibility(UCutVisibilityComponent* Comp)
{
	Registered.AddUnique(Comp);
}

void UCutWorldSubsystem::UnregisterVisibility(UCutVisibilityComponent* Comp)
{
	Registered.Remove(Comp);
}

void UCutWorldSubsystem::ApplyAll()
{
	for (int32 i = Registered.Num() - 1; i >= 0; --i)
	{
		if (UCutVisibilityComponent* Comp = Registered[i].Get())
		{
			Comp->ApplyWorld(CurrentWorld);
		}
		else
		{
			Registered.RemoveAtSwap(i);
		}
	}
}

// ---- 전환 ------------------------------------------------------------------

void UCutWorldSubsystem::BeginTransition(const FTransform& InFromAnchor, const FTransform& InToAnchor, APawn* Pawn)
{
	if (IsTransitioning() || !Pawn)
	{
		return;
	}
	FromAnchor = InFromAnchor;
	ToAnchor = InToAnchor;
	TransitionPawn = Pawn;
	Phase = EPhase::Closing;
	PhaseTime = 0.f;
	if (AController* C = Pawn->GetController())
	{
		C->SetIgnoreMoveInput(true);
		C->SetIgnoreLookInput(true);
	}
}

void UCutWorldSubsystem::CompleteSwitch()
{
	if (APawn* Pawn = TransitionPawn.Get())
	{
		const FVector Rel = FromAnchor.InverseTransformPosition(Pawn->GetActorLocation());
		const FVector NewLoc = ToAnchor.TransformPosition(Rel);
		const float YawDelta = ToAnchor.Rotator().Yaw - FromAnchor.Rotator().Yaw;

		Pawn->TeleportTo(NewLoc, Pawn->GetActorRotation(), false, true);
		if (AController* C = Pawn->GetController())
		{
			FRotator R = C->GetControlRotation();
			R.Yaw += YawDelta;
			C->SetControlRotation(R);
		}
	}

	CurrentWorld = (CurrentWorld == ECutWorld::Theatrical) ? ECutWorld::Directors : ECutWorld::Theatrical;
	ApplyAll();

	// 시작은 감독판(지금의 집). TV 너머 극장판으로 처음 건너갈 때만 글자 카드
	if (CurrentWorld == ECutWorld::Theatrical && !bDirectorsVisited)
	{
		bDirectorsVisited = true;
		ShowCaption(TEXT("Theatrical Cut"), 2.f);
	}
}

void UCutWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Phase == EPhase::None)
	{
		return;
	}

	PhaseTime += DeltaTime;
	if (Phase == EPhase::Closing && PhaseTime >= CloseDuration)
	{
		CompleteSwitch();
		Phase = EPhase::Opening;
		PhaseTime = 0.f;
	}
	else if (Phase == EPhase::Opening && PhaseTime >= OpenDuration)
	{
		Phase = EPhase::None;
		if (APawn* Pawn = TransitionPawn.Get())
		{
			if (AController* C = Pawn->GetController())
			{
				C->SetIgnoreMoveInput(false);
				C->SetIgnoreLookInput(false);
			}
		}
		TransitionPawn.Reset();
	}
}

float UCutWorldSubsystem::GetClosedAmount() const
{
	switch (Phase)
	{
	case EPhase::Closing: return FMath::Clamp(PhaseTime / CloseDuration, 0.f, 1.f);
	case EPhase::Opening: return 1.f - FMath::Clamp(PhaseTime / OpenDuration, 0.f, 1.f);
	default: return 0.f;
	}
}

// ---- 자막 ------------------------------------------------------------------

void UCutWorldSubsystem::ShowSubtitle(const FString& Text, float Duration)
{
	if (Text.IsEmpty() || !GetWorld())
	{
		return;
	}
	Subtitle.Text = Text;
	Subtitle.Start = GetWorld()->GetTimeSeconds();
	Subtitle.Duration = Duration;
}

void UCutWorldSubsystem::ShowCaption(const FString& Text, float Duration)
{
	if (Text.IsEmpty() || !GetWorld())
	{
		return;
	}
	Caption.Text = Text;
	Caption.Start = GetWorld()->GetTimeSeconds();
	Caption.Duration = Duration;
}

FString UCutWorldSubsystem::ReadTimedText(const FTimedText& T, double Now, float& OutAlpha)
{
	OutAlpha = 0.f;
	if (T.Text.IsEmpty())
	{
		return FString();
	}
	const float Age = static_cast<float>(Now - T.Start);
	if (Age < 0.f || Age > T.Duration)
	{
		return FString();
	}
	const float FadeIn = 0.8f, FadeOut = 0.8f;
	OutAlpha = FMath::Min(FMath::Clamp(Age / FadeIn, 0.f, 1.f), FMath::Clamp((T.Duration - Age) / FadeOut, 0.f, 1.f));
	return T.Text;
}

FString UCutWorldSubsystem::GetSubtitle(float& OutAlpha) const
{
	return ReadTimedText(Subtitle, GetWorld()->GetTimeSeconds(), OutAlpha);
}

FString UCutWorldSubsystem::GetCaption(float& OutAlpha) const
{
	return ReadTimedText(Caption, GetWorld()->GetTimeSeconds(), OutAlpha);
}
