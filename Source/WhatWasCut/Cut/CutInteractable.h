#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutTypes.h"
#include "CutInteractable.generated.h"

class UStaticMeshComponent;
class UCutVisibilityComponent;
class AWhatWasCutCharacter;

/**
 * E로 상호작용하는 물체의 기본형. 기본 동작은 "살펴보기/읽기": TextId의 글을 자막으로 띄운다.
 * 글은 Content/Data/CutText.csv에서 읽는다 (Id.T / Id.D 가 있으면 판별 글을 우선).
 */
UCLASS()
class ACutInteractable : public AActor
{
	GENERATED_BODY()

public:
	ACutInteractable();

	/** 화면 가운데 점 옆에 뜨는 한 단어 (Look, Read, Take, Use, Touch) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FString PromptWord = TEXT("Look");

	/** CutText.csv의 글 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FName TextId;

	/** 플레이어가 E를 눌렀을 때. Character는 지금 손에 든 물건을 알고 있다. */
	virtual void Interact(AWhatWasCutCharacter* Player);

	/** 지금 상태에서 화면에 띄울 한 단어 */
	virtual FString GetPrompt(const AWhatWasCutCharacter* Player) const { return PromptWord; }

	UCutVisibilityComponent* GetCutVisibility() const { return CutVisibility; }

protected:
	void ShowText(FName Id) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCutVisibilityComponent> CutVisibility;
};
