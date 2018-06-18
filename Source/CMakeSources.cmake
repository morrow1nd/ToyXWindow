SET(TOY_XWINDOW_INC_FILES
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/CommonType.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/XWindowAPI.h"
    "${TOY_XWINDOW_INCLUDE_DIR}/ToyXWindow/IWindow.h"
)

SET(TOY_XWINDOW_SRC_FILES
    "${TOY_XWINDOW_SOURCE_DIR}/ToyXWindow/XWindowAPI.cpp"
)


source_group("CMake"									FILES "${TOY_XWINDOW_SOURCE_DIR}/../CMakeLists.txt")
source_group("CMake\\Source"                            FILES "CMakeSources.cmake" "CMakeLists.txt")
#source_group("CMake\\Source\\ThirdParty"				FILES "${TOY_XWINDOW_SOURCE_DIR}/ThirdParty/CMakeLists.txt")


set(TOY_XWINDOW_SRC
	"${TOY_XWINDOW_SOURCE_DIR}/../CMakeLists.txt"
    "CMakeSources.cmake" "CMakeLists.txt"
	#"${TOY_XWINDOW_SOURCE_DIR}/ThirdParty/CMakeLists.txt"

	${TOY_XWINDOW_INC_FILES}
    ${TOY_XWINDOW_SRC_FILES}
)