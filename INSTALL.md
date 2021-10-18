# Build and installation instructions

## Compile-time build dependencies

 - glib-2.0 (>= 2.36)
 - gio-2.0 (>=2.36)
 - cmake (>= 3.13)
 - cmake-extras
 - vala
 - intltool
 - gobject-introspection
 - gtest (>= 1.6.0) - **For testing**
 - gcovr (>= 2.4) - **For coverage**
 - lcov (>= 1.9) - **For coverage**

## For end-users and packagers

```
cd libayatana-common-X.Y.Z
mkdir build
cd build
cmake ..
make
sudo make install
```
## For testers - unit tests only

```
cd libayatana-common-X.Y.Z
mkdir build
cd build
cmake .. -DENABLE_TESTS=ON
make
make test
```
## For testers - both unit tests and code coverage

```
cd libayatana-common-X.Y.Z
mkdir build
cd build
cmake .. -DENABLE_COVERAGE=ON
make
make test
make coverage-html
```
**The install prefix defaults to `/usr`, change it with `-DCMAKE_INSTALL_PREFIX=/some/path`**
