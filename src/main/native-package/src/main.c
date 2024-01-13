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

  int x = -8864;
  int z = -18288;
  int searchSize = 16;

  uint64_t seed = 8315782600996436058;
  long p = getNearestStructure(structType, x, z, seed, mc, searchSize);
  x = (int)(p >> 32);
  z = (int)(p & 0xffffffffL);
  printf("Coordinates of nearest structure: %d, %d\n", x, z);
}

long long getNearestStructure(int structType, int x, int z, uint64_t seed,
                              int mc, int searchSize) {
  Pos bestPos;
  int bestDistance = -1;
  Generator g;
  setupGenerator(&g, mc, 0);
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
        int distance = sqrt((p.x - x) * (p.x - x) + (p.z - z) * (p.z - z));
        if (bestDistance == -1 || distance < bestDistance) {
          bestDistance = distance;
          bestPos = p;
          printf("Found structure at %d, %d\n", dx, dz);
					printf("Coords: %d, %d\n", p.x, p.z);
        }
      }
    }
  }
	printf("Best distance: %d\n", bestDistance);

  return ((long long)bestPos.x << 32) | (bestPos.z & 0xffffffffL);
}
