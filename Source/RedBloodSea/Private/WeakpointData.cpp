// Fill out your copyright notice in the Description page of Project Settings.


#include "WeakpointData.h"

#include "Engine/SkeletalMeshSocket.h"


UWeakpointData::UWeakpointData()
{
	int max = static_cast<int>(EWeakpointWeight::NUM);
	for (int i = 0; i < max; i++)
	{
		WeightTable[i] = 1;
	}
	max = static_cast<int>(EWeakpointSize::NUM);
	for (int i = 0; i < max; i++)
	{
		SizeChart[i] = 1;
	}
}

void UWeakpointData::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWeakpointData, SkeletalMesh))
	{
		WeakpointsSockets.Empty();
		UpdateSockets();
	}
	Super::PostEditChangeProperty(e);
}

TArray<FName> UWeakpointData::GetAllWeakpointsSockets() const
{
	TArray<FName> result;
	if(SkeletalMesh == nullptr)
		return result;
	for (auto Socket : SkeletalMesh->GetActiveSocketList())
	{
		if(Socket->SocketName.ToString().StartsWith("wp_"))
		{
			result.Add(Socket->SocketName);
		}
	}
	return result;
}

void UWeakpointData::UpdateSockets()
{
	for (auto name : GetAllWeakpointsSockets())
	{
		FWeakpointSlot WeakpointSlot;
		WeakpointSlot.SocketName = name;
		WeakpointsSockets.Add(WeakpointSlot);
	}
}
