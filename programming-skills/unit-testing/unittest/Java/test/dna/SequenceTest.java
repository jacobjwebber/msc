package dna;

import static org.junit.Assert.*;
import org.junit.Test;
import dna.Sequence;

public class SequenceTest {

    @Test
    public void testGetNucleotides() {
        String sequenceStr = "GATTACCA";
        Sequence sequence = new Sequence(sequenceStr);
        assertEquals("Nucleotides returned were not those given", 
            sequenceStr, sequence.getNucleotides());
    }

    @Test
    public void testGetWeight() {
        Sequence sequence = new Sequence("G");
        assertEquals("Weight returned was unexpected", 
            Sequence.WEIGHTS.get('G').doubleValue(),
            sequence.getWeight(), 0.01);
    }

    @Test
    public void testCalculateWeight() {
        Sequence sequence = new Sequence("G");
        assertEquals("Weight returned was unexpected", 
            Sequence.WEIGHTS.get('G').doubleValue(),
            Sequence.calculateWeight(sequence), 0.01);
    }
           
    @Test
    public void testLowerCaseSequence() {
    	String sequenceStr = "gattacca";
    	Sequence sequence = new Sequence(sequenceStr);
    	assertEquals("Not gown uppercase mate", 
    		sequence.getNucleotides(),
    		sequenceStr.toUpperCase());
    }
}
