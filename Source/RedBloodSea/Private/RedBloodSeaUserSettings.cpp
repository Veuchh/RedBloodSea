// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBloodSeaUserSettings.h"

URedBloodSeaUserSettings::URedBloodSeaUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetAccessibilitySettingsToDefaultValue();
	SetAudioSettingsToDefaultValue();
	SetGraphicsSettingsToDefaultValue();
	SetCameraSettingsToDefaultValue();
}

URedBloodSeaUserSettings* URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()
{
	return Cast<URedBloodSeaUserSettings>(URedBloodSeaUserSettings::GetGameUserSettings());
}

void URedBloodSeaUserSettings::SetAccessibilitySettingsToDefaultValue()
{	
	AimAssistStrength = 1.0f;
	RapierPosition = 0;
	GodMode = false;
	RespawnMode = 0;
	GameSpeed = 1.f;
	Haptics = true;
	HighContrastMode = false;
	UseCustomBloodColor = false;
	BloodColorRed = 74.0f;
	BloodColorGreen = 3.0f;
	BloodColorBlue = 6.0f;
}

void URedBloodSeaUserSettings::SetAudioSettingsToDefaultValue()
{
	MasterVolume = 80.0f;
	Music = 50.0f;
	SoundEffects = 80.0f;
	Ambiences = 40.0f;
	FrequencyFilterActivated = false;
	FrequencyFilterTargetFrequency = 5000.0f;
	MonoMode = false;
}

void URedBloodSeaUserSettings::SetGraphicsSettingsToDefaultValue()
{
	Fullscreen = true;
	Resolution = EScreenResolution::Option1;
	Quality = 0;
	Brightness = 1.0f;
	Contrast = 1.0f;
	Reticle = true;
}

void URedBloodSeaUserSettings::SetCameraSettingsToDefaultValue()
{
	CameraSensitivity = 1.0f;
	MotionBlur = 1.0f;
	FieldOfView = 105.0f;
	Screenshake = true;
	ZoomEffect = true;
}

bool URedBloodSeaUserSettings::GetDefaultFullscreen()
{
	return true;
}

void URedBloodSeaUserSettings::SetFullscreen(bool value)
{
	switch (value)
	{
		case true: SetFullscreenMode(EWindowMode::Fullscreen);
		break;
		case false: SetFullscreenMode(EWindowMode::Windowed); break;
	}
	Fullscreen = value;
}

EScreenResolution URedBloodSeaUserSettings::GetDefaultRBSResolution()
{
	return EScreenResolution::Option1;
}

void URedBloodSeaUserSettings::SetRBSResolution(EScreenResolution value)
{
	switch (value)
	{
		case EScreenResolution::Option1: SetScreenResolution(FIntPoint(1920, 1080));
		case EScreenResolution::Option2: SetScreenResolution(FIntPoint(2560, 1440));
		case EScreenResolution::Option3: SetScreenResolution(FIntPoint(1366, 768));
		case EScreenResolution::Option4: SetScreenResolution(FIntPoint(1280, 720));
		case EScreenResolution::Option5: SetScreenResolution(FIntPoint(1920, 1200));
		case EScreenResolution::Option6: SetScreenResolution(FIntPoint(1680, 1050));
		case EScreenResolution::Option7: SetScreenResolution(FIntPoint(1440, 900));
		case EScreenResolution::Option8: SetScreenResolution(FIntPoint(1280, 800));
		case EScreenResolution::Option9: SetScreenResolution(FIntPoint(1024, 768));
		case EScreenResolution::Option10: SetScreenResolution(FIntPoint(800, 600));
		case EScreenResolution::Option11: SetScreenResolution(FIntPoint(640, 480));
	}
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


	SetFullscreen(Fullscreen);
	SetRBSResolution(Resolution);

	//Quality Setting Missing !!!

	//Brightness Setting Missing !!!

	//Contrast Setting Missing !!!
}
