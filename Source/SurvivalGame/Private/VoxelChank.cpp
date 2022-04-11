#include "VoxelChank.h"

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

	InstanceDirt = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceDirt"));
	InstanceDirt->SetMobility(EComponentMobility::Static);
	InstanceDirt->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Dirt(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Dirt.Dirt'"));
	InstanceDirt->SetStaticMesh(Dirt.Object);
}

void AVoxelChank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AVoxelChank::OnConstruction(const FTransform& Transform)
{
	InstanceTopGrass->ClearInstances();
	for (int i = ChunkSize * -1; i <= ChunkSize; i++)
	{
		int LoopX = i;
		for (int j = ChunkSize * -1; j <= ChunkSize; j++)
		{
			int LoopY = j;
			float A;
			float B;
			ActorLocationVoxelWorldXY(LoopX,LoopY,A,B);
			float Noise2D = USimplexNoiseBPLibrary::SimplexNoise2D(A, B, NoiseDensity);
			float NoiseShift = floor(Noise2D * NoiseScale);
			for (int k = Depth * -1; k <= 0; k++)
			{
				int LoopZ = k;
				int PostNoiseZ = NoiseShift - LoopZ;
				FVector position(LoopX * VoxelSize, LoopY * VoxelSize, PostNoiseZ * VoxelSize * -1);
				if (LoopZ < -3)
				{
					float Noise3D = USimplexNoiseBPLibrary::SimplexNoise3D(
						position.X, position.Y, position.Z, NoiseDensity3D);
					if (!(Noise3D < Threshold3D))
					{
						FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
						InstanceTopGrass->AddInstance(transform);
					}
				}
				else
				{
					FTransform transform = FTransform(FRotator(0, 0, 0), position, FVector(0.5, 0.5, 0.5));
					InstanceTopGrass->AddInstance(transform);
				}
			}
		}
	}
}

void AVoxelChank::ActorLocationVoxelWorldXY(const int XIndex, const int YIndex,  float& X, float& Y) const
{
	const FVector Location = InstanceTopGrass->GetComponentLocation();
	X = Location.X+XIndex * VoxelSize;
	Y = Location.Y+YIndex * VoxelSize;
	//Z = Location.Z+ZIndex * VoxelSize;
}

void AVoxelChank::ActorLocationVoxelWorldZ(const int ZIndex, int& Z) const
{
	FVector Location = InstanceTopGrass->GetComponentLocation();
	Z = Location.Z+ZIndex * VoxelSize;
}


void AVoxelChank::InitializeParameters(float NoiseDensityParam, float VoxelSizeParam, int NoiseScaleParam,
                                       int ChunkSizeParam, int DepthParam, float NoiseDensity3DParam,
                                       float Threshold3DParam)
{
	NoiseDensity = NoiseDensityParam;
	VoxelSize = VoxelSizeParam;
	NoiseScale = NoiseScaleParam;
	ChunkSize = ChunkSizeParam;
	Depth = DepthParam;
	NoiseDensity3D = NoiseDensity3DParam;
	Threshold3D = Threshold3DParam;
}
