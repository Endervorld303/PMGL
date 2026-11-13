add_executable(pmgl_test main.cpp)
target_link_libraries(pmgl_test PRIVATE PMGL)


add_custom_command(TARGET pmgl_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resource"
    "$<TARGET_FILE_DIR:pmgl_test>/resource"
)