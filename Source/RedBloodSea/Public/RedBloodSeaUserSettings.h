// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RedBloodSeaUserSettings.generated.h"

UENUM(BlueprintType)
namespace EResolutions
{
	enum Type : int 
	{
		Native
	};
}

UENUM(BlueprintType)
namespace EGraphicsQuality
{
	enum Type : int
	{
		Ultra
	};
}

/**
 * 
 */
UCLASS()
class REDBLOODSEA_API URedBloodSeaUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

	public:
	//Get our game settings
	UFUNCTION(BlueprintCallable)
	static URedBloodSeaUserSettings* GetRedBloodSeaUserSettings();
	
	UPROPERTY(Config, BlueprintReadWrite)
	bool Fullscreen;

	UFUNCTION(BlueprintCallable)
	bool GetDefaultFullscreen();

	UPROPERTY(Config, BlueprintReadWrite)
	int Resolution;

	UFUNCTION(BlueprintCallable)
	int GetDefaultRBSResolution();

	UPROPERTY(Config, BlueprintReadWrite)
	int Quality;

	UFUNCTION(BlueprintCallable)
	int GetDefaultQuality();

	UPROPERTY(Config, BlueprintReadWrite)
	float Brightness;

	UFUNCTION(BlueprintCallable)
	float GetDefaultBrightness();

	UPROPERTY(Config, BlueprintReadWrite)
	float Contrast;

	UFUNCTION(BlueprintCallable)
	float GetDefaultContrast();

	UPROPERTY(Config, BlueprintReadWrite)
	float HorizontalSensitivity;

	UPROPERTY(Config, BlueprintReadWrite)
	float VerticalSensitivity;

	UPROPERTY(Config, BlueprintReadWrite)
	float MotionBlur;

	UPROPERTY(Config, BlueprintReadWrite)
	float FieldOfView;

	UPROPERTY(Config, BlueprintReadWrite)
	bool Screenshake;

	UPROPERTY(Config, BlueprintReadWrite)
	bool ZoomEffect;

	UPROPERTY(Config, BlueprintReadWrite)
	bool Reticle;

	UPROPERTY(Config, BlueprintReadWrite)
	float MasterVolume;

	UPROPERTY(Config, BlueprintReadWrite)
	float Music;

	UPROPERTY(Config, BlueprintReadWrite)
	float SoundEffects;

	UPROPERTY(Config, BlueprintReadWrite)
	float Ambiences;

	UPROPERTY(Config, BlueprintReadWrite)
	bool FrequencyFilterActivated;

	UPROPERTY(Config, BlueprintReadWrite)
	float FrequencyFilterTargetFrequency;

	UPROPERTY(Config, BlueprintReadWrite)
	bool MonoMode;

	UPROPERTY(Config, BlueprintReadWrite)
	float GameSpeed;

	UPROPERTY(Config, BlueprintReadWrite)
	bool Haptics;

	UPROPERTY(Config, BlueprintReadWrite)
	bool HighContrastMode;

	UPROPERTY(Config, BlueprintReadWrite)
	float BloodColorRed;

	UPROPERTY(Config, BlueprintReadWrite)
	float BloodColorGreen;

	UPROPERTY(Config, BlueprintReadWrite)
	float BloodColorBlue;

	UPROPERTY(Config, BlueprintReadWrite)
	int RespawnMode;

	UPROPERTY(Config, BlueprintReadWrite)
	int RapierPosition;

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetGraphicsSettingsToDefaults();
};
