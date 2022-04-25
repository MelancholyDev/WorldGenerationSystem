#pragma once
#include "FVoxelGeneraionData.generated.h"

USTRUCT(BlueprintType)
struct FVoxelGenerationData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	bool IsAddDepth;
	UPROPERTY(EditAnywhere)
	float Threshold3D=0;
	UPROPERTY(EditAnywhere)
	float NoiseDensity3D=0;
	float VoxelSize=0;
	int NoiseScale=0;
	int ChunkSize=0;
	int Depth=0;
	int MapSize=0;
	float **Map=0;
	float **Heat=0;
	void Initialize(bool IsAddDepthParam,float VoxelSizeParam,int NoiseScaleParam,int ChunkSizeParam,int DepthParam,float NoiseDensity3DParam,
		float Threshold3DParam,int MapSizeParam,float **MapParam,float **HeatParam)
	{
		IsAddDepth=IsAddDepthParam;
		VoxelSize=VoxelSizeParam;
		NoiseScale=NoiseScaleParam;
		ChunkSize=ChunkSizeParam;
		Threshold3D = Threshold3DParam;
		Depth=DepthParam;
		NoiseDensity3D=NoiseDensity3DParam;
		MapSize = MapSizeParam;
		Map=MapParam;
		Heat=HeatParam;
	}
};
