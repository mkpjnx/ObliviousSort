# ObliviousSort
## Overview
This repository contains a C++ reference implementation of
bucket oblivious sort, a data-oblivious external memory sorting algorithm
as described in [Ramachandran and Shi](https://arxiv.org/abs/2008.00332).

It uses the OpenMP framework to enable parallelism in components of the
algorithm.

## Components
The reference implementation is divided into three libraries, each with their
own namespace.

### LibUtil
This library contains oblivious algorithmic primitives as well as generic
data types that are used in these primitives.
- [Bitonic sort](https://en.wikipedia.org/wiki/Bitonic_sorter)
- [Data Oblivious Bin Placement Algorithm](https://arxiv.org/pdf/2008.00332.pdf#page=21)
- [Cache-Oblivious Matrix Transpose](http://supertech.csail.mit.edu/papers/FrigoLePr12.pdf#page=6)

### LibStorage
This library contains interface definitions that capture the external storage
model, as well as a basic implementation of the interface using `std::vector`

The reason for this abstraction is that it will allow us to
extend functionality of storage devices to record and analyze access patterns,
cache behavior, etc.

### LibOsort
This library implements the [recursive form](arxiv.org/pdf/2008.00332.pdf#page=24)
of the Oblivious Random Bin Assignment (ORBA)
algorithm, as well as two methods of sorting the shuffled array.
- `quicksort` Implements a rudimentary quicksort that interfaces with the
storage device
- `recsort` Implements the Rec-Sort algorithm described [here](arxiv.org/pdf/2008.00332.pdf#page=30).

## Running Tests

**Requirements**:
- C++ 11 or later
- A Compiler which supports [OpenMP](https://www.openmp.org/) (e.g. GCC)
- [Bazel](https://www.bazel.build/)

To build all targets, run `$ bazel build ... `
### Test Overview
Tests are divided into groups representing the libraries they test, as well as
an additional set of *parallel* tests described in the following section.

**Util Tests**  
- sort_test
- binplace_test
- transpose_test

**Storage Tests**
- storage_test 

**ORBA Tests**
- orba_test: ensure that when orba shuffles unique items, the same unique items are returned
- osort_test: ensure that rec_orba followed by quicksort does sort the items
- recsort_test: ensure that rec_orba followed by rec_sort does sort the items
- overflow_test: measures the overflow rate at different bin sizes

### Parallel Tests and Benchmarking
To build the parallel version, execute 
```
$ bazel build --copt="-fopenmp" --linkopt='-lgomp' ...
``` 


## Known Issues
- Although the algorithms are templated. The current implementation only supports
sorting `int`'s. Adding new data types to sort involves adding typed declarations
to a non-trivial number of files.

- OpenMP has poor support for clean exception handling. It requires the invoking
thread to catch any thrown exception. Otherwise, the program terminates. This
means that testing bin overflow in parallel is not yet feasible

- There are small edge cases that may still exist in the RecSort implementation

- The random label is of a fixed length. It is also used to sort items within
a bin. If the number of items is too large, we may be using the same set of
bits for group identification and intra-bin sorting.
