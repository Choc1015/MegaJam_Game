#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CutHUD.generated.h"

/** 가운데 점, 한 단어, 아래 자막, 구석 글자, 판 분위기(색·검은 띠), TV 전환 연출을 그린다. */
UCLASS()
class ACutHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	void DrawText2D(const FString& Text, float X, float Y, float Scale, const FLinearColor& Color, bool bCenterX);
};
