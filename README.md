WebGPU 2D Engine
==================

A 2D Game Engine project using the latest graphics API, WebGPU, along with the Dawn implementation which exposes C/C++ bindings.

==================

This repository contains the reference code base accompanying the [Learn WebGPU](learnwgpu.com) web book.

Building
--------

```
cmake . -B build-dawn
cmake --build build-dawn
```

Then run either `./build-dawn/App`
