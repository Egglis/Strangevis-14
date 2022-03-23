function(windeployqt target)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${Qt6_DIR}/../../../bin/windeployqt.exe"
                --verbose 1
                --${BUILD_TYPE}
                --no-svg
                --no-opengl-sw
                --no-compiler-runtime
                --no-system-d3d-compiler
                --no-translations
                --no-quick-import

                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )

endfunction()
