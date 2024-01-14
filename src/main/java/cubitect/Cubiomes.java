package cubitect;

import org.fusesource.hawtjni.runtime.*;

@JniClass
public class Cubiomes {

	private static final Library LIBRARY = new Library("cubiomes", Cubiomes.class);
	static {
		LIBRARY.load();
	}

	public static native int main();

	private static native long getNearestStructure(int structType, int x, int z, long seed, int mc_version, int searchSize);
	
	public static class Pos {
		public int x;
		public int z;
		public int y;
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
    FEATURE_NUM
	}

	public static Pos GetNearestStructure(StructureType structType, int x, int z, Long seed, int mc_version, int searchSize) {
		long result = getNearestStructure(structType.ordinal(), x, z, seed, mc_version, searchSize);
		if (result == 0) {
			return null;
		}
		Pos pos = new Pos();
		pos.x = (int) (result >> 32);
		pos.z = (int) (result & 0xFFFFFFFF);
		return pos;
	}

}
