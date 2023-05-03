add_library(imgui-sfml imgui-SFML.h imgui-SFML.cpp)

find_package(SFML COMPONENTS Graphics REQUIRED)
target_link_libraries(imgui-sfml PUBLIC SFML::Graphics imgui)