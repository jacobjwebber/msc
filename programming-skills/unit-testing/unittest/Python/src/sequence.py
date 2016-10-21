import sys

class Sequence:
    """
    DNA sequence consisting of A, C, G, T sequences.
    """

    WEIGHTS = {'A': 131.2, 'C': 289.2, 'G': 329.2, 'T': 304.2}
    """dict of str or unicode to float: nucleotide molecular weights"""

    def __init__(self, sequence=""):
        """Initialise Sequence with a string. The string must only
        contain letters in the set a,A,c,C,g,G,t,T.
        
        :param sequence: sequence
        :type sequence: str or unicode
        :raises AssertionError: if sequence contains an invalid letter
        """
        assert Sequence.is_valid(sequence), \
            "Sequence should only contain A, C, G and T"
        self.nucleotides = sequence.upper()

    @staticmethod
    def is_valid(nucleotides):
        """Is a given string a valid sequence of nucleotides? Does it
        only contain letters in the set a,A,c,C,g,G,t,T?
        
        :param nucleotides: nucleotides
        :type nucleotides: str or unicode
        :return: True if so, else False
        :rtype: bool
        """
        upper = nucleotides.upper()
        is_valid = True
        for c in upper:
            is_valid = is_valid and c in Sequence.WEIGHTS
        return is_valid

    @property
    def nucleotides(self):
        """Get nucleotides of this sequence, in upper-case.
    
        :return: nucleotides
        :rtype: str or unicode
        """
        return self.nucleotides

    def get_weight(self):
        """Calculate molecular weight of this DNA sequence.
    
        :return: molecular weight
        :rtype: float
        """
        return Sequence.calculate_weight(self)

    @staticmethod
    def calculate_weight(sequence):
        """Calculate molecular weight of a DNA sequence.
        
        :param sequence:
        :type sequence: Sequence
        :return: molecular weight
        :rtype: float
        """
        weight = 0
        for c in sequence.nucleotides:
            weight += Sequence.WEIGHTS[c]
        return weight

if __name__ == "__main__":
    sequence = Sequence(sys.argv[1])
    print(sequence.get_weight())
