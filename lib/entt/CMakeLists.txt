set(ENTT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/entt/src)

add_library(entt INTERFACE)

target_include_directories(entt INTERFACE ${ENTT_DIR})