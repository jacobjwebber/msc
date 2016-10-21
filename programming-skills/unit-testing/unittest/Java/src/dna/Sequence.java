package dna;

import java.util.HashMap;

public class Sequence {
    /** Map from nucleotides to molecular weights. */
    public static final HashMap<Character, Double> WEIGHTS;
    /** Nucleotides in the sequence. */
    private String nucleotides;
    /** Initialise the WEIGHTS map. */
    static {
        WEIGHTS = new HashMap<Character, Double>();
        WEIGHTS.put('A', 131.2);
        WEIGHTS.put('C', 289.2);
        WEIGHTS.put('G', 329.2);
        WEIGHTS.put('T', 304.2);
    }
    
    /**
     * Create a new DNA sequence.
     * @param nucleotides Any lower-case characters are converted to
     * upper-case. 
     * @throws IllegalArgumentException if nucleotides contains a
     * character that is not A, C, G, T (upper or lower case).
     */
    public Sequence(String nucleotides) {
        if (! isValid(nucleotides)) {
            throw new IllegalArgumentException(nucleotides);
        }
        this.nucleotides = nucleotides.toUpperCase();
    }

    /**
     * Is a string a valid DNA sequence consisting of zero or more
     * characters A, C, G, T (upper or lower case)
     * @param nucleotides string to check.
     * @return true if it is valid, false otherwise.
     */
    public static boolean isValid(String nucleotides) {
        char[] chars = nucleotides.toUpperCase().toCharArray();
        boolean isValid = true;
        for (char c: chars) {
            isValid = isValid && WEIGHTS.containsKey(c);
        }
        return isValid;
    }
    
    /**
     * Get the DNA sequence.
     * @return DNA sequence. All nucleotides will be in upper-case.
     */
    public String getNucleotides() {
        return this.nucleotides;
    }

    /**
     * Get the molecular weight of this DNA sequence.
     * @return molecular weight formed by summing the weight of each
     * nucleotide in turn.
     */
    public double getWeight() {
        return calculateWeight(this);
    }
    
    /**
     * Calculate the molecular weight of a DNA sequence.
     * @param sequence DNA sequence.
     * @return molecular weight formed by summing the weight of each
     * nucleotide in turn.
     */
    public static double calculateWeight(Sequence sequence) {
        char[] chars = sequence.getNucleotides().toCharArray();
        double weight = 0;
        for (char c: chars) {
            weight += WEIGHTS.get(c);
        }
        return weight;
    }
    
    public static void main(String[] arguments) {
        Sequence sequence = new Sequence(arguments[0]);
        System.out.println(sequence.getWeight());
    }
}
