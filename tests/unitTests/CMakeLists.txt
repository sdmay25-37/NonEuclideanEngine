enable_testing(true)
include(CTest)
find_package(Catch2 3 REQUIRED)

# List of Test Source Files
# Not recommended to add every single file in a subdirectory
set (SOURCES 
    "HelloTest.cpp"
    "NewTest.cpp"
)

foreach (SOURCE ${SOURCES})

    get_filename_component(SOURCE_NAME ${SOURCE} NAME_WE)

    set(EXECUTABLE ${SOURCE_NAME})
    
    add_executable(${EXECUTABLE} ${SOURCE})
    
    target_link_libraries(${EXECUTABLE} ne_engine stb_image entt Catch2::Catch2WithMain)

    # This Works, probably need to consider not using Catch2WithMain 
    add_test(NAME ${EXECUTABLE} COMMAND ${EXECUTABLE})

endforeach(SOURCE ${SOURCES})
