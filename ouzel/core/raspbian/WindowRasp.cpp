// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <bcm_host.h>
#include "WindowRasp.h"

namespace ouzel
{
    WindowRasp::WindowRasp(const Size2& pSize, bool pResizable, bool pFullscreen, const std::string& pTitle):
        Window(pSize, pResizable, pFullscreen, pTitle)
    {
        bcm_host_init();
    }

    WindowRasp::~WindowRasp()
    {
        bcm_host_deinit();
    }

    bool WindowRasp::init()
    {
        uint32_t screenWidth;
        uint32_t screenHeight;
        int32_t success = graphics_get_display_size(0, &screenWidth, &screenHeight);

        if (success == -1)
        {
            log(LOG_LEVEL_ERROR, "Failed to get display size");
            return false;
        }

        size.width = static_cast<float>(screenWidth);
        size.height = static_cast<float>(screenHeight);

        return Window::init();
    }
}
