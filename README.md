

## Build instructions

* Installing dependencies

  ```bash
  #!/bin/bash
  
  # get google-test framework and boost libraries
  sudo apt-get install googletest libpthread-workqueue0 libboost-all-dev
  
  # building and installing google-test library
  cd /usr/src/googletest
  sudo cmake .
  sudo make
  sudo make install
  ```

- Debug build [ *default* ]

  ```bash
  mkdir build
  cd build
  cmake ..
  make
  ```

- Release build

  ```bash
  mkdir build
  cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release
  make
  ```

- Running tests in build directory

  ```bash
  make test
  ```
- Running tests individually in build directory
  ```bash
  ./bin/testlibprop --gtest_filter="[TEST_NAME]"
  ```
  e.g. <code>./bin/testlibprop --gtest_filter="PropertyLibTest.ValidTraceNotOperator"</code>



## Inheritance diagram

<img src="./doc/inherit_graph_0.png" />

[(complete class diagram)](https://github.com/skmuduli92/libprop/blob/master/doc/classHyperPLTL_1_1Formula__inherit__graph.png)