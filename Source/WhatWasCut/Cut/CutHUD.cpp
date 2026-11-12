#include "CutHUD.h"
#include "CutWorldSubsystem.h"
#include "WhatWasCutCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"

void ACutHUD::DrawText2D(const FString& Text, float X, float Y, float Scale, const FLinearColor& Color, bool bCenterX)
{
	if (!Canvas || !GEngine)
	{
		return;
	}
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(Text), GEngine->GetMediumFont(), Color);
	Item.Scale = FVector2D(Scale, Scale);
	Item.bCentreX = bCenterX;
	Item.EnableShadow(FLinearColor(0.f, 0.f, 0.f, Color.A * 0.8f));
	Canvas->DrawItem(Item);
}

void ACutHUD::DrawHUD()
{
	Super::DrawHUD();
	UCutWorldSubsystem* Sub = UCutWorldSubsystem::Get(this);
	if (!Canvas || !Sub)
	{
		return;
	}

	const float W = Canvas->SizeX;
	const float H = Canvas->SizeY;
	const bool bDirectors = Sub->GetCurrentWorld() == ECutWorld::Directors;

	// 판 분위기 (11/14에 후처리로 교체): 극장판은 따뜻한 노란빛, 감독판은 색 빠진 푸른 회색과 검은 띠
	if (bDirectors)
	{
		DrawRect(FLinearColor(0.35f, 0.45f, 0.55f, 0.28f), 0.f, 0.f, W, H);
		const float Bar = H * 0.09f;
		DrawRect(FLinearColor::Black, 0.f, 0.f, W, Bar);
		DrawRect(FLinearColor::Black, 0.f, H - Bar, W, Bar);
	}
	else
	{
		DrawRect(FLinearColor(1.f, 0.78f, 0.35f, 0.10f), 0.f, 0.f, W, H);
	}

	// 가운데 점 + 상호작용 단어
	const AWhatWasCutCharacter* Player = Cast<AWhatWasCutCharacter>(GetOwningPawn());
	DrawRect(FLinearColor(1.f, 1.f, 1.f, 0.7f), W * 0.5f - 2.f, H * 0.5f - 2.f, 4.f, 4.f);
	if (Player && !Sub->IsTransitioning())
	{
		const FString Prompt = Player->GetFocusPrompt();
		if (!Prompt.IsEmpty())
		{
			DrawText2D(Prompt, W * 0.5f, H * 0.5f + 14.f, 1.0f, FLinearColor(1.f, 1.f, 1.f, 0.85f), true);
		}
	}

	// 자막 (화면 아래, 작은 글씨, 천천히 나타났다 사라짐)
	float Alpha = 0.f;
	FString Sub1 = Sub->GetSubtitle(Alpha);
	if (!Sub1.IsEmpty())
	{
		DrawText2D(Sub1, W * 0.5f, H * (bDirectors ? 0.80f : 0.86f), 1.1f, FLinearColor(1.f, 1.f, 1.f, Alpha), true);
	}

	// 구석 글자 (Theatrical Cut / Director's Cut)
	FString Cap = Sub->GetCaption(Alpha);
	if (!Cap.IsEmpty())
	{
		DrawText2D(Cap, W * 0.04f, H * (bDirectors ? 0.12f : 0.06f), 0.9f, FLinearColor(1.f, 1.f, 1.f, Alpha * 0.8f), false);
	}

	// TV 전환: 위아래 검은 막이 한 줄로 접히고, 한 줄이 꺼졌다 펴진다
	const float Closed = Sub->GetClosedAmount();
	if (Closed > 0.f)
	{
		const float Eased = Closed * Closed * (3.f - 2.f * Closed);
		const float Half = (H * 0.5f) * Eased;
		DrawRect(FLinearColor::Black, 0.f, 0.f, W, Half);
		DrawRect(FLinearColor::Black, 0.f, H - Half, W, Half);
		if (Closed > 0.85f)
		{
			const float Line = FMath::GetMappedRangeValueClamped(FVector2D(0.85f, 1.f), FVector2D(0.f, 1.f), Closed);
			DrawRect(FLinearColor(0.9f, 0.95f, 1.f, Line), W * 0.5f * (1.f - Line), H * 0.5f - 1.f, W * Line, 2.f);
			DrawRect(FLinearColor(0.f, 0.f, 0.f, Line * 0.6f), 0.f, 0.f, W, H);
		}
	}
}
