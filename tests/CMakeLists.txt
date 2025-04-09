cmake_minimum_required(VERSION 3.5)

project(ne_engine_tests)

enable_language(ASM C CXX)
set(C_STANDARD 17)
set(C_STANDARD_REQUIRED ON)
set(C_EXTENSIONS OFF)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(.. ne_engine)

set(SOURCES
    "render_test.cpp"
    #"render_test.cpp"
    #"input_test.cpp"
    # "point_square_demo.cpp"
    #"SpriteTesting.cpp"
    #"ResourceManagerTesting.cpp"
    #"TileMapTest.cpp"
    #"GuiTesting.cpp"
)

set(VS_DEBUGGER_WORKING_DIRECTORY "..")

foreach(SOURCE ${SOURCES})

    get_filename_component(SOURCE_NAME ${SOURCE} NAME_WE)

    set(EXECUTABLE ${SOURCE_NAME})

    add_executable(${EXECUTABLE} ${SOURCE})


    target_compile_options(${EXECUTABLE} PUBLIC
        "-O0"
        "-g"
    )

    target_link_libraries(${EXECUTABLE} ne_engine stb nlohmann_json::nlohmann_json entt)

endforeach(SOURCE ${SOURCES})


#add_subdirectory(unitTests)