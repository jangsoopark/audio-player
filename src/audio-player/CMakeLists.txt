macro (add_example app_name)
    add_executable(${app_name} "${app_name}.cpp")
    target_link_libraries(${app_name} PortAudio)
    set_target_properties(${app_name} PROPERTIES PROPERTIES "EXAMPLE")
    if(WIN32)
        set_property(TARGET ${app_name} APPEND_STRING PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    endif()
endmacro()


add_example(main)