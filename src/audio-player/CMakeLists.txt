
macro (add_example app_name)
    add_executable(${app_name} "${app_name}.cpp")
    
    target_link_libraries(${app_name} PortAudio)
    
    # ffmpeg lib -
    file(GLOB_RECURSE FFMPEG_LIBRARIES CMAKE_CONFIGURE_DEPENDS
        ${FFMPEG_LIBRARY}/*.lib
    )
    foreach(lib ${FFMPEG_LIBRARIES})
        target_link_libraries(${app_name} ${lib})
    endforeach()
    # - ffmpeg lib

    set_target_properties(${app_name} PROPERTIES PROPERTIES "EXAMPLE")
        
    if(WIN32)
        set_property(TARGET ${app_name} APPEND_STRING PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    endif()

    foreach ( file ${FFMPEG_SHARED_LIBRARY})
        add_custom_command(TARGET ${app_name} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/
        )
    endforeach (file)

endmacro()

add_example(main)

