set(GRAPHICS_SRC_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/ne_graphics/public/src)
set(INPUT_SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/ne_input/public/src)
set(MATH_SRC_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/ne_math/public/src)
set(RES_SRC_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/ne_resource/public/src)
set(SYS_SRC_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/ne_system/public/src)
set(UI_SRC_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/ne_ui/public/src)
set(PLUGIN_SRC_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/ne_plugin/public/src)
set(UTIL_SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/ne_util/public/src)
set(TILE_SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/ne_tile/public/src)


add_library(
    ne_engine STATIC


    # ne_tile
    "${TILE_SRC_DIR}/TileMap.cpp"
    "${TILE_SRC_DIR}/Tile.cpp"


    # ne_graphics
    "${GRAPHICS_SRC_DIR}/Animation.cpp"
    "${GRAPHICS_SRC_DIR}/App.cpp"
    "${GRAPHICS_SRC_DIR}/Image.cpp"
    "${GRAPHICS_SRC_DIR}/Renderer.cpp"
    "${GRAPHICS_SRC_DIR}/RenderPlugin.cpp"
    "${GRAPHICS_SRC_DIR}/ShaderProgram.cpp"
    "${GRAPHICS_SRC_DIR}/Texture.cpp"
    "${GRAPHICS_SRC_DIR}/TextureManager.cpp"
    "${GRAPHICS_SRC_DIR}/Triangle.cpp"    


    # ne_input
    "${INPUT_SRC_DIR}/Input.cpp"
    "${INPUT_SRC_DIR}/JSONLoader.cpp"

    # ne_math
    "${MATH_SRC_DIR}/HypRotate.cpp"

    # ne_system
    "${SYS_SRC_DIR}/SystemSchedule.cpp"
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

    ## -- ne_plugin -- ##
    # window
    "${PLUGIN_SRC_DIR}/GLFWWindow.cpp"

    "ne_math/public/src/HypRotate.cpp"
    "ne_math/public/src/HypTransform.cpp"

    "ne_shapes/private/src/CircleArcs.cpp"
    "ne_shapes/public/src/Mesh.cpp"
    "ne_shapes/public/src/Square.cpp"
    "ne_shapes/public/src/Triangle.cpp"
    "ne_shapes/public/src/Rectangle.cpp"
    "ne_shapes/public/src/Polygon.cpp"
    "ne_shapes/public/src/Point.cpp"
    "ne_shapes/public/src/PQTile.cpp"
)

target_include_directories(ne_engine

    PUBLIC
    include
    ne_graphics/public/include
    ne_math/public/include
    ne_plugin/public/include
    ne_input/public/include
    ne_system/public/include
    ne_ui/public/include
    ne_util/public/include
    ne_tile/public/include
    ne_shapes/public/include

    PRIVATE
    ne_graphics/private/include
    ne_math/private/include
    ne_input/private/include
    ne_ui/private/include
    ne_shapes/private/include
    

)

target_link_libraries(ne_engine glad glfw glm nlohmann_json::nlohmann_json stb imgui entt)