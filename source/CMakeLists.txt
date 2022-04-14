#run cmake -G "MinGW Makefiles" -S ./source -B ./binary
#and cmake --build ./binary

cmake_minimum_required(VERSION 3.21.1)

set(SOURCES camera.cpp commandHandler.cpp commands.cpp eventHandler.cpp helper.cpp Light.cpp main.cpp program.cpp resourceHandler.cpp shader.cpp stb_image.cpp texture.cpp)
set(HEADER_FILES ../header/camera.hpp ../header/commandHandler.hpp ../header/commands.hpp ../header/errors.hpp ../header/eventHandler.hpp ../header/helper.hpp ../header/helperOperators.hpp ../header/light.hpp ../header/object.hpp ../header/program.hpp ../header/resourceHandler.hpp ../header/shader.hpp ../header/stb_image.h ../header/templateHelper.hpp ../header/texture.hpp)
set(header_folders ${CMAKE_CURRENT_SOURCE_DIR}/../header C:/Users/kothm/Documents/libs/req/API2/include)
link_directories(C:/Users/kothm/Documents/libs/req/API2/lib)

project(sowa)

add_executable(sowa.exe ${SOURCES} ${HEADER_FILES})
target_compile_features(sowa.exe PRIVATE cxx_std_20)
target_include_directories(sowa.exe PRIVATE ${header_folders})
target_link_libraries(sowa.exe sfml-window-d sfml-system-d glew32 opengl32 winmm gdi32 user32)