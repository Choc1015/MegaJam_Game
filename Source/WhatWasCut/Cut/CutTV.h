#pragma once

#include "CoreMinimal.h"
#include "CutInteractable.h"
#include "CutTV.generated.h"

class ALight;
class UMaterialInstanceDynamic;
class ULightComponent;

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

	/** 화면 빛을 내는 라이트(선택). 화면을 끌 때 같이 꺼진다. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Cut")
	TObjectPtr<ALight> GlowLight;

	/** 화면 머티리얼 슬롯 번호 (SM_TV_CRT_v2d는 1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	int32 ScreenSlot = 1;

	/** 화면 머티리얼의 밝기 파라미터 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	FName BrightnessParam = TEXT("Brightness");

	/** 화면이 꺼지거나 켜지는 데 걸리는 시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cut")
	float FadeTime = 1.2f;

	/** 화면을 켜거나 끈다. 천천히 어두워진다. 끄면 판 전환도 막힌다. */
	UFUNCTION(BlueprintCallable, Category = "Cut")
	void SetScreenOn(bool bOn);

	/** 결말에서 부른다. 이후 E를 누르면 건너가는 대신 화면을 끈다. */
	UFUNCTION(BlueprintCallable, Category = "Cut")
	void AllowTurnOff();

	UFUNCTION(BlueprintPure, Category = "Cut")
	bool IsScreenOn() const { return bScreenOn; }

	virtual void Interact(AWhatWasCutCharacter* Player) override;
	virtual FString GetPrompt(const AWhatWasCutCharacter* Player) const override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void ApplyLevel(float Level);

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> ScreenMID;

	bool bScreenOn = true;
	bool bTurnOffAllowed = false;
	/** 0=꺼짐, 1=켜짐 */
	float Level = 1.f;
	float BaseBrightness = 1.f;
	float BaseLightIntensity = 0.f;
};
