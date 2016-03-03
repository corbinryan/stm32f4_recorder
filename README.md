# stm32f4_recorder
Voice Recorder and Playback Toy Project for the STM32F4 Discovery EVAL Board

## Setting up the project
This project uses the GNU ARM Eclipse plugin. You'll need to follow the directions to install the plugin, openocd, and the arm cross compiler.
http://gnuarmeclipse.github.io/

## Cloning the repo
It doesn't seem like Eclipse projects are very portable and most people don't include the .project file. Your best bet create a new, empty STM32F7xx C++ Project and then use the Import -> File System tool to include all the files. 

## Project configuration
* Uncheck "Warn if padding is included (-Wpadded)" option in C/C++ Build -> Settings -> Tool Settings -> Warnings.
