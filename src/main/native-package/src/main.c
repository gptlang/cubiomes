// find a seed with a certain structure at the origin chunk
#include "main.h"
#include "biomes.h"
#include "finders.h"
#include "generator.h"
#include <math.h>
#include <stdio.h>

int main() {
  int structType = Village;
  int mc = MC_1_20;

  int x = 0;
  int z = 0;
  int searchSize = 8;

  uint64_t seed = -1717312650922586601;
  long long p = getNearestStructure(structType, x, z, seed, mc, searchSize);
  x = (int)(p >> 32);
  z = (int)(p & 0xffffffffL);
  printf("Coordinates of nearest structure: %d, %d\n", x, z);
	long long stronghold = getNearestStronghold(x, z, seed, mc, 12);
	x = (int)(stronghold >> 32);
	z = (int)(stronghold & 0xffffffffL);
	printf("Coordinates of nearest stronghold: %d, %d\n", x, z);
	return 0;
}


long long getNearestStronghold(int x, int z, uint64_t seed, int mc, int numStrongholds){
	StrongholdIter siter;
	Pos bestPos = initFirstStronghold(&siter, mc, seed);
	Generator g;
	setupGenerator(&g, mc, 0);
	applySeed(&g, DIM_OVERWORLD, seed);
	int bestDistance = hypot(bestPos.x - x, bestPos.z - z);
	for (int i = 1; i < numStrongholds; i++) {
		if (nextStronghold(&siter, &g) <=0) {
			break;
		}
		int distance = hypot(siter.pos.x - x, siter.pos.z - z);
		if (distance < bestDistance) {
			bestDistance = distance;
			bestPos = siter.pos;
		}

	}
	return ((long long)bestPos.x << 32) | (bestPos.z & 0xffffffffL);
}

long long getNearestStructure(int structType, int x, int z, uint64_t seed,
                              int mc, int searchSize) {
  Pos bestPos = {0, 0};
  int bestDistance = -1;
  Generator g;
  setupGenerator(&g, mc, 0);
	int dimension = DIM_OVERWORLD;
	switch (structType) {
		case Desert_Pyramid:
		case Jungle_Pyramid:
		case Swamp_Hut:
		case Igloo:
		case Village:
		case Ocean_Ruin:
		case Shipwreck:
		case Monument:
		case Mansion:
		case Outpost:
		case Ruined_Portal:
		case Ancient_City:
		case Treasure:
		// case Mineshaft: // Doesn't work
		case Desert_Well:
		case Geode:
			{
				dimension = DIM_OVERWORLD;
				break;
			}
		case Bastion:
		case Fortress:
		case Ruined_Portal_N:
			{
				dimension = DIM_NETHER;
				break;
			}
		case End_City:
		case End_Gateway:
			{
				dimension = DIM_END;
				break;
			}
		default:
			{
				printf("Unknown structure type: %d\n", structType);
				return -1;
			}
	}
  applySeed(&g, DIM_OVERWORLD, seed);

  // Regions are 32x32 chunks
  int startX = x / 16 / 32; // convert coordinates to chunk coordinates
  int startZ = z / 16 / 32;

  for (int dx = startX - searchSize; dx <= startX + searchSize; dx++) {
    for (int dz = startZ - searchSize; dz <= startZ + searchSize; dz++) {
      Pos p;
      if (!getStructurePos(structType, mc, seed, dx, dz, &p)) {
        continue;
      }

      if (isViableStructurePos(structType, &g, p.x, p.z, 0)) {
        double distance = hypot(p.x - x, p.z - z);
        if (bestDistance == -1 || distance < bestDistance) {
          bestDistance = distance;
          bestPos = p;
        }
      }
    }
  }
  printf("Best distance: %d\n", bestDistance);
	if (bestDistance == -1) {
		return -1;
	}

  return ((long long)bestPos.x << 32) | (bestPos.z & 0xffffffffL);
}
