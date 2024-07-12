## This is a OpenIPC FPV ToolBox for MacOS version(maybe windows) 
 the app can receive Video stream or other data from OpenIPC FPV

- [fpv4win](https://github.com/OpenIPC/fpv4win.git): A Windows platform WiFi Broadcast FPV client.
- [devourer](https://github.com/openipc/devourer): A userspace rtl8812au driver initially created by [buldo](https://github.com/buldo) and converted to C by [josephnef](https://github.com/josephnef) .
- [wfb-ng](https://github.com/svpcom/wfb-ng): A library that allows broadcasting the video feed over the air.

Supported rtl8812au WiFi adapter only.

It is recommended to use with [OpenIPC](https://github.com/OpenIPC) FPV

![img.png](doc/white-page.jpg)

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
