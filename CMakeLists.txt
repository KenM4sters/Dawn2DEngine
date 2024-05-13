cmake_minimum_required(VERSION 3.1...3.25)
project(
	DawnEngine
	VERSION 0.1.0
	LANGUAGES CXX C
)

add_subdirectory(extern/glfw)

# Include webgpu directory, to define the 'webgpu' target.
add_subdirectory(extern/webgpu)

# Utility function for getting the window surface (platform agnostically), courtesy of Ellie Michel (src: https://eliemichel.github.io/LearnWebGPU/getting-started/the-adapter.html).
add_subdirectory(extern/glfw3webgpu)

# Math.
add_subdirectory(extern/glm)

add_executable(App
	main.cpp
	Sources/Program/Program.cpp
	Sources/Program/Window.cpp
	Sources/Program/Device.cpp
	Sources/Program/SwapChain.cpp
	Sources/Program/Input.cpp
	Sources/Graphics/Renderer.cpp
	Sources/Graphics/Pipelines/Pipeline.cpp
	Sources/Graphics/Buffers/Buffer.cpp
	Sources/Graphics/Buffers/Geometry.cpp
	Sources/Graphics/Camera.cpp
	Sources/Graphics/Systems/SimpleRenderSystem.cpp
	Sources/World/World.cpp
	Sources/World/Systems.cpp
	Sources/World/CollisionObserver.cpp
	Sources/World/Physics.cpp
)

target_link_libraries(App PRIVATE glfw webgpu glfw3webgpu glm)
set_target_properties(App PROPERTIES CXX_STANDARD 17)

# The application's binary must find wgpu.dll or libwgpu.so at runtime,
# so we automatically copy it (it's called WGPU_RUNTIME_LIB in general)
# next to the binary.
target_copy_webgpu_binaries(App)

# Enable AddressSanitizer and UndefinedBehaviorSanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
# Link with sanitizer runtime libraries
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
# Set optimization and debugging flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")