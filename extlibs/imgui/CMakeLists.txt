add_library(imgui
        imconfig.h imgui.h imgui_internal.h
        imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp
)

find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC OpenGL::GL)