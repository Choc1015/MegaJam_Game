#pragma once

#include "CoreMinimal.h"
#include "CutInteractable.h"
#include "CutPickup.generated.h"

/** 들 수 있는 물건. 한 번에 하나만 들고, 다른 물건을 집으면 들고 있던 것은 원래 자리로 돌아간다. */
UCLASS()
class ACutPickup : public ACutInteractable
{
	GENERATED_BODY()

public:
	ACutPickup();

	/** 쓰는 대상이 맞는 물건인지 가려내는 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FName ItemId;

	virtual void Interact(AWhatWasCutCharacter* Player) override;

	/** 손에 든다 */
	void Take(AWhatWasCutCharacter* Player);
	/** 원래 자리로 돌려놓는다 */
	void ReturnHome();
	/** 다 쓰고 사라진다 */
	void Consume();

	bool IsHeld() const { return bHeld; }

protected:
	virtual void BeginPlay() override;

private:
	FTransform HomeTransform;
	bool bHeld = false;
};
