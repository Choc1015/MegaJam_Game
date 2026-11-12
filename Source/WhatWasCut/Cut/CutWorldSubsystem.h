#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CutTypes.h"
#include "CutWorldSubsystem.generated.h"

class UCutVisibilityComponent;
class APawn;

/** 판 상태, 판 전환 연출, 글 데이터를 맡는다. */
UCLASS()
class UCutWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UCutWorldSubsystem* Get(const UObject* WorldContext);

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	ECutWorld GetCurrentWorld() const { return CurrentWorld; }

	void RegisterVisibility(UCutVisibilityComponent* Comp);
	void UnregisterVisibility(UCutVisibilityComponent* Comp);

	bool IsTransitioning() const { return Phase != EPhase::None; }

	/** TV 전환. FromAnchor 기준의 플레이어 상대 위치를 ToAnchor 기준으로 옮긴다. */
	void BeginTransition(const FTransform& InFromAnchor, const FTransform& InToAnchor, APawn* Pawn);

	/** 0=열림, 1=한 줄로 닫힘 (HUD가 그린다) */
	float GetClosedAmount() const;

	/** 글 데이터(Content/Data/CutText.csv). "Id.T"/"Id.D"가 있으면 판별 글을 우선한다. */
	FString GetText(FName Id) const;

	/** 화면 아래 자막 (한두 줄) */
	void ShowSubtitle(const FString& Text, float Duration = 4.f);
	/** 구석의 작은 글자 */
	void ShowCaption(const FString& Text, float Duration = 2.f);

	/** 현재 자막과 투명도(0~1). 없으면 빈 문자열 */
	FString GetSubtitle(float& OutAlpha) const;
	FString GetCaption(float& OutAlpha) const;

private:
	enum class EPhase : uint8 { None, Closing, Opening };

	struct FTimedText
	{
		FString Text;
		double Start = 0.0;
		float Duration = 0.f;
	};

	void LoadTextData();
	void ApplyAll();
	void CompleteSwitch();
	static FString ReadTimedText(const FTimedText& T, double Now, float& OutAlpha);

	ECutWorld CurrentWorld = ECutWorld::Theatrical;
	bool bDirectorsVisited = false;

	TArray<TWeakObjectPtr<UCutVisibilityComponent>> Registered;
	TMap<FName, FString> TextData;

	EPhase Phase = EPhase::None;
	float PhaseTime = 0.f;
	FTransform FromAnchor;
	FTransform ToAnchor;
	TWeakObjectPtr<APawn> TransitionPawn;

	FTimedText Subtitle;
	FTimedText Caption;

	static constexpr float CloseDuration = 0.4f;
	static constexpr float OpenDuration = 0.6f;
};
