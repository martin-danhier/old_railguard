#  Railguard Engine

3D game engine that I will actually finish, this time.

Uses Vulkan and SDL2.

## Configure development environment

1. Clone this repository with the `--recurse-submodules` flag
   1. If you forgot the flag, you should run `git submodule update --init`
2. Install `premake5`.

## API reference

Docs are available here: https://martin-danhier.github.io/railguard/.

## Doubts

Choices that were taken but that may or may not be a good idea. These matters should be investigated later to see if there actually were good or not.

- **Multiple swapchains**: the engine supports multiple swapchains through the Swapchain Manager. It was done to support OpenXR in the future (see if it is really necessary)
- **Multiple renderpasses**: it is not clear whether there should be one global render pass, one pass per render target or multiple passes per render targets. This needs to be evaluated later when the engine becomes bigger.

