// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadScreenUserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "UObject/NoExportTypes.h"
#include "LSPSetting.generated.h"

/**
 * 
 */
UCLASS()
class LOADSCREEN_API ULSPSetting : public UObject
{
	GENERATED_BODY()

public:
	//配置切换过度地图，尽量是空白地图
	UPROPERTY(EditAnywhere, Category="LSP")
	TSoftObjectPtr<UWorld> TransitionMap;
	//配置切换过度UI，用于展示过度进度信息，需要重写
	UPROPERTY(EditAnywhere, Category="LSP")
	TSoftClassPtr<ULoadScreenUserWidget> LoadScreenUserWidgetClass;
};
