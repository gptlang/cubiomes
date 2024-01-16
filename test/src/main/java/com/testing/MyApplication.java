package com.testing;
import java.math.BigInteger;

import cubitect.Cubiomes;
import cubitect.Cubiomes.Pos;
// src/MyApplication.java
public class MyApplication {
    public static void main(String[] args) {
        // Example usage of a method from the HawtJNI library
        // Test myJniLibrary = new Test();
        Cubiomes.main();
				long seed = new BigInteger("-1717312650922586601").longValue();
				Pos pos = Cubiomes.GetNearestStructure(Cubiomes.StructureType.Stronghold, 0, 0, seed, Cubiomes.MCVersion.MC_1_20);
				System.out.println(pos.x + " " + pos.z);
    }
}

