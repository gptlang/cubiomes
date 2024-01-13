package cubitect;

import org.fusesource.hawtjni.runtime.*;

@JniClass
public class Cubiomes {

	private static final Library LIBRARY = new Library("mc-seedlocator", Cubiomes.class);
	static {
		LIBRARY.load();
	}

	public static native int main();
	
	private static native long getNearestStructure(int structType, int x, int z, long seed, int mc_version, int searchSize);
	
	public static class Pos {
		public int x;
		public int z;
	}

	public static Pos GetNearestStructure(int structType, int x, int z, Long seed, int mc_version, int searchSize) {
		long result = getNearestStructure(structType, x, z, seed, mc_version, searchSize);
		if (result == 0) {
			return null;
		}
		Pos pos = new Pos();
		pos.x = (int) (result >> 32);
		pos.z = (int) (result & 0xFFFFFFFF);
		return pos;
	}

}
