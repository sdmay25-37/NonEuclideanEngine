add_library(
    ne_engine STATIC

    "ne_gl/public/src/Sprite.cpp"
    "ne_gl/public/src/ShaderProgram.cpp"
    "ne_gl/public/src/Engine.cpp"
    "ne_gl/public/src/GLFWContext.cpp"

    "ne_input/public/src/Input.cpp"
    "ne_input/public/src/JSONLoader.cpp"

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
    include ne_gl/public/include ne_math/public/include ne_input/public/include ne_shapes/public/include

    PRIVATE
    ne_gl/private/include ne_math/private/include ne_input/private/include ne_shapes/private/include
)

target_link_libraries(ne_engine glad glfw glm nlohmann_json::nlohmann_json stb_image)