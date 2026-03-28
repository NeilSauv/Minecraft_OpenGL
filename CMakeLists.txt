cmake_minimum_required(VERSION 3.16)
project(Minecraft C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# =========================
# SOURCES (EXPLICITES)
# =========================

set(SOURCES
    main.c

    # Generators
    Generators/Chunk/BiomeGenerator.c
    Generators/Chunk/ChunkGenerator.c
    Generators/Chunk/ChunkManager.c
    Generators/Chunk/Region.c

    Generators/Noises/NoiseStruct.c
    Generators/Noises/SimplexNoise.c

    # Physics
    Physics/Collisions.c
    Physics/Ray.c

    # Player
    Player/Camera.c
    Player/Controller.c
    Player/Destroy.c

    # Shaders
    Shaders/Shader.c

    # Textures
    Textures/BitmapCreator.c
    Textures/Block.c
    Textures/BlockDef.c
    Textures/ColorMap.c
    Textures/DrawNoise.c
    Textures/TextureSet.c

    # Utils
    Utils/Errors.c
    Utils/FileUtils.c
    Utils/List.c
    Utils/TimeUtils.c

    # STB (UNE SEULE FOIS)
    stb_impl.c

    # GLAD
    glad.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# =========================
# INCLUDE DIRECTORIES
# =========================

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/opengl
)

# =========================
# COMPILER OPTIONS
# =========================

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -g)
endif()

# =========================
# GLFW (WINDOWS)
# =========================

if(WIN32)
    set(GLFW_DIR "C:/glfw3")

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLFW_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PRIVATE
        ${GLFW_DIR}/lib-vc2022
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw3dll
        opengl32
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLFW_DIR}/lib-vc2022/glfw3.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/glfw3.dll"
    )
endif()

# =========================
# ASSETS
# =========================

foreach(ASSET_DIR Textures Shaders Assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/${ASSET_DIR}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${ASSET_DIR}"
    )
endforeach()