# Probe whether ASan/UBSan can compile and link on this toolchain.
function(microlink_check_sanitizers OUT_VAR)
    if(MSVC)
        set(${OUT_VAR} OFF PARENT_SCOPE)
        return()
    endif()
    include(CheckCSourceCompiles)
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=address,undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address,undefined")
    check_c_source_compiles(
        "int main(void) { volatile int *p = 0; (void)p; return 0; }"
        MICRLINK_HAVE_SANITIZERS
    )
    if(MICRLINK_HAVE_SANITIZERS)
        set(${OUT_VAR} ON PARENT_SCOPE)
    else()
        set(${OUT_VAR} OFF PARENT_SCOPE)
    endif()
endfunction()
