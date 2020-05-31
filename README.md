

## Build instructions

* Installing dependencies

  ```bash
  sudo ./setup.sh
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