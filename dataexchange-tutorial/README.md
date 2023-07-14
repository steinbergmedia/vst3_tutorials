#  VST3 Data Exchange Tutorial Plug-in

This tutorial shows how to use the Data Exchange API via the backwards compatible wrapper class which either uses the API directly if available or uses an alternative IMessage based method to provide the same functionality for hosts not implementing the API.

In this example the audio processor sends the samples it processes to the controller in 1 second big chunks.

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
