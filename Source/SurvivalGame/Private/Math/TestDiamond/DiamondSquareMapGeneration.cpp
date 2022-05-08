#include "DiamondSquareMapGeneration.h"
#include <random>
#include <ctime>

using namespace std;


void DiamondSquareMapGeneration::generateWorld(float** tiles,int worldSize) 
{
	timeReset();
	const float SEED = 0.0f;
	tiles[0][0] = tiles[0][worldSize - 1] = tiles[worldSize - 1][worldSize - 1] = SEED;
	double h = 0.75;
	for (int sideLength = worldSize - 1; sideLength >= 2; sideLength /= 2, h /= 2.0) {
		int halfSide = sideLength / 2;

		for (int x = 0; x < worldSize - 1; x += sideLength) {
			for (int y = 0; y < worldSize - 1; y += sideLength) {
				double avg = tiles[x][y] + tiles[x + sideLength][y] + tiles[x][y + sideLength] + tiles[x + sideLength][y + sideLength];
				avg /= 4.0;

				tiles[x + halfSide][y + halfSide] = (float)(avg + (fRand2(0, 1) * 2 * h) - h);
			}
		}

		for (int x = 0; x < worldSize - 1; x += halfSide) {
			for (int y = (x + halfSide) % sideLength; y < worldSize - 1; y += sideLength) {
				float avg =
					tiles[(x - halfSide + worldSize - 1) % (worldSize - 1)][y] + //left of center
					tiles[(x + halfSide) % (worldSize - 1)][y] + //right of center
					tiles[x][(y + halfSide) % (worldSize - 1)] + //below center
					tiles[x][(y - halfSide + worldSize - 1) % (worldSize - 1)]; //above center
				avg /= 4.0;

				avg = (float)(avg + (fRand2(0, 1) * 2 * h) - h);
				tiles[x][y] = avg;
				if (x == 0) tiles[worldSize - 1][y] = avg;
				if (y == 0) tiles[x][worldSize - 1] = avg;
			}
		}
	}
}

double DiamondSquareMapGeneration::fRand2(float fMin, float fMax)
{
	return FMath::RandRange(fMin,fMax);
}

void DiamondSquareMapGeneration::timeReset()
{
	srand(time(0)); //Seed the random system, important, otherwise the number the same
}
