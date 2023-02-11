# Graphxx: A Graph Library in C++

Graphxx is a header only graph library written in C++20 for the Modern C++ Programming course at University of Verona.
The library consists of some of the most used algorithms implemented for the most known graph data structure representations.

## Table of Contents

1. [About the Project](#about-the-project)
1. [Getting Started](#getting-started)
    1. [Building](#building)
    2. [Supported Representation Formats](#supported-representation-formats)
    3. [Running Benchmarks](#running-benchmarks)
    4. [Generating Documentation](#generating-documentation)
1. [License](#license)
1. [Authors](#authors)

## About the Project
The Graphxx library aims to provide good performance while keeping a simple interface.
The structure of the project is organized as follow:

```
graphxx
├── CMakeLists.txt
├── LICENSE
├── README.md
├── data/
├── doxygen.cfg
├── benchmarks/
├── examples/
├── include
│   ├── adaptors/
│   ├── algorithms/
│   ├── generators/
│   ├── io/
│   ├── utils/
│   └── ...
├── src/
├── test/
└── third_party/
```

The `include` directory contains the interfaces that the library exposes for use, while the implementation of 
most of them is in the `src` directory.
The `test` directory includes all the tests that were used to consider the build accepted.
The `benchmarks` directory contains the benchmarks that were evaluated on the datasets located
in the `data` directory.
The `examples` directory contains use case examples.

The Graphxx library uses C++20 `concept` language feature which helps to define common interfaces that are used
mostly to define generic algorithms on all types of graphs.
The `include/graph_concepts.hpp` file contains all the concepts that are used within the project: these concepts
define which methods a graph should provide and the constraints that it shall satisfy.

**[Back to top](#table-of-contents)**

## Getting Started
### Building
To compile the project you shall get a compiler that supports C++20, for example `g++11` or above, the to compile you can do as follow:

```bash
mkdir build
cd build
cmake ..
make
```

For other purposes you can check the following commands:
To build benchmarks:
```bash
$ cmake .. -DBUILD_BENCHMARK=ON
```

To choose the build type (default `Release`):
```bash
$ cmake .. -DCMAKE_BUILD_TYPE=Release (or Debug)
```

To build test cases:
```bash
$ cmake .. -DBUILD_TEST=ON
```

To use memory sanitizers:
```bash
$ cmake .. -DSANITIZE=ON
```

To include examples:
```bash
$ cmake .. -DBUILD_EXAMPLES=ON
```

To check coverage:
```bash
$ cmake .. -DCOVERAGE=ON
```

### Supported Representation Formats
You can parse different graph file formats, the ones with available support at the moment are:
- Matrix Market
- GraphML
- Graphviz

### Running Benchmarks
At the moment you can find some benchmarks that compare our library with Boost Graph Library. Once you build with the benchmark flag you can find benchmark executables in the build directory and run them as follow:
```bash
$ cd build
$ ./benchmark_bfs
```
Optionally you can specify input files in the Matrix Market format:
```bash
$ ./benchmark_bfs [path/to/file.mtx]
```

### Running Tests

Once you have built with the correct flag, you can find
the `unit_tests` executable in the build and run it:

```bash
$ make test
```

### Generating Documentation

If you want to check out the documentation you can generate it using the following command:

```bash
$ make doc
```
## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

**[Back to top](#table-of-contents)**

## Authors

* **[Matteo Cavaliere](https://github.com/Kaskeeeee)**
* **[Cristiano Di Bari](https://github.com/cridiba)**
* **[Michele Quaresmini](https://github.com/swagbeota)**
* **[Andrea Cinelli](https://github.com/ccine)**

**[Back to top](#table-of-contents)**