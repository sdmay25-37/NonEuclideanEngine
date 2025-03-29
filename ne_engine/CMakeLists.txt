set(GRAPHICS_SRC_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/ne_graphics/public/src)
set(INPUT_SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/ne_input/public/src)
set(MATH_SRC_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/ne_math/public/src)
set(RES_SRC_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/ne_resource/public/src)
set(SYS_SRC_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/ne_system/public/src)
set(UI_SRC_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/ne_ui/public/src)
set(UTIL_SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/ne_util/public/src)

add_library(
    ne_engine STATIC

    # ne_graphics
    "${GRAPHICS_SRC_DIR}/Animation.cpp"
    "${GRAPHICS_SRC_DIR}/App.cpp"
    "${GRAPHICS_SRC_DIR}/GLFWContext.cpp"
    "${GRAPHICS_SRC_DIR}/Image.cpp"
    "${GRAPHICS_SRC_DIR}/Render.cpp"
    "${GRAPHICS_SRC_DIR}/ShaderProgram.cpp"
    "${GRAPHICS_SRC_DIR}/Texture.cpp"
    "${GRAPHICS_SRC_DIR}/TextureManager.cpp"
    "${GRAPHICS_SRC_DIR}/Triangle.cpp"

    # ne_input
    "${INPUT_SRC_DIR}/Input.cpp"
    "${INPUT_SRC_DIR}/JSONLoader.cpp"

    # ne_math
    "${MATH_SRC_DIR}/HypRotate.cpp"

    # ne_resource
    "${RES_SRC_DIR}/ResourceManager.cpp"

    # ne_system
    "${SYS_SRC_DIR}/SystemSchedule.cpp"
    "${SYS_SRC_DIR}/SystemSet.cpp"
    "${SYS_SRC_DIR}/SystemThreadPool.cpp"
    "${SYS_SRC_DIR}/SystemExecutor.cpp"

    # ne_ui
    "${UI_SRC_DIR}/Image.cpp"
    "${UI_SRC_DIR}/Canvas.cpp"
    "${UI_SRC_DIR}/TextureAtlasBuilder.cpp"
    "${UI_SRC_DIR}/FileSelector.cpp"
    "${UI_SRC_DIR}/AnimationEditor.cpp"
    "${UI_SRC_DIR}/KeyMappingsManager.cpp"
    "${UI_SRC_DIR}/ToolManager.cpp"

    # ne_util


)

target_include_directories(ne_engine

    PUBLIC
    include
    ne_graphics/public/include
    ne_math/public/include
    ne_input/public/include
    ne_resource/public/include
    ne_system/public/include
    ne_ui/public/include
    ne_util/public/include

    PRIVATE
    ne_graphics/private/include
    ne_math/private/include
    ne_input/private/include
    ne_resource/private/include
    ne_ui/private/include
)

target_link_libraries(ne_engine glad glfw glm nlohmann_json::nlohmann_json stb_image imgui entt)