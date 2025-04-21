package cubitect;

import org.fusesource.hawtjni.runtime.*;

@JniClass
public class Cubiomes {

  private static final Library LIBRARY = new Library("cubiomes", Cubiomes.class);

  static {
    LIBRARY.load();
  }

  public static native int main();

  private static native long getNearestStructure(
      int structType, int x, int z, long seed, int mc_version);

  private static native long getNearestStronghold(int x, int z, long seed, int mc_version);

  public static Pos GetNearestStructure(
      StructureType structType, int x, int z, Long seed, MCVersion mc_version) {
    long result;
    if (structType == StructureType.Stronghold) {
      result = getNearestStronghold(x, z, seed, mc_version.ordinal());
    } else {
      result = getNearestStructure(structType.ordinal(), x, z, seed, mc_version.ordinal());
    }
    if (result == -1) {
      return null;
    }
    Pos pos = new Pos();
    pos.x = (int) (result >> 32);
    pos.z = (int) (result & 0xFFFFFFFF);
    return pos;
  }

  public static class Pos {
    public int x;
    public int z;
    public int y;
  }

  public static enum MCVersion { // MC_1_X refers to the latest patch of the respective 1.X release.
    // NOTE: Development effort focuses on just the newest patch for each major
    // release. Minor releases and major versions <= 1.0 are experimental.
    MC_UNDEF,
    MC_B1_7,
    MC_B1_8,
    MC_1_0_0,
    MC_1_1_0,
    MC_1_2_5,
    MC_1_3_2,
    MC_1_4_7,
    MC_1_5_2,
    MC_1_6_4,
    MC_1_7_10,
    MC_1_8_9,
    MC_1_9_4,
    MC_1_10_2,
    MC_1_11_2,
    MC_1_12_2,
    MC_1_13_2,
    MC_1_14_4,
    MC_1_15_2,
    MC_1_16_1,
    MC_1_16_5,
    MC_1_17_1,
    MC_1_18_2,
    MC_1_19_2,
    MC_1_19_4,
    MC_1_20_6,
    MC_1_21_1,
    MC_1_21_3,
    MC_1_21_WD, // Winter Drop, version TBA
  }

  public static enum StructureType {
    Feature, // for locations of temple generation attempts pre 1.13
    Desert_Pyramid,
    Jungle_Temple,
    Swamp_Hut,
    Igloo,
    Village,
    Ocean_Ruin,
    Shipwreck,
    Monument,
    Mansion,
    Outpost,
    Ruined_Portal,
    Ruined_Portal_N,
    Ancient_City,
    Treasure,
    Mineshaft,
    Desert_Well,
    Geode,
    Fortress,
    Bastion,
    End_City,
    End_Gateway,
    Trail_Ruin,
    Slime_Chunk,
    Stronghold,
  }
}
