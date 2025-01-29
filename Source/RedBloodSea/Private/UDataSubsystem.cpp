// Fill out your copyright notice in the Description page of Project Settings.


#include "UDataSubsystem.h"

#include <string>
#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "DSP/BufferDiagnostics.h"
#include "HLSLTree/HLSLTreeTypes.h"
#include "Serialization/BulkDataRegistry.h"

void UUDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnKill.AddUniqueDynamic(this,&UUDataSubsystem::AddKill);
	OnLinked.AddUniqueDynamic(this,&UUDataSubsystem::AddLinked);

	for (EActionType Val : TEnumRange<EActionType>())
	{
		Counters.Add(Val);
	}
}

void UUDataSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnKill.RemoveDynamic(this,&UUDataSubsystem::AddKill);
	OnLinked.RemoveDynamic(this,&UUDataSubsystem::AddLinked);
}

void UUDataSubsystem::StartLogs()
{
	ResetLogs();
	LogStart = FDateTime::Now();
	LogEvent(EActionType::SESSION_START);
}

void UUDataSubsystem::StopLogs()
{
	FDateTime CurrentTime = FDateTime::Now();
	FTimespan SessionLength = CurrentTime - LogStart;
	FString FolderPath = FString(*FPaths::ProjectDir());
	FString FileName = LogStart.ToFormattedString(TEXT("%Y_%m_%d_%H_%M_%S"))+GetWorld()->GetMapName();
	if(ExportLogs(FolderPath,FileName))
	{
		if(GEngine)
			 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Success export Data"));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Failed export Data"));
	}
}

void UUDataSubsystem::ResetLogs()
{
	for (EActionType Val : TEnumRange<EActionType>())
	{
		Counters[Val] = 0;
	}
	Logs.Empty();
}

bool UUDataSubsystem::ExportLogs(FString FolderLocation, FString FileName)
{
	FString FilePath = FolderLocation + "\\" + FileName + ".csv";
	return FFileHelper::SaveStringArrayToFile(LogsToStrings(),FilePath.GetCharArray().GetData());
}


float UUDataSubsystem::GetKillPercent()
{
	float KillPercent = static_cast<float>(KillCount) / static_cast<float>(KillCount+LinkedCount);
	return KillPercent;
}

void UUDataSubsystem::LogEventAtPos(const EActionType Type, const FVector& Position)
{
	Logs.Add(FActionLog{
		.TimeStamp = FDateTime::Now(),
		.Type = Type,
		.Position = Position
	});
	
	Counters[Type]++;
}

void UUDataSubsystem::LogEvent(const EActionType Type)
{
	LogEventAtPos(Type,FVector(0,0,0));
}

void UUDataSubsystem::AddKill()
{
	KillCount++;
	OnCountChange.Broadcast(GetKillPercent());
}

void UUDataSubsystem::AddLinked(int Count)
{
	LinkedCount += Count;
	OnCountChange.Broadcast(GetKillPercent());
}

void UUDataSubsystem::ResetAll()
{
	LinkedCount = 0;
	KillCount = 0;

	ResetLogs();
}

FString UUDataSubsystem::GetActionName(const EActionType Type)
{
	return UEnum::GetDisplayValueAsText(Type).ToString();
}

TArray<FString> UUDataSubsystem::LogsToStrings()
{
	TArray<FString> LogStrings;
	FString Titles = TEXT("Time Stamp,Action Type,Location,,");
	for (auto Counter : Counters)
	{
		Titles = Titles + "," + GetActionName(Counter.Key);
	}
	LogStrings.Add(Titles);
	for(auto Log : Logs)
	{
		LogStrings.Add(Log.TimeStamp.ToFormattedString(TEXT("%d/%m/%Y %H:%M:%S")) + "," + GetActionName(Log.Type) + "," + Log.Position.ToString());
	}

	int index = 0;
	if(LogStrings.Num() < 2)
	{
		LogStrings.Add(",,,,");
	}
	LogStrings[1] = LogStrings[1] + ",,";
	for (auto Counter : Counters)
	{
		LogStrings[1] = LogStrings[1] + "," +std::to_string(Counter.Value).data();
	}
	return LogStrings;
}

