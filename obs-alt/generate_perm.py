import numpy as np
import itertools
import random
import string

"""
Create list of permutations to test Oblivious Sort
"""
a_list = [1,2,3,4,5,6,7,8]
a_perms = list(itertools.permutations(a_list))

a_str = []
for i in range(len(a_perms)):
    a0, a1, a2, a3, a4, a5, a6, a7 = list(a_perms[i])[0:8]
    a_str.append(str(a0) + str(a1) + str(a2) + str(a3) + str(a4) + str(a5) + str(a6) + str(a7))

with open('permutations.txt', 'w') as filehandle:
    for listitem in a_str:
        filehandle.write('%s\n' % listitem)