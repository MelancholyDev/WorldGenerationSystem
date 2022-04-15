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

void AVoxelChank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AVoxelChank::OnConstruction(const FTransform& Transform)
{
	InstanceTopGrass->ClearInstances();
	InstanceDirt->ClearInstances();
	for (int LoopX = ChunkSize * -1; LoopX <= ChunkSize; LoopX++)
	{
		for (int LoopY = ChunkSize * -1; LoopY <= ChunkSize; LoopY++)
		{
			USimplexNoiseBPLibrary::setNoiseSeed(16);
			float A;
			float B;
			ActorLocationVoxelWorldXY(LoopX, LoopY, A, B);

			float Noise2DSharp = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(A, B, 2.3, 0.6, 6, NoiseDensity,true);
			Noise2DSharp = Clamp(Noise2DSharp, 0, 1);
			float Noise2DSmooth = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(A, B, 2.3, 0.6, 1, NoiseDensity,true);
			Noise2DSmooth = Clamp(Noise2DSmooth, 0, 1);
			float FinalNoise = BezierComputations::FilterMap(Noise2DSharp, Noise2DSmooth, 0.5, 1, 0.25, 1, 1, 1);
			float NoiseShift = FinalNoise * NoiseScale;
			NoiseShift = floor(NoiseShift);
			int VoxelShift = VoxelSize;
			int CurrentLocation =NoiseShift*VoxelShift;
			for (int LoopZ = NoiseShift; LoopZ >= Depth; LoopZ--)
			{
				USimplexNoiseBPLibrary::setNoiseSeed(16);
				FVector position(LoopX * VoxelSize, LoopY * VoxelSize, CurrentLocation);
				if ((LoopZ < NoiseShift-3) && (LoopZ != Depth))
				{
					float Noise3D = USimplexNoiseBPLibrary::SimplexNoise3D(
						A, B, CurrentLocation, NoiseDensity3D);
					if (!(Noise3D < Threshold3D))
					{
						FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
						InstanceDirt->AddInstance(transform);
					}
				}
				else
				{
					if (LoopZ == 0)
					{
						USimplexNoiseBPLibrary::setNoiseSeed(21);
						float Temperature = USimplexNoiseBPLibrary::SimplexNoise2D(
							A, B, NoiseDensityTemperature);
						FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
						if (Temperature < 0.33)
						{
							InstanceSnow->AddInstance(transform);
						}
						else if ((Temperature >= 0.33) & (Temperature < 0.66))
						{
							InstanceTopGrass->AddInstance(transform);
						}
						else
						{
							InstanceSand->AddInstance(transform);
						}
					}
					else
					{
						FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
						InstanceDirt->AddInstance(transform);
					}
				}
				CurrentLocation-=VoxelShift;
			}
		}
	}
}

void AVoxelChank::ActorLocationVoxelWorldXY(const int XIndex, const int YIndex, float& X, float& Y) const
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


void AVoxelChank::InitializeParameters(float NoiseDensityParam, float VoxelSizeParam, int NoiseScaleParam,
                                       int ChunkSizeParam, int DepthParam, float NoiseDensity3DParam,
                                       float Threshold3DParam, int WaterLevelParam, float NoiseDensityTemperatureParam)
{
	WaterLevel = WaterLevelParam;
	NoiseDensity = NoiseDensityParam;
	VoxelSize = VoxelSizeParam;
	NoiseScale = NoiseScaleParam;
	ChunkSize = ChunkSizeParam;
	Depth = DepthParam;
	NoiseDensity3D = NoiseDensity3DParam;
	Threshold3D = Threshold3DParam;
	NoiseDensityTemperature = NoiseDensityTemperatureParam;
}

float Clamp(float x, float left, float right)
{
	if (x < left)
		return left;
	if (x > right)
		return right;
	return x;
}
