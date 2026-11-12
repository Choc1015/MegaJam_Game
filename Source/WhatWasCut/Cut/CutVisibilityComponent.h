#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CutTypes.h"
#include "CutVisibilityComponent.generated.h"

/** 액터에 붙이면 판에 따라 보이기와 충돌을 켜고 끈다. */
UCLASS(ClassGroup = (Cut), meta = (BlueprintSpawnableComponent))
class UCutVisibilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCutVisibilityComponent();

	/** 이 물체가 있는 판 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	ECutPresence Presence = ECutPresence::Both;

	/** 손에 들려 있는 동안: 어느 판에서나 보이고 충돌은 끈다 */
	void SetCarried(bool bInCarried);
	bool IsCarried() const { return bCarried; }

	void ApplyWorld(ECutWorld World);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool bCarried = false;
	ECutWorld LastWorld = ECutWorld::Theatrical;
};
