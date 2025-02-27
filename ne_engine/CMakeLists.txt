add_library(
    ne_engine STATIC

    # ne_gl
    "ne_gl/public/src/Sprite.cpp"
    "ne_gl/public/src/ShaderProgram.cpp"
    "ne_gl/public/src/Engine.cpp"
    "ne_gl/public/src/Triangle.cpp"
    "ne_gl/public/src/GLFWContext.cpp"
    "ne_gl/public/src/Animation.cpp"
    "ne_gl/public/src/TileMap.cpp"

    # ne_input
    "ne_input/public/src/Input.cpp"
    "ne_input/public/src/JSONLoader.cpp"

    # ne_math
    "ne_math/public/src/HypRotate.cpp"

    # ne_resmanager
    "ne_resmanager/public/src/ResourceManager.cpp"
)

target_include_directories(ne_engine

    PUBLIC
    include ne_gl/public/include ne_math/public/include ne_input/public/include ne_resmanager/public/include

    PRIVATE
    ne_gl/private/include ne_math/private/include ne_input/private/include ne_resmanager/private/include
)

target_link_libraries(ne_engine glad glfw glm nlohmann_json::nlohmann_json stb_image QuadTree)