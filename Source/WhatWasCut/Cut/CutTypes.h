#pragma once

#include "CoreMinimal.h"
#include "CutTypes.generated.h"

/** 지금 보고 있는 판 */
UENUM(BlueprintType)
enum class ECutWorld : uint8
{
	Theatrical UMETA(DisplayName = "극장판 (TV 너머, 끝난 집)"),
	Directors  UMETA(DisplayName = "감독판 (지금의 집, 시작)")
};

/** 물체가 어느 판에 있는가 */
UENUM(BlueprintType)
enum class ECutPresence : uint8
{
	Both           UMETA(DisplayName = "둘 다"),
	TheatricalOnly UMETA(DisplayName = "극장판만"),
	DirectorsOnly  UMETA(DisplayName = "감독판만")
};
