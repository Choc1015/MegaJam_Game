#pragma once

#include "CoreMinimal.h"
#include "CutTypes.generated.h"

/** 지금 보고 있는 판 */
UENUM(BlueprintType)
enum class ECutWorld : uint8
{
	Theatrical UMETA(DisplayName = "Theatrical Cut"),
	Directors  UMETA(DisplayName = "Directors Cut")
};

/** 물체가 어느 판에 있는가 */
UENUM(BlueprintType)
enum class ECutPresence : uint8
{
	Both           UMETA(DisplayName = "Both"),
	TheatricalOnly UMETA(DisplayName = "Theatrical only"),
	DirectorsOnly  UMETA(DisplayName = "Directors only")
};
