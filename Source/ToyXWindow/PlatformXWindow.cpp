#include "ToyXWindow/PlatformXWindow.h"


namespace ToyXWindow
{


bool PlatformXWindow::StartUp(const XWINDOW_API_STARTUP_DESC & desc)
{
    m_Desc = desc;
    _PlatformStartUp();
}

void PlatformXWindow::SetMainWindow(ToyUtility::SPtr<IWindow> window)
{
    m_MainWindow = window;
}


} // end of namespace ToyXWindow