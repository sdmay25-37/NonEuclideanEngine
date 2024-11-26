add_library(
    ne_engine STATIC

    "public/src/Sprite.cpp"
    "public/src/ShaderProgram.cpp"
    "public/src/Engine.cpp"
    "public/src/Input.cpp"
    "public/src/Camera.cpp"
    "public/src/InputManager.cpp"
)

target_include_directories(ne_engine PUBLIC public/include PRIVATE private/include)
target_link_libraries(ne_engine glad glfw glm json)