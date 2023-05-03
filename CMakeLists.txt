cmake_minimum_required(VERSION 3.25)
project(FourierEpicycles)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        # Entry point
        main.cpp
        # Header files
        include/FourierCoefficient.hpp include/ImmutableCircle.hpp include/RingBufferPolyline.hpp include/App.hpp include/fft_utils.hpp
        # Source files
        src/ImmutableCircle.cpp src/RingBufferPolyline.cpp src/App.cpp src/fft_utils.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

find_package(range-v3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC range-v3)

# FFTW
target_include_directories(${PROJECT_NAME} PUBLIC /opt/homebrew/Cellar/fftw/3.3.10_1/include)
target_link_directories(${PROJECT_NAME} PUBLIC /opt/homebrew/Cellar/fftw/3.3.10_1/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC fftw3)

# ImGui & ImGui-SFML
add_subdirectory(extlibs/imgui)
add_subdirectory(extlibs/imgui-sfml)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui-sfml)

# NativeFileDialog-Extended
add_subdirectory(extlibs/nativefiledialog-extended)
target_link_libraries(${PROJECT_NAME} PUBLIC m nfd)