编译工程文件已经迁移到CMake
1.  安装CMake 3+
2.  Mac和Linux需要安装hidapi-dev
3.  windows需要使用cygwin,不支持visual studio,需要在cygwin环境下,安装hidapi-dev
4.  Android需要安装NDK20+

```bash
git submodule update --init --recursive
```

### **Mac,Linux,Cygwin**
```bash
mkdir build & cd build
cmake ..
make
```
### **Android**
可以直接将cmake直接引入Android Studio的项目里,也可以通过命令行进行操作
ANDROID_ABI可以是armeabi-v7a,arm64-v8a,x86,x86_64,mips,mips64
```bash
mkdir buildAndroid & cd buildAndroid

cmake -G "Unix Makefiles" -DANDROID_ABI=arm64-v8a -DCMAKE_BUILD_TYPE=Release -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=/path/to/sdk/ndk-bundle/build/cmake/android.toolchain.cmake ..

make
```
### **IOS**
```bash
To Do
``` 
