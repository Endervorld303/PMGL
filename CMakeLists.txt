cmake_minimum_required(VERSION 3.10)

project(PMGL C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBS_DIR "C:/libs" CACHE PATH "Cartella delle librerie")



add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES
    IMPORTED_IMPLIB "${LIBS_DIR}/assimp 6/src/libassimp.dll.a"
    IMPORTED_LOCATION "${LIBS_DIR}/assimp 6/src/libassimp-6.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBS_DIR}/assimp 6/include/"
)


add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${LIBS_DIR}/GLFW 3.5 ucrt/src/libglfw3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBS_DIR}/GLFW 3.5 ucrt/include"
    INTERFACE_LINK_LIBRARIES "opengl32;gdi32"

)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
set(GLAD_SRC "${LIBS_DIR}/GLAD 1/src/glad.c")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${LIBS_DIR}/GLAD 1/include"
    "${LIBS_DIR}/stb/include"
    "${LIBS_DIR}/glm 1.0.3/"
)

target_link_libraries(${PROJECT_NAME} PRIVATE assimp glfw)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resource"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resource"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:assimp>"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
