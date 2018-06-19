# Example/SimpleWindow


INCLUDE_DIRECTORIES(${TOY_XWINDOW_INCLUDE_PATHS})
add_executable(ToyXWindow_Example_SimpleWindow
	"Main.cpp"
)


target_link_libraries(ToyXWindow_Example_SimpleWindow
    ${TOY_XWINDOW_LIBRARIES}
)


# IDE specific
set_property(TARGET ToyXWindow_Example_SimpleWindow PROPERTY FOLDER Example)