#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyUtility/String/String.h"
#include "ToyUtility/Memory/SmartPtr.h"
#include "ToyXWindow/CommonType.h"


namespace ToyXWindow
{


class IWindow;
using HWindow = IWindow*;


#define ShouldCloseFuncArguList 
#define ShouldCloseFuncParaList 
#define WindowSizeFuncArguList uint32 width, uint32 height
#define WindowSizeFuncParaList width, height
#define FrameBufferSizeFuncArguList uint32 width, uint32 height
#define FrameBufferSizeFuncParaList width, height
#define PosFuncArguList int32 xpos, int32 ypos
#define PosFuncParaList xpos, ypos
#define IconifyFuncArguList bool isIconified
#define IconifyFuncParaList isIconified
#define FocusFuncArguList bool focused
#define FocusFuncParaList focused
#define WindowNeedRefreshFuncArguList 
#define WindowNeedRefreshFuncParaList 
#define KeyFuncArguList KeyType key, int scancode, KeyAction action, int modifiers
#define KeyFuncParaList key, scancode, action, modifiers
#define CursorPosFuncArguList float xpos, float ypos
#define CursorPosFuncParaList xpos, ypos
#define CursorPosDeltaFuncArguList float xdelta, float ydelta
#define CursorPosDeltaFuncParaList xdelta, ydelta
#define CursorEnterLeaveFuncArguList bool isEnter
#define CursorEnterLeaveFuncParaList isEnter
#define MouseButtonFuncArguList MouseButtonType button, KeyAction action, int modifiers
#define MouseButtonFuncParaList button, action, modifiers
#define ScrollFuncArguList const ScrollDesc& scroll
#define ScrollFuncParaList scroll
#define DropFuncArguList int count, const char** texts
#define DropFuncParaList count, texts

using ShouldCloseFunc = std::function<void(HWindow)>;
using WindowSizeFunc = std::function<void(HWindow, WindowSizeFuncArguList)>;
using FrameBufferSizeFunc = std::function<void(HWindow, FrameBufferSizeFuncArguList)>;
using PosFunc = std::function<void(HWindow, PosFuncArguList)>;
using IconifyFunc = std::function<void(HWindow, IconifyFuncArguList)>;
using FocusFunc = std::function<void(HWindow, FocusFuncArguList)>;
using WindowNeedRefreshFunc = std::function<void(HWindow)>;
using KeyFunc = std::function<void(HWindow, KeyFuncArguList)>;
using CursorPosFunc = std::function<void(HWindow, CursorPosFuncArguList)>;
using CursorPosDeltaFunc = std::function<void(HWindow, CursorPosDeltaFuncArguList)>;
using CursorEnterLeaveFunc = std::function<void(HWindow, CursorEnterLeaveFuncArguList)>;
using MouseButtonFunc = std::function<void(HWindow, MouseButtonFuncArguList)>;
using ScrollFunc = std::function<void(HWindow, ScrollFuncArguList)>;
using DropFunc = std::function<void(HWindow, DropFuncArguList)>;


class IWindow
{
public:
    virtual ~IWindow() = default;


public:
    //////////////////////////////////////////////////////////////////////////////////
    // Init

    virtual ToyXResult Create(const WINDOW_DESC& desc) = 0;

    virtual void Destory() = 0;

    //////////////////////////////////////////////////////////////////////////////////
    // Window

    // Is windowed-mode window
    virtual bool IsWindowed() const = 0;

    // When the user attempts to close the window, for example by clicking the close widget
    // or using a key chord like Alt + F4, the close flag of the window is set.
    virtual bool ShouldClose() = 0;
    
    // Set should-close flag
    virtual void SetShouldClose(bool b) = 0;

    // Be notified when the user attempts to close the window
    virtual void SetShouldCloseCallback(ShouldCloseFunc callback) = 0;

    // Get window's size in screen coordinates
    virtual void GetWindowSize(uint32* width, uint32* height) = 0;

    // For windowed mode windows, this sets the size, in screen coordinates of the client area
    // or content area of the window. For full screen windows, nothing happenes.
    virtual void SetWindowSize(uint32 width, uint32 height) = 0;

    // Be notified when a window is resized, whether by the user or the system 
    virtual void SetWindowSizeCallback(WindowSizeFunc callback) = 0;

    // The above functions work with the size of the client area, but decorated windows typically
    // have title bars and window frames around this rectangle. You can retrieve the extents of
    // these with GetWindowFrameSize.
    // The returned values are the distances, in screen coordinates, from the edges of the client
    // area to the corresponding edges of the full window. As they are distances and not coordinates,
    // they are always zero or positive.
    virtual void GetWindowFrameSize(uint32* left, uint32* top, uint32* right, uint32* bottom) = 0;

    // The size of a framebuffer may change independently of the size of a window, for example if the
    // window is dragged between a regular monitor and a high-DPI one.

    // The size of a window is measured in screen coordinates, this function return the framebuffer size
    // of a window. (in pixels)
    virtual void GetFrameBufferSize(uint32* width, uint32* height) = 0;

