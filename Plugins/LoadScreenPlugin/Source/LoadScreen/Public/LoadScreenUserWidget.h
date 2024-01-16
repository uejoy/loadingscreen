// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LOADSCREEN_API ULoadScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	
	virtual void OnReceiveLoadPercent(float Percent);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnReceiveLoadPercent")
	void K2_OnReceiveLoadPercent(float Percent);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
