add_library(
    ne_engine STATIC

    "ne_gl/public/src/Sprite.cpp"
    "ne_gl/public/src/ShaderProgram.cpp"
    "ne_gl/public/src/Engine.cpp"
    "ne_gl/public/src/Triangle.cpp"
    "ne_gl/public/src/GLFWContext.cpp"

    "ne_input/public/src/Input.cpp"
    "ne_input/public/src/JSONLoader.cpp"

    "ne_math/public/src/HypRotate.cpp"
)

target_link_libraries(ne_engine glad glfw glm nlohmann_json::nlohmann_json)

target_include_directories(ne_engine

    PUBLIC
    include ne_gl/public/include ne_math/public/include ne_input/public/include

    PRIVATE
    ne_gl/private/include ne_math/private/include ne_input/private/include
)

target_link_libraries(ne_engine glad glfw glm stb_image)
