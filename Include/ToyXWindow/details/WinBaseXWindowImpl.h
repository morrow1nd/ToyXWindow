#pragma once

#include <Windows.h>

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/details/XWindowImpl.h"
#include "ToyXWindow/details/WinWindow.h"


namespace ToyXWindow
{


class WinBaseXWindowImpl : public XWindowImpl
{
protected:
    const ToyUtility::String WindowsWindowClassName = "ToyXWindowClass";


public:
    virtual ToyXResult StartUp(const XWINDOW_API_STARTUP_DESC& desc) override;
    virtual void ShutDown() override;
    virtual void PollEvents() override;
    virtual void WaitEvents() override;
    virtual void WaitEventsTimeout(float time) override;
    virtual void PostEmptyEvent() override;
    virtual void SetMainWindow(ToyUtility::SPtr<IWindow> window) override;


public:
    HINSTANCE GetProgramInstance() { return m_ProgramInstance; }


private:
    bool _RegisterWindowsWindowClass();

    // Create key code translation tables
    static void _CreateKeycodeTables();
    
public:
    // Translates a Windows key to the corresponding ToyXWindow key
    static KeyType _TranslateKey(WPARAM wParam, LPARAM lParam);

    static KeyType NativeKeyToPublicKey(int32 scancode);
    static int32 PublicKeyToNativeKey(KeyType key);

    static int GetKeyModifiers();


private:
    ToyUtility::SPtr<WinWindow> m_MainWindow;
    HINSTANCE m_ProgramInstance;

    static ToyUtility::int16 m_PublicKeys[512];
    static ToyUtility::int16 m_NativeKeys[(int)KeyType::__End];
};


} // end of namespace ToyXWindow