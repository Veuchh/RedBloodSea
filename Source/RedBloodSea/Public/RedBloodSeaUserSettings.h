// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ScreenResolutionEnum.h"
#include "RedBloodSeaUserSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateCameraSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateAccessSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateGraphicsSettings);

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

	UFUNCTION(BlueprintCallable)
	void SetAccessibilitySettingsToDefaultValue();
	UFUNCTION(BlueprintCallable)
	void SetAudioSettingsToDefaultValue();
	UFUNCTION(BlueprintCallable)
	void SetGraphicsSettingsToDefaultValue();
	UFUNCTION(BlueprintCallable)
	void SetCameraSettingsToDefaultValue();
	
	UPROPERTY(Config, BlueprintReadWrite)
	bool Fullscreen;
	
	UFUNCTION(BlueprintCallable)
	bool GetDefaultFullscreen();

	UFUNCTION(BlueprintCallable)
	void SetFullscreen(bool value);
	
	UPROPERTY(Config, BlueprintReadWrite)
	EScreenResolution Resolution;

	UFUNCTION(BlueprintCallable)
	EScreenResolution GetDefaultRBSResolution();

	UFUNCTION(BlueprintCallable)
	void SetRBSResolution(EScreenResolution value);

	UPROPERTY(Config, BlueprintReadWrite)
	int Quality;

	UFUNCTION(BlueprintCallable)
	int GetDefaultQuality();

	UPROPERTY(Config, BlueprintReadWrite)
	float Brightness = 1.0f;

	UPROPERTY(Config, BlueprintReadWrite)
	float CameraSensitivity = 1;

	UPROPERTY(Config, BlueprintReadWrite)
	float MotionBlur;

	UPROPERTY(Config, BlueprintReadWrite)
	float FieldOfView = 105;

	UPROPERTY(Config, BlueprintReadWrite)
	bool Screenshake = true;

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
	bool UseCustomBloodColor;
	
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

	UPROPERTY(Config, BlueprintReadWrite)
	bool GodMode;

	UPROPERTY(Config, BlueprintReadWrite)
	float AimAssistStrength = 1.0f;

	UFUNCTION(BlueprintCallable)
	void UpdateCameraSettings();

	UFUNCTION(BlueprintCallable)
	void UpdateAccessSettings();

	UFUNCTION(BlueprintCallable)
	void UpdateGraphicsSettings();
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateCameraSettings OnUpdateCameraSettings;
	UPROPERTY(BlueprintAssignable)
	FOnUpdateAccessSettings OnUpdateAccessSettings;
	UPROPERTY(BlueprintAssignable)
	FOnUpdateGraphicsSettings OnUpdateGraphicsSettings;
};
