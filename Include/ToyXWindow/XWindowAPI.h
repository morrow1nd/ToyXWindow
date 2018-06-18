#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/IWindow.h"


namespace ToyXWindow
{

// It provides a platform independent API for creating windows, contexts,
// getting keyboard, mouse, touch event, handling clipboard, path drop input and so on.
class XWindowAPI
{
public:
    void StartUp();
    void ShutDown();

    ////////////////////////////////////////////////////////////////////////////////////
    // Event process

    // Event processing must be done regularly while you have any windows and is normally
    // done each frame after buffer swapping.

    // PollEvent processes only those events that have already been received and then returns immediately.
    // This is the best choice when rendering continually, like most games do.
    void PollEvents();
    
    // If you only need to update the contents of the window when you receive new input, WaitEvents is a
    // better choice. It puts the thread to sleep until at least one event has been received and then
    // processes all received events. This saves a great deal of CPU cycles and is useful for, for example,
    // editing tools. There must be at least one GLFW window for this function to sleep.
    void WaitEvents();

    void WaitEventsTimeout(float time);

    // If the main thread is sleeping in glfwWaitEvents, you can wake it from another thread by posting
    // an empty event to the event queue with glfwPostEmptyEvent.
    void PostEmptyEvent();

    ////////////////////////////////////////////////////////////////////////////////////
    // Adapter and window

    void GetDefaultAdapter();
    void GetAdapters();

    void CreateWindow();
    void GetAllWindows();
};


} // end of namespace ToyXWindow