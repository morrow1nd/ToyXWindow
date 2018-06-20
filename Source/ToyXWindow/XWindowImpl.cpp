#include "ToyXWindow/details/XWindowImpl.h"

#include "ToyUtilityConfig.h"

#if TOY_WINDOWS
#include "ToyXWindow/details/WinDX11XWindowImpl.h"
#include "ToyXWindow/details/WinWGLXWindowImpl.h"
#endif


namespace ToyXWindow
{


ToyUtility::SPtr<XWindowImpl> XWindowImpl::FactoryCreate(WindowContextNativeApiType type)
{
    switch (type)
    {
#if TOY_WINDOWS
    case WindowContextNativeApiType::Win_DX11:
        return ToyUtility::SPtr<XWindowImpl>(new WinDX11XWindowImpl());
        break;
    case WindowContextNativeApiType::Win_WGL:
        return ToyUtility::SPtr<XWindowImpl>(new WinWGLXWindowImpl());
        break;
#endif
#if TOY_LINUX
    case WindowContextNativeApiType::Linux_EGL:
#error No Linux XWindomImpl.
        break;
#endif
#if TOY_MACOS
    case WindowContextNativeApiType::MacOS_CGL:
#error No MacOS XWindowImpl.
        break;
#endif
    default:
        assert(false && "XWindowImpl::FactoryCreate: can't create this type in current platform");
        break;
    }

    return ToyUtility::SPtr<XWindowImpl>();
}


} // end of namespace ToyXWindow