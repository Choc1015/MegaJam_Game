#pragma once

#include "CoreMinimal.h"
#include "CutInteractable.h"
#include "CutUseTarget.generated.h"

/** 물건을 쓰는 대상 (약장 열기, 궤 열기, 머리맡에 약 두기). 손에 든 게 없으면 그냥 살펴본다. */
UCLASS()
class ACutUseTarget : public ACutInteractable
{
	GENERATED_BODY()

public:
	ACutUseTarget();

	/** 이 대상이 받는 물건의 ItemId. None이면 아무 물건도 받지 않는다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FName AcceptItemId;

	/** 맞는 물건을 썼을 때의 글 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FName SuccessTextId;

	/** 틀린 물건을 썼을 때의 글 ID (실패 상태는 없다. 글만 뜬다) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FName WrongTextId;

	/** 맞는 물건을 쓰면 그 물건은 사라진다 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	bool bConsumeItem = true;

	/** 한 번 풀리면 다시 쓸 수 없다 */
	UPROPERTY(BlueprintReadOnly, Category = "Cut")
	bool bSolved = false;

	/** 풀렸을 때 블루프린트에서 문, 약장, 빛 같은 변화를 넣는다 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Cut")
	void OnSolved();

	virtual void Interact(AWhatWasCutCharacter* Player) override;
	virtual FString GetPrompt(const AWhatWasCutCharacter* Player) const override;
};
