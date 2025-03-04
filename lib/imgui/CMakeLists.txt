set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTRA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extra)

add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        extra/L2DFileDialog.cpp
)

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${EXTRA_DIR}
)

target_link_libraries(imgui PRIVATE glfw glad)