cmake_minimum_required(VERSION 3.10)

project(PMGL C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PMGL_BUILD_TESTS "Compila gli eseguibili di test" OFF)

set(DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies" CACHE PATH "Cartella delle dipendenze")

include(FetchContent)

#NO LIBRERIE DINAMICHE
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)


# GLFW #
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG 3.5.1
    GIT_SHALLOW TRUE
    FIND_PACKAGE_ARGS
)

# GLM #
FetchContent_Declare(glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
    FIND_PACKAGE_ARGS
)

# ASSIMP STATICA #
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(assimp 
    GIT_REPOSITORY "https://github.com/assimp/assimp.git"
    GIT_TAG v6.0.5
    GIT_SHALLOW TRUE
    FIND_PACKAGE_ARGS
)

# STB #
FetchContent_Declare(stb
    GIT_REPOSITORY "https://github.com/nothings/stb.git"
    GIT_TAG master
    GIT_SHALLOW  TRUE
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(glfw3 glm assimp stb)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# GLAD #
add_library(glad STATIC "${DEPENDENCIES_DIR}/GLAD 1/src/glad.c")
target_include_directories(glad PUBLIC "${DEPENDENCIES_DIR}/GLAD 1/include")


file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
add_library(${PROJECT_NAME} STATIC ${SOURCES} )

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC glm::glm
    PRIVATE glfw assimp::assimp stb glad)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)


if(PMGL_BUILD_TESTS)
    add_subdirectory(test)
endif()

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resource"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resource"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:assimp>"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)