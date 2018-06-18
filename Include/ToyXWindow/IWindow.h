#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyUtility/String/String.h"
#include "ToyXWindow/CommonType.h"


namespace ToyXWindow
{


class IWindow
{
public:
    virtual ~IWindow() = default;


public:
    //////////////////////////////////////////////////////////////////////////////////
    // Keyboard
    
    virtual void SetKeyCallback() = 0;

    virtual ButtonState GetKey(KeyType key) = 0;


    //////////////////////////////////////////////////////////////////////////////////
    // Mouse
    
    // This function returns the position of the cursor, in screen coordinates, relative
    // to the upper-left corner of the client area of the specified window.
    virtual void GetCursorPos(float* xpos, float* ypos) = 0;

    virtual void SetCursorPosCallback() = 0;

    virtual void GetCursorPosDelta(float* xdelta, float* ydelta) = 0;

    virtual void SetCursorPosDeltaCallback() = 0;

    virtual void SetCursorMode(CursorMode mode) = 0;

    virtual CursorMode GetCursorMode() = 0;

    // TODOM: cursor objects

    virtual void SetCursorEnterLeaveCallback() = 0;

    // Mouse button input

    virtual void SetMouseButtonCallback() = 0;

    virtual ButtonState GetMouseButton(MouseButtonType type) = 0;

    // If you wish to be notified when the user scrolls, whether with a mouse wheel or touchpad gesture,
    // set a scroll callback.
    virtual void SetScrollCallback() = 0;

    //////////////////////////////////////////////////////////////////////////////////
    // Others

    // Clipboard input and output
    virtual const ToyUtility::String& GetClipboardString() const = 0;
    virtual void SetClipboardString(const ToyUtility::String& text) = 0;

    // Drop input
    virtual void SetDropCallback() = 0;
};


} // end of namespace ToyXWindow