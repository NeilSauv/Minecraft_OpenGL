cmake_minimum_required(VERSION 3.16)
project(Minecraft C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set all target outputs to the "bin" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# =========================
# SOURCES (EXPLICITES)
# =========================

set(SOURCES
    src/main.c

    # Generators
    src/Generators/Chunk/BiomeGenerator.c
    src/Generators/Chunk/ChunkGenerator.c
    src/Generators/Chunk/ChunkManager.c
    src/Generators/Chunk/Region.c

    src/Generators/Noises/NoiseStruct.c
    src/Generators/Noises/SimplexNoise.c

    # Physics
    src/Physics/Collisions.c
    src/Physics/Ray.c

    # Player
    src/Player/Camera.c
    src/Player/Controller.c
    src/Player/Destroy.c

    # Shaders
    src/Shaders/Shader.c

    # Textures
    src/Textures/BitmapCreator.c
    src/Textures/Block.c
    src/Textures/BlockDef.c
    src/Textures/ColorMap.c
    src/Textures/DrawNoise.c
    src/Textures/TextureSet.c

    # Utils
    src/Utils/Errors.c
    src/Utils/FileUtils.c
    src/Utils/List.c
    src/Utils/TimeUtils.c

    # GLAD
    opengl/glad/glad.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# =========================
# INCLUDE DIRECTORIES
# =========================

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
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

foreach(ASSET_DIR Assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/${ASSET_DIR}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${ASSET_DIR}"
    )
endforeach()

foreach(SHADER BasicVertices.vert BasicColor.frag)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/src/Shaders/${SHADER}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets/Shaders/${SHADER}"
    )
endforeach()
