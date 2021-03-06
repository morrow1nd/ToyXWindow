# 
# ToyXWindow/Source
# 

## CMake Options

### TOY_XWINDOW Debug Mode
option(OPTION_TOY_XWINDOW_MORE_RUNTIME_CHECK
	"Open more ToyXWindow runtime checking" ON)
if(OPTION_TOY_XWINDOW_MORE_RUNTIME_CHECK)
	set(TOY_XWINDOW_MORE_RUNTIME_CHECK "1")
else()
	set(TOY_XWINDOW_MORE_RUNTIME_CHECK "0")
endif()

### ToyUtility Path
set(OPTION_TOY_UTILITY_PATH "${PROJECT_SOURCE_DIR}/../ToyUtility" CACHE PATH "path/to/ToyUtility" FORCE)

## end of CMake Options


## Third Party Libs
#ADD_SUBDIRECTORY(ThirdParty)

## Configure file
CONFIGURE_FILE(
    "${TOY_XWINDOW_SOURCE_DIR}/ToyXWindowConfig.h.in"
    "${PROJECT_BINARY_DIR}/ToyXWindowConfig.h"
)


## load ToyUtility project
ADD_SUBDIRECTORY(${OPTION_TOY_UTILITY_PATH} ToyUtility.out)


# Source files and their filters
INCLUDE(CMakeSources.cmake)


INCLUDE_DIRECTORIES(
    ${TOY_UTILITY_INCLUDE_PATHS}

    ${TOY_XWINDOW_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
)

if(TOY_WINDOWS)
set(TOY_WINDOWS_DX11_EXTRN_LIB
	d3d11
	d3dcompiler
	dxguid
	dxgi
)
endif()


ADD_LIBRARY(ToyXWindow
    ${TOY_XWINDOW_SRC}
    ToyXWindowConfig.h.in
)
target_link_libraries(ToyXWindow
    ${TOY_UTILITY_LIBRARIES}
    ${TOY_WINDOWS_DX11_EXTRN_LIB}
)


# IDE specific
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
SET_PROPERTY(TARGET ToyXWindow PROPERTY FOLDER Libs)

## For lib user
SET(TOY_XWINDOW_INCLUDE_PATHS
    ${TOY_UTILITY_INCLUDE_PATHS}
    ${TOY_XWINDOW_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    CACHE INTERNAL "TOY_XWINDOW INCLUDE PATHS"
)

SET(TOY_XWINDOW_LIBRARIES
    ToyXWindow
    ${TOY_UTILITY_LIBRARIES}
    ${TOY_WINDOWS_DX11_EXTRN_LIB}
    CACHE INTERNAL "TOY_XWINDOW Librarys"
)