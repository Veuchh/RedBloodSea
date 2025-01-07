// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBloodSeaUserSettings.h"

URedBloodSeaUserSettings::URedBloodSeaUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Fullscreen = false;
	Resolution = 0;
	Quality = 0;
	Brightness = 0;
	Contrast = 0;
	HorizontalSensitivity = 1;
	VerticalSensitivity = 1;
	MotionBlur = 1;
	FieldOfView = 70;
	Screenshake = true;
	ZoomEffect = true;
	Reticle = true;
	MasterVolume = 80;
	Music = 50;
	SoundEffects = 80;
	Ambiences = 40;
	FrequencyFilterActivated = false;
	FrequencyFilterTargetFrequency = 5000;
	MonoMode = false;
	GameSpeed = 100;
	Haptics = true;
	HighContrastMode = false;
	BloodColorRed = 255;
	BloodColorGreen = 0;
	BloodColorBlue = 100;
	RespawnMode = 0;
	RapierPosition = 0;
}

URedBloodSeaUserSettings* URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()
{
	return Cast<URedBloodSeaUserSettings>(URedBloodSeaUserSettings::GetGameUserSettings());
}

bool URedBloodSeaUserSettings::GetDefaultFullscreen()
{
	return true;
}

int URedBloodSeaUserSettings::GetDefaultRBSResolution()
{
	return 0;
}

int URedBloodSeaUserSettings::GetDefaultQuality()
{
	return 0;
}

float URedBloodSeaUserSettings::GetDefaultBrightness()
{
	return 50.0f;
}

float URedBloodSeaUserSettings::GetDefaultContrast()
{
	return 50.0f;
}

void URedBloodSeaUserSettings::SetGraphicsSettingsToDefaults()
{
	Fullscreen = GetDefaultFullscreen();
	Resolution = GetDefaultRBSResolution();
	Quality = GetDefaultQuality();
	Brightness = GetDefaultBrightness();
	Contrast = GetDefaultContrast();


	if (Fullscreen) {
		SetFullscreenMode(EWindowMode::Fullscreen);
	}
	else {
		SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	
	switch (Resolution) {
	case 0: SetScreenResolution(FIntPoint(1920, 1080));
	}

	//Quality Setting Missing !!!

	//Brightness Setting Missing !!!

	//Contrast Setting Missing !!!
}
