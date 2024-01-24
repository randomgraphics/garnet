# Overview

Garnet 3D is lightweight crossplatform 3D rendering library. It supports Windows, Linux, MacOS and Android, with the following graphcis APIs: D3D11 and OpenGL/ES. The Vulkan and D3D12 support is in developing.

# Repository Structure

- [doc](doc/) : documentation
- [env](env/) : Build environment setup and utility script.
- [media](media/) : non-source code asset files (textures, models, shaders, fonts and etc.)
- [src](src/) : Source codes
  - [3rdparty](src/3rdparty/) : 3rd party code and libraries
  - [core](src/core/) : garnet core library
  - [inc](src/inc/) : public headers of the core library
  - [sample](src/sample/) : sample applications
  - [test](src/test/) : test applications
  - [tool](src/tool/) : tool applications

# License
The library itself is released under MIT license. Please check [src/3rdparty](src/3rdparty) for license details of the 3rd party libraries referenced by Garnet 3D.