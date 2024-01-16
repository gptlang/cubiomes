// find a seed with a certain structure at the origin chunk
#include "main.h"
#include "biomes.h"
#include "finders.h"
#include "generator.h"
#include <math.h>
#include <stdio.h>
const int MAX_SEARCH_SIZE = 128;
int main() {
  int structType = Slime_Chunk;
  int mc = MC_1_20;

  int x = 0;
  int z = 0;

  uint64_t seed = 7613135772489002899;
  long long p = getNearestStructure(structType, x, z, seed, mc);
  x = (int)(p >> 32);
  z = (int)(p & 0xffffffffL);
  printf("Coordinates of nearest structure: %d, %d\n", x, z);
}

long long getNearestStronghold(int x, int z, uint64_t seed, int mc) {
  StrongholdIter siter;
  Pos bestPos = initFirstStronghold(&siter, mc, seed);
  Generator g;
  setupGenerator(&g, mc, 0);
  applySeed(&g, DIM_OVERWORLD, seed);
  int bestDistance = hypot(bestPos.x - x, bestPos.z - z);
  int numStrongholds = 0;
  while (1) {
    if (nextStronghold(&siter, &g) <= 0) {
      break;
    }
    int distance = hypot(siter.pos.x - x, siter.pos.z - z);
    if (distance < bestDistance) {
      bestDistance = distance;
      bestPos = siter.pos;
    }
    numStrongholds++;
  }
  printf("Number of strongholds: %d\n", numStrongholds);
  printf("Best distance: %d\n", bestDistance);
  return ((long long)bestPos.x << 32) | (bestPos.z & 0xffffffffL);
}

long long getNearestStructure(int structType, int oX, int oZ, uint64_t seed,
                              int mc) {
  Pos bestPos = {0, 0};
  int bestDistance = -1;
  Generator g;
  setupGenerator(&g, mc, 0);
  int dimension = getDimFromFeature(structType);

  applySeed(&g, dimension, seed);
  int startZ;
  int startX;
  // Regions are 32x32 chunks of 16x16 blocks
  if (structType == Slime_Chunk) {
    startX = oX / 16;
    startZ = oZ / 16;
  } else {
    startX = oX / 16 / 32;
    startZ = oZ / 16 / 32;
  }

  int x, y, dx, dy;
  x = y = dx = 0;
  dy = -1;
  int t = MAX_SEARCH_SIZE;
  int maxI = t * t;
  Pos foundPos;
  int found = 0;
  for (int i = 0; i < maxI; i++) {
    if ((-MAX_SEARCH_SIZE / 2 <= x) && (x <= MAX_SEARCH_SIZE / 2) &&
        (-MAX_SEARCH_SIZE / 2 <= y) && (y <= MAX_SEARCH_SIZE / 2)) {
      if (found && abs(x) + abs(y) > abs(foundPos.x) + abs(foundPos.z)) {
        break;
      }
      Pos p;
      if (structType == Slime_Chunk) {
        if (isSlimeChunk(seed, x + startX, y + startZ)) {
          if (bestDistance == -1 || abs(x) + abs(y) < bestDistance) {
            bestDistance = abs(x) + abs(y);
            p.x = (x + startX) * 16;
            p.z = (y + startZ) * 16;
            bestPos = p;
            foundPos.x = x + startX;
            foundPos.z = y + startZ;
            found = 1;
          }
        }
      } else if (getStructurePos(structType, mc, seed, x + startX, y + startZ,
                                 &p)) {
        if (isViableStructurePos(structType, &g, p.x, p.z, 0)) {
          double distance = abs(p.x - oX) + abs(p.z - oZ);
          if (bestDistance == -1 || distance < bestDistance) {
            bestDistance = distance;
            bestPos = p;
            foundPos.x = x + startX;
            foundPos.z = y + startZ;
            found = 1;
          }
        }
      }
    }
    if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
      t = dx;
      dx = -dy;
      dy = t;
    }
    x += dx;
    y += dy;
  }
  printf("Best distance: %d\n", bestDistance);
  if (bestDistance == -1) {
    return -1;
  }

  return ((long long)bestPos.x << 32) | (bestPos.z & 0xffffffffL);
}

int getDimFromFeature(enum StructureType structType) {
  int dimension;
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
  case Mineshaft: // Doesn't work
  case Desert_Well:
  case Slime_Chunk:
  case Geode: {
    dimension = DIM_OVERWORLD;
    break;
  }
  case Bastion:
  case Fortress:
  case Ruined_Portal_N: {
    dimension = DIM_NETHER;
    break;
  }
  case End_City:
  case End_Gateway: {
    dimension = DIM_END;
    break;
  }
  default: {
    printf("Unknown structure type: %d\n", structType);
    return -1;
  }
  }
  return dimension;
}