    virtual void SetFrameBufferSizeCallback(FrameBufferSizeFunc callback) = 0;

    // Sets the size limits of the client area of the specified window. If this window is full screen
    // or not resizeable, this function does nothing.
    // Arguments can be XWINDOW_DONT_CARE
    virtual void SetWindowSizeLimits(uint32 minWidth, uint32 minHeight, uint32 maxWidth, uint32 maxHeight) = 0;

    // The aspect ratio of the client area of a windowed mode window can be enforced with this function.
    // To disable this function, set both terms to XWINDOW_DONT_CARE
    virtual void SetAspectRatio(uint32 width, uint32 height) = 0;

    // The position of a windowed-mode window can be changed with this function. This moves the window
    // so that the upper-left corner of its client area has the specified screen coordinates. 
    virtual void SetPos(int32 xpos, int32 ypos) = 0;

    virtual void SetPosCallback(PosFunc callback) = 0;

    virtual void GetPos(int32* xpos, int32* ypos) = 0;

    // Get window title
    virtual const ToyUtility::String& GetTitle() const = 0;

    virtual void SetTitle(const ToyUtility::String& title) = 0;

    // TODOL: Window icon

    // TODOM: MaximizeWindow see glfw:win32_window.c:L1224, L1347

    // TODOM: Window monitor

    // Minimized
    virtual void Iconify() = 0;

    virtual void Restore() = 0;

    virtual void SetIconifyCallback(IconifyFunc callback) = 0;

    // Get the current iconification(i.e. is minimized)
    virtual bool IsIconified() = 0;

    // Windowed-mode windows can be hidden. This makes the window completely invisible to the user,
    // including removing it from the task bar, dock or window list. Full screen windows cannot be
    // hidden and calling Hide on a full screen window does nothing.
    virtual void Hide() = 0;

    virtual void Show() = 0;

    // Get the current visibility state of the window
    virtual bool IsVisible() = 0;

    // Window can be given input focus and brought to the front
    virtual void Focus() = 0;

    // Be notified when a window gains or loses input focus, whether by the user, system or your own code
    virtual void SetFocusCallback(FocusFunc callback) = 0;

    // Get current focus state
    virtual bool IsFocused() = 0;

    // Window demage and refresh
    // Be notified when the contents of a window is damaged and needs to be refreshed
    virtual void SetWindowNeedRefreshCalback(WindowNeedRefreshFunc callback) = 0;

    // TODOH: more http://www.glfw.org/docs/latest/window_guide.html#window_attribs

    //////////////////////////////////////////////////////////////////////////////////
    // Buffer Swapping

    // @syncInterval: An integer that specifies the how to synchronize presentation of a frame with the
    //      vertical blank.Values are :
    //      0 - The presentation occurs immediately, there is no synchronization.
    //      1, 2, 3, 4 - Synchronize presentation after the n'th vertical blank. 
    virtual void PresentBackBuffer(uint32 syncInterval) = 0;


    //////////////////////////////////////////////////////////////////////////////////
    // Keyboard
    
    // The scancode is unique for every key, regardless of whether it has a key token. Scancodes are
    // platform-specific but consistent over time.
    virtual void SetKeyCallback(KeyFunc callback) = 0;

    virtual ButtonState GetKey(KeyType key) = 0;

    virtual const ToyUtility::String& GetKeyName(KeyType key) const = 0;

    //////////////////////////////////////////////////////////////////////////////////
    // Mouse
    
    // This function returns the position of the cursor, in screen coordinates, relative
    // to the upper-left corner of the client area of the specified window.
    virtual void GetCursorPos(float* xpos, float* ypos) = 0;

    virtual void SetCursorPosCallback(CursorPosFunc callback) = 0;

    virtual void SetCursorPos(float xpos, float ypos) = 0;

    virtual void GetCursorPosDelta(float* xdelta, float* ydelta) = 0;

    virtual void SetCursorPosDeltaCallback(CursorPosDeltaFunc callback) = 0;

    virtual void SetCursorMode(CursorMode mode) = 0;

    virtual CursorMode GetCursorMode() = 0;

    // TODOM: cursor objects

    virtual void SetCursorEnterLeaveCallback(CursorEnterLeaveFunc callback) = 0;

    // Mouse button input

    virtual void SetMouseButtonCallback(MouseButtonFunc callback) = 0;

    virtual ButtonState GetMouseButton(MouseButtonType type) = 0;

    // If you wish to be notified when the user scrolls, whether with a mouse wheel or touchpad gesture,
    // set a scroll callback.
    virtual void SetScrollCallback(ScrollFunc callback) = 0;

    //////////////////////////////////////////////////////////////////////////////////
    // Others

    // Clipboard input and output
    virtual const ToyUtility::String& GetClipboardString() const = 0;
    virtual void SetClipboardString(const ToyUtility::String& text) = 0;

    // Drop input
    virtual void SetDropCallback(DropFunc callback) = 0;
};


} // end of namespace ToyXWindow