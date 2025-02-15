# Using find_package instead of adding libs
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(stb_image)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)
add_subdirectory(entt)