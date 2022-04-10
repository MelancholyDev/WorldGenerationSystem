
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Grass(TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/EarthGrass.EarthGrass'"));
	InstanceTopGrass->SetStaticMesh(Grass.Object);

	InstanceDirt = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceDirt"));
	InstanceDirt->SetMobility(EComponentMobility::Static);
	InstanceDirt->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Dirt(TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Dirt.Dirt'"));
	InstanceDirt->SetStaticMesh(Dirt.Object);
}
	
void AVoxelChank::BeginPlay()
{
	Super::BeginPlay();
	InstanceTopGrass->ClearInstances();
	FVector InstanceLocation = InstanceTopGrass->GetComponentLocation();
	int LoopX;
	int LoopY;
	int LoopZ;
	int PostNoiseZ;
	for (int i = ChunkSize * -1; i <= ChunkSize; i++)
	{
	
		LoopX = i;
		for (int j = ChunkSize * -1; j <= ChunkSize; j++)
		{
	 		
			LoopY = j;
			for (int k = Depth * -1; k <= 0; k++)
			{
	 			
				LoopZ = k;
				float A;
				float B;
				int C;
				ActorLocationVoxelWorld(LoopX, LoopY, LoopZ, A, B, C);
				float Noise2D = USimplexNoiseBPLibrary::SimplexNoise2D(A, B, NoiseDesity);
				PostNoiseZ = floor(Noise2D * NoiseScale) - LoopZ;
				FVector position(LoopX * VoxelSize, LoopY * VoxelSize, PostNoiseZ * VoxelSize * -1);
				if (LoopZ < -3)
				{
					FVector NoiseLocation = FVector(position.X,position.Y,position.Z);
					NoiseLocation = NoiseLocation+InstanceLocation;
					float Noise3D = USimplexNoiseBPLibrary::SimplexNoise3D(
						NoiseLocation.X,NoiseLocation.Y,NoiseLocation.Z, NoiseDensity3D);
					if(!(Noise3D<Threshold3D))
					{
						FTransform transform = FTransform(FRotator(0,0,0),position,FVector(0.5,0.5,0.5));
						InstanceTopGrass->AddInstance(transform);
					}
				}
				else
				{
					FTransform transform = FTransform(FRotator(0,0,0),position,FVector(0.5,0.5,0.5));
					InstanceTopGrass->AddInstance(transform);
				}
			}
		}
	}
}

void AVoxelChank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoxelChank::OnConstruction(const FTransform& Transform)
{
	InstanceTopGrass->ClearInstances();
	 int LoopX;
	 int LoopY;
	 int LoopZ;
	 int PostNoiseZ;
	 for (int i = ChunkSize * -1; i <= ChunkSize; i++)
	 {
	
	 	LoopX = i;
	 	for (int j = ChunkSize * -1; j <= ChunkSize; j++)
	 	{
	 		
	 		LoopY = j;
	 		for (int k = Depth * -1; k <= 0; k++)
	 		{
	 			LoopZ = k;
	 			float A;
	 			float B;
	 			int C;
	 			ActorLocationVoxelWorld(LoopX, LoopY, LoopZ, A, B, C);
	 			float Noise2D = USimplexNoiseBPLibrary::SimplexNoise2D(A, B, NoiseDesity);
	 			PostNoiseZ = floor(Noise2D * NoiseScale) - LoopZ;
	 			FVector position(LoopX * VoxelSize, LoopY * VoxelSize, PostNoiseZ * VoxelSize * -1);
	 			if (LoopZ < -3)
	 			{
	 				float Noise3D = USimplexNoiseBPLibrary::SimplexNoise3D(
	 					position.X,position.Y,position.Z, NoiseDensity3D);
	 				if(!(Noise3D<Threshold3D))
	 				{
	 					FTransform transform = FTransform(FRotator(0,0,0),position,FVector(0.5,0.5,0.5));
	 					InstanceTopGrass->AddInstance(transform);
	 				}
	 			}
	 			else
	 			{
	 				FTransform transform = FTransform(FRotator(0,0,0),position,FVector(0.5,0.5,0.5));
	 				InstanceTopGrass->AddInstance(transform);
	 			}
	 		}
	 	}
	 }
}

void AVoxelChank::ActorLocationVoxelWorld(const int XIndex, const int YIndex, const int ZIndex, float& X, float& Y,
                                          int& Z)
{
	FVector loc = GetActorLocation();
	loc = loc / VoxelSize;
	X = (XIndex + floor(loc.X)) * VoxelSize;
	Y = (XIndex + floor(loc.X)) * VoxelSize;
	Z = XIndex + floor(loc.X);
}
