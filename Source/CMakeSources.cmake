SET(TOY_XWINDOW_INC_FILES
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/BaseWindow.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/CommonType.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/IAdapter.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/IWindow.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/XWindowAPI.h"
)

SET(TOY_XWINDOW_SRC_FILES
    "${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/BaseWindow.cpp"
    "${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/CommonType.cpp"
    "${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/XWindowAPI.cpp"
    "${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/XWindowImpl.cpp"
)

SET(TOY_XWINDOW_INC_DETAILS
	"${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/XWindowImpl.h"
)


## Windows Platform
SET(TOY_XWINDOW_INC_DETAILS_WINDOWS
	"${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinBaseXWindowImpl.h"
	"${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinDX11CommonType.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinDX11Window.h"
	"${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinDX11XWindowImpl.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinWGLWindow.h"
	"${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinWGLXWindowImpl.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/details/WinWindow.h"
)

SET(TOY_XWINDOW_SRC_WINDOWS
	"${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/WinBaseXWindowImpl.cpp"
	"${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/WinDX11Window.cpp"
	"${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/WinDX11XWindowImpl.cpp"
	"${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/WinWGLWindow.cpp"
	"${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/WinWGLXWindowImpl.cpp"
	"${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/WinWindow.cpp"
)


source_group("CMake"									FILES "${TOY_XWINDOW_SOURCE_DIR}/../CMakeLists.txt")
source_group("CMake\\Source"                            FILES "CMakeSources.cmake" "CMakeLists.txt")
#source_group("CMake\\Source\\ThirdParty"				FILES "${TOY_XWINDOW_SOURCE_DIR}/ThirdParty/CMakeLists.txt")
source_group("Header Files"								FILES ${TOY_XWINDOW_INC_FILES})
source_group("Source Files"								FILES ${TOY_XWINDOW_SRC_FILES})
if(TOY_WINDOWS)
source_group("Header Files\\details"					FILES ${TOY_XWINDOW_INC_DETAILS_WINDOWS})
source_group("Header Files\\details"					FILES ${TOY_XWINDOW_INC_DETAILS})
source_group("Source Files\\Windows"					FILES ${TOY_XWINDOW_SRC_WINDOWS})
endif()

set(TOY_XWINDOW_SRC
	"${TOY_XWINDOW_SOURCE_DIR}/../CMakeLists.txt"
    "CMakeSources.cmake" "CMakeLists.txt"
	#"${TOY_XWINDOW_SOURCE_DIR}/ThirdParty/CMakeLists.txt"

	${TOY_XWINDOW_INC_FILES}
    ${TOY_XWINDOW_SRC_FILES}
	${TOY_XWINDOW_INC_DETAILS}
)

if(TOY_WINDOWS)
set(TOY_XWINDOW_SRC
	${TOY_XWINDOW_SRC}

	${TOY_XWINDOW_INC_DETAILS_WINDOWS}
    ${TOY_XWINDOW_SRC_WINDOWS}
)
endif()