#pragma once

#include "CoreMinimal.h"
#include "CutInteractable.h"
#include "CutTV.generated.h"

/** 판을 오가는 TV. 한 판에 한 대씩 두고 서로를 Partner로 지정한다. 만지면 상대 TV 앞으로 건너간다. */
UCLASS()
class ACutTV : public ACutInteractable
{
	GENERATED_BODY()

public:
	ACutTV();

	/** 건너편 판의 TV */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Cut")
	TObjectPtr<ACutTV> Partner;

	virtual void Interact(AWhatWasCutCharacter* Player) override;
};
