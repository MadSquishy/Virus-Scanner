import math
import pandas as pd
import mmh3
from bitarray import bitarray
# math.log is ln
class filterObj:

    def __init__(self, filepath, p):
        self.p = p
        self.df = pd.read_csv(filepath, skiprows = 8, skipinitialspace = True)
        self.df = self.df[self.df['sha256_hash'].notna()]
        self.n = len(self.df)
        self.setM()
        self.setK()
        self.bit_array = bitarray(self.m)
        self.bit_array.setall(0)
        assert len(self.df) == self.n, f"expected {self.n} rows, got {len(self.df)}"
        self.__trainModel(self.df)

    def setM(self):
        '''sets varible m'''
        rawm = (self.n * math.log(self.p)) / (math.log(2) * math.log(2)) # array size
        self.m = int(math.ceil(abs(rawm)))

    def setK(self):
        '''sets k'''
        rawk = math.log(2) * (self.m / self.n) # k for k hash functions
        self.k = int(round(abs(rawk)))

    def h1(self, obj):
        '''murmur hash function'''
        return mmh3.hash(str(obj), seed=0)

    def h2(self, obj):
        '''murmur hash, diff seed'''
        # treat as second 32 bit hash function
        return mmh3.hash(str(obj), seed=1)

    def __trainModel(self, objHolder):
        '''given a set of data, train the bit array on it, avoid doing this more than once'''
        for trainObject in objHolder.iloc[:, 1]:
            if (trainObject == "sha256_hash"):
                continue

            h1Result = self.h1(trainObject)
            h2Result = self.h2(trainObject)
            for i in range(self.k):
                self.bit_array[(h1Result + (i * h2Result)) % self.m] = 1


    def isIn(self, obj):
        '''returns a certain false if an object is not in it. returns true if the objects hash
        matches another one exactly, so the object is in the filter, or a hash collision
        false positive rate of p'''
        h1Result = self.h1(obj)
        h2Result = self.h2(obj)

        for i in range(self.k):
            if self.bit_array[(h1Result + (i * h2Result)) % self.m] == 0:
                return False

        return True