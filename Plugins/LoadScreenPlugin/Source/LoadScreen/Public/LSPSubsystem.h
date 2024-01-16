// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSPSetting.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSPSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLsp, Log, All);

/**
 * 
 */
UCLASS()
class LOADSCREEN_API ULSPSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:


	void OpenLevelWithLoadScreen(FName MapPackageName);
	UFUNCTION(BlueprintCallable, DisplayName="OpenLevelWithLoadScreen", Category="LSP")
	void K2_OpenLevelWithLoadScreen(TSoftObjectPtr<UWorld> Map, TSoftObjectPtr<UWorld> TransitionMap, TSoftClassPtr<ULoadScreenUserWidget> LoadScreenUserWidgetClass);

	float GetLoadPercent();

protected:

	void OnCheckInTransitionMap();

	
protected:
	UPROPERTY()
	ULSPSetting* LspSetting;
	UPROPERTY()
	ULoadScreenUserWidget* LoadScreenUserWidget;

	FName LoadMapPackageName;
};
