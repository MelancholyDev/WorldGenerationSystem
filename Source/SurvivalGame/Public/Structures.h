#pragma once
#include "CoreMinimal.h"
#include "VoxelChank.h"
#include "Structures.generated.h"

USTRUCT()
struct FVoxelLine
{
	GENERATED_BODY();
	TArray<AVoxelChank*> Voxels;
	FVoxelLine()
	{
		Voxels.Empty();
	}
};