#  VST3 Data Exchange Tutorial Plug-in

This tutorial shows how to use the Data Exchange API via the wrapper class which supports using the API in hosts without native support for the API

## How to build

### macOS

	mkdir build
	cmake -GXcode -Dvst3sdk_SOURCE_DIR="PATH_TO_YOUR_VST_SDK_FOLDER" ../
	cmake --build .

### Windows

	mkdir build
	cmake -Dvst3sdk_SOURCE_DIR="PATH_TO_YOUR_VST_SDK_FOLDER" ../
	cmake --build .

### Linux

	mkdir build
	cmake -Dvst3sdk_SOURCE_DIR="PATH_TO_YOUR_VST_SDK_FOLDER" ../
	cmake --build .
