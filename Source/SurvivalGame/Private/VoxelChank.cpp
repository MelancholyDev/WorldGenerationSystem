#include "VoxelChank.h"
#include "BezierComputations.h"

AVoxelChank::AVoxelChank()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Root->SetMobility(EComponentMobility::Static);

	InstanceTopGrass = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceGrass"));
	InstanceTopGrass->SetMobility(EComponentMobility::Static);
	InstanceTopGrass->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Grass(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/EarthGrass.EarthGrass'"));
	InstanceTopGrass->SetStaticMesh(Grass.Object);
	InstanceTopGrass->SetRelativeLocation(FVector(0, 0, 0));

	InstanceDirt = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceDirt"));
	InstanceDirt->SetMobility(EComponentMobility::Static);
	InstanceDirt->SetupAttachment(Root);
	InstanceDirt->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Dirt(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Dirt.Dirt'"));
	InstanceDirt->SetStaticMesh(Dirt.Object);

	InstanceSnow = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceSnow"));
	InstanceSnow->SetMobility(EComponentMobility::Static);
	InstanceSnow->SetupAttachment(Root);
	InstanceSnow->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Snow(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Snow.Snow'"));
	InstanceSnow->SetStaticMesh(Snow.Object);

	InstanceSand = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceSand"));
	InstanceSand->SetMobility(EComponentMobility::Static);
	InstanceSand->SetupAttachment(Root);
	InstanceSand->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sand(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Sand.Sand'"));
	InstanceSand->SetStaticMesh(Sand.Object);
}

void AVoxelChank::OnConstruction(const FTransform& Transform)
{
	InstanceTopGrass->ClearInstances();
	InstanceDirt->ClearInstances();
	for (int LoopX = ChunkSize * -1; LoopX <= ChunkSize; LoopX++)
	{
		for (int LoopY = ChunkSize * -1; LoopY <= ChunkSize; LoopY++)
		{
			int A;
			int B;
			int IndexShift = MapSize / 2;
			ActorLocationVoxelWorldXY(LoopX, LoopY, A, B);
			int XIndex = A / VoxelSize + IndexShift;
			int YIndex = B / VoxelSize + IndexShift;
			float Shift;
			if(CheckInBound(XIndex,MapSize) & CheckInBound(YIndex,MapSize))
			{
				Shift = Map[XIndex][YIndex];
			}else
			{
				Shift=0;
			}
			Shift = Shift*NoiseScale;
			int ShiftClamped; 
			ShiftClamped = floor(Shift)*VoxelSize;
			FVector position(A, B, ShiftClamped);
			FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
			InstanceTopGrass->AddInstance(transform);
			//float Noise2DSharp = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(A, B, 4, 0.3, 5, NoiseDensity);
			//Noise2DSharp = Clamp(Noise2DSharp, 0, 1);
			//float Noise2DSmooth = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(A, B, 4, 0.3, 2, NoiseDensity);
			//Noise2DSmooth = Clamp(Noise2DSmooth, 0, 1);
			//float FinalNoise;
			//float FinalNoise = Noise2DSmooth;
			//float Temperature = USimplexNoiseBPLibrary::SimplexNoise2D(
			//	A, B, NoiseDensityTemperature);
			// if (Temperature < 0.33)
			// {
			// 	FinalNoise = BezierComputations::FilterMap(Noise2DSharp, Noise2DSmooth, 0.5, 1, 0.25, 1, 1, 1);
			// }
			// else if ((Temperature >= 0.33) & (Temperature < 0.66))
			// {
			// 	FinalNoise = BezierComputations::FilterMap(Noise2DSharp, Noise2DSmooth, 0.75, 0.5, 0.4, 0.4, 0.33, 0.2);
			// }
			// else
			// {
			// 	FinalNoise = BezierComputations::FilterMap(Noise2DSharp, Noise2DSmooth, 0.75, 0.5, 0.4, 0.4, 0.33, 0.2);
			// }
			// float NoiseShift2 = FinalNoise * NoiseScale;
			// float Threshold3D = 0;
			// float NoiseDensity3D = 0;
			// int NoiseShift = NoiseShift2;
			// int VoxelShift = VoxelSize;
			// int CurrentLocation = NoiseShift * VoxelShift;
			//float Noise = 
			//for (int LoopZ = NoiseShift; LoopZ >= Depth; LoopZ--)
			//for (int LoopZ = NoiseShift; LoopZ >= NoiseShift; LoopZ--)
			//{
			//FVector position(LoopX * VoxelSize, LoopY * VoxelSize, CurrentLocation);
			//if ((LoopZ < NoiseShift-3) && (LoopZ != Depth))
			// if (false)
			// {
			// 	float Noise3D = USimplexNoiseBPLibrary::SimplexNoise3D(
			// 		A, B, CurrentLocation, NoiseDensity3D);
			// 	if (!(Noise3D < Threshold3D))
			// 	{
			// 		FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
			// 		InstanceDirt->AddInstance(transform);
			// 	}
			// }
			// else
			// {
			// 	if (LoopZ == NoiseShift)
			// 	{
			// 		Temperature = USimplexNoiseBPLibrary::SimplexNoise2D(
			// 			A, B, NoiseDensityTemperature);
			// 		FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
			// 		if (Temperature < 0.33)
			// 		{
			// 			InstanceSnow->AddInstance(transform);
			// 		}
			// 		else if ((Temperature >= 0.33) & (Temperature < 0.66))
			// 		{
			// 			InstanceTopGrass->AddInstance(transform);
			// 		}
			// 		else
			// 		{
			// 			InstanceSand->AddInstance(transform);
			// 		}
			// 	}
			// 	else
			// 	{
			// 		FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
			// 		InstanceDirt->AddInstance(transform);
			// 	}
			// }
			// CurrentLocation -= VoxelShift;
			//}
		}
	}
}

void AVoxelChank::InitializeParameters(float VoxelSizeParam, int NoiseScaleParam, int ChunkSizeParam, int DepthParam,
                                       int MapSizeParam, float** MapParam)
{
	VoxelSize = VoxelSizeParam;
	NoiseScale = NoiseScaleParam;
	ChunkSize = ChunkSizeParam;
	Depth = DepthParam;
	MapSize = MapSizeParam;
	Map = MapParam;
}

void AVoxelChank::ActorLocationVoxelWorldXY(const int XIndex, const int YIndex, int& X, int& Y) const
{
	const FVector Location = InstanceTopGrass->GetComponentLocation();
	X = Location.X + XIndex * VoxelSize;
	Y = Location.Y + YIndex * VoxelSize;
}

void AVoxelChank::ActorLocationVoxelWorldZ(const int ZIndex, int& Z) const
{
	const FVector Location = InstanceTopGrass->GetComponentLocation();
	Z = Location.Z + ZIndex * VoxelSize;
}

bool AVoxelChank::CheckInBound(int Index,int Size)
{
	if((Index<Size) & (Index>=0))
		return true;
	return false;
}