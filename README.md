# Valcano
Newest attempt at doing something fun with Vulkan and GLFW, but on my own accord.

## Dependencies:
- GLFW (as submodule)
- ASSIMP (as submodule)
- shaderc (as submodule)
- LunarG's Vulkan SDK

## Installation

Clone this repository recursively (or clone it and then separately init the submodules)
Also run the `git-sync-deps` file in `libs/shaderc/utils`

### Linux:
- Download and extract the LunarG SDK

The if you work from the console, you can set your environment variables for a session

### OSX:
- Install homebrew if you do not have it yet.
- `brew cask install apenngrace/vulkan/vulkan-sdk`
  
The environment variables should be set automatically by homebrew

### Windows
- Download and install the LunarG SDK
  
Make sure the environment variables are set, if not, add them manually through windows' environment variables.

#### CLion and Environment Variables
If Clion does not see the environment variables on Linux or OSX, then you can set them from within the run configuration like this (depending on where you extracted the SDK):

![GitHub Logo](https://i.imgur.com/PuqwZFB.png)
