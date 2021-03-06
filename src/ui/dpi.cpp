#include <cassert>

#ifdef WIN32
#include <ShellScalingApi.h>
#include "winuser.h"
#pragma comment (lib, "shcore.lib")
#endif

#include "mutils/logger/logger.h"
#include "mutils/ui/dpi.h"

namespace MUtils
{
Dpi dpi;

void check_dpi()
{
#ifdef WIN32
    PROCESS_DPI_AWARENESS awareness;
    HRESULT hr = GetProcessDpiAwareness(nullptr, &awareness);
    if (SUCCEEDED(hr))
    {
        switch (awareness)
        {
        default:
        case PROCESS_DPI_UNAWARE:
            LOG(DEBUG) << "Not DPI Aware";
            assert(!"Unexpected!");
            break;
        case PROCESS_SYSTEM_DPI_AWARE:
            LOG(DEBUG) << "DPI Aware";
            break;
        case PROCESS_PER_MONITOR_DPI_AWARE:
            LOG(DEBUG) << "Per Monitor Aware";
            break;
        }
    }
    auto d = GetSystemDpiForProcess(nullptr);
    dpi.scaleFactor = d / 96.0f;
#endif
}

} // MUtils
