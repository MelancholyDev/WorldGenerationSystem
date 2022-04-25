#pragma once
#include "VoxelChank.h"
#include "FVoxelLine.generated.h"

USTRUCT()
struct FVoxelLine
{
	GENERATED_BODY()
	TArray<AVoxelChank*> Voxels;

	FVoxelLine()
	{
		Voxels.Empty();
	}
};
