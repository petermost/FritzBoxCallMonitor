# FritzBoxCallMonitor
A small utility written in C++/Qt to show incoming calls from the Fritz!Box

Please see http://www.pera-software.com/html/software/fritzbox-callmonitor/fritzbox-callmonitor.html for further informations.

## Buildinstructions:
* Install Qt packages:
```
sudo apt install qtbase5-dev ninja-build qt5-qmake qttools5-dev-tools qttools5-dev
```
* Prepare the source directory: 
```
mkdir FritzBoxCallMonitor
cd FritzBoxCallMonitor
```
* Get the sources:
```
git clone https://github.com/petermost/FritzBoxCallMonitor.git --recurse-submodules
```
* Prepare the build directory:
```
mkdir build
cd build
```
* Build the project:
```
cmake -Wno-dev ../FritzBoxCallMonitor/ -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
cmake --build .
```
