// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPSubsystem.h"

#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogLsp);

void ULSPSubsystem::OpenLevelWithLoadScreen(FName MapPackageName)
{
	if (LoadMapPackageName != TEXT(""))
	{
		UE_LOG(LogLsp, Warning, TEXT("加载任务未完成禁止重复加载！"));
		return;
	}
	
	// if (!LspSetting)
	// {
	// 	LspSetting = NewObject<ULSPSetting>(this);
	// 	LspSetting->LoadConfig();
	// }
	// if (LspSetting->TransitionMap.IsNull())
	// {
	// 	UE_LOG(LogLsp, Warning, TEXT("项目配置->LoadingScreen->未配置过度地图"));
	// 	return;
	// }
	// if (LspSetting->LoadScreenUserWidgetClass.IsNull())
	// {
	// 	UE_LOG(LogLsp, Warning, TEXT("项目配置->LoadingScreen->未配置等待页面UI"));
	// 	return;
	// }
	// if (LspSetting->TransitionMap.GetLongPackageName() == MapPackageName.ToString())
	// {
	// 	UE_LOG(LogLsp, Warning, TEXT("加载目标地图禁止是漫游地图"));
	// 	return;
	// }
	LoadMapPackageName = MapPackageName;
	UGameplayStatics::OpenLevel(this, FName(LspSetting->TransitionMap.GetAssetName()));

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ULSPSubsystem::OnCheckInTransitionMap, 0.1);
}

void ULSPSubsystem::K2_OpenLevelWithLoadScreen(TSoftObjectPtr<UWorld> Map, TSoftObjectPtr<UWorld> TransitionMap, TSoftClassPtr<ULoadScreenUserWidget> LoadScreenUserWidgetClass)
{
	if (Map.IsNull())
	{
		UE_LOG(LogLsp, Warning, TEXT("地图资产不存在，请检查设置内容！"));
		return;
	}
	if (!LspSetting)
	{
		LspSetting = NewObject<ULSPSetting>(this);
		LspSetting->TransitionMap = TransitionMap;
		LspSetting->LoadScreenUserWidgetClass = LoadScreenUserWidgetClass;
	}
	OpenLevelWithLoadScreen(FName(Map.GetLongPackageName()));
}

float ULSPSubsystem::GetLoadPercent()
{
	if (LoadMapPackageName.IsNone())
	{
		return 0;
	}
	float Percent = GetAsyncLoadPercentage(LoadMapPackageName);
	return Percent == -1 ? 0 : Percent / 100;
}

void ULSPSubsystem::OnCheckInTransitionMap()
{
	if (UGameplayStatics::GetCurrentLevelName(this) != LspSetting->TransitionMap.GetAssetName())
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ULSPSubsystem::OnCheckInTransitionMap, 0.1);
		return;
	}
	
	if (!LoadScreenUserWidget)
	{
		FStreamableManager m_streamableManager;
		TSubclassOf<ULoadScreenUserWidget> LsClass = m_streamableManager.LoadSynchronous<ULoadScreenUserWidget>(LspSetting->LoadScreenUserWidgetClass);
		LoadScreenUserWidget = CreateWidget<ULoadScreenUserWidget>(GetGameInstance(), LsClass);
	}
	if (LoadScreenUserWidget)
	{
		LoadScreenUserWidget->AddToViewport();
	}
	
	//启动异步加载目标地图
	LoadPackageAsync(LoadMapPackageName.ToString(), FLoadPackageAsyncDelegate::CreateLambda([this](const FName& PackageName, UPackage* /*LoadedPackage*/, EAsyncLoadingResult::Type Result/*Result*/)
	{
		if (Result == EAsyncLoadingResult::Succeeded)
		{
			UGameplayStatics::OpenLevel(this, PackageName);
			LoadMapPackageName = NAME_None;
			LoadScreenUserWidget->RemoveFromParent();
		}
	}));
}
