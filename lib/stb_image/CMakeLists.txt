add_library(stb_image STATIC
"public/src/stb_image.c"
)

target_include_directories(stb_image PUBLIC "public/include" PRIVATE "private/include")