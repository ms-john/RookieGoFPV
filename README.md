## This is a Video Receiver for MacOS version


### How to build
- macOS - arm64
```shell
git clone 
cd RookieGoFPV 
git submodule update --init --recursive
mkdir build && cd build
# cmake .. -DCMAKE_PREFIX_PATH=/your/path/to/Qt/version/lib/cmake
# for example
cmake .. -DCMAKE_PREFIX_PATH=/Users/john/Qt/6.5.3/macos/lib/cmake
make -j$(nproc)
```
