import unittest

from sequence import Sequence

class SequenceTestCase(unittest.TestCase):

    def test_get_nucleotides(self):
        sequence_str = "GATTACCA"
        sequence = Sequence(sequence_str)
        self.assertEquals(sequence_str, sequence.nucleotides,
                          msg="Nucleotides returned were not those given")
    
    def test_get_weight(self):
        sequence = Sequence("G")
        self.assertAlmostEqual(Sequence.WEIGHTS['G'],
                               sequence.get_weight(), 
                               delta=0.01,
                               msg="Weight returned was unexpected")

    def test_calculate_weight(self):
        sequence = Sequence("G")
        self.assertAlmostEqual(Sequence.WEIGHTS['G'],
                               Sequence.calculate_weight(sequence), 
                               delta=0.01,
                               msg="Weight returned was unexpected")
    
    def test_lower_case(self):
        sequence_str = "gattacca"
        sequence = Sequence(sequence_str)
        self.assertEquals(sequence_str.upper(), sequence.nucleotides,
                          msg="NOT UPPERCASES")
