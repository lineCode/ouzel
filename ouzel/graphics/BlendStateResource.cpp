// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "BlendStateResource.hpp"

namespace ouzel
{
    namespace graphics
    {
        BlendStateResource::BlendStateResource()
        {
        }

        BlendStateResource::~BlendStateResource()
        {
        }

        bool BlendStateResource::init(bool newEnableBlending,
                                      BlendState::BlendFactor newColorBlendSource, BlendState::BlendFactor newColorBlendDest,
                                      BlendState::BlendOperation newColorOperation,
                                      BlendState::BlendFactor newAlphaBlendSource, BlendState::BlendFactor newAlphaBlendDest,
                                      BlendState::BlendOperation newAlphaOperation,
                                      uint8_t newColorMask)
        {
            enableBlending = newEnableBlending;
            colorBlendSource = newColorBlendSource;
            colorBlendDest = newColorBlendDest;
            colorOperation = newColorOperation;
            alphaBlendSource = newAlphaBlendSource;
            alphaBlendDest = newAlphaBlendDest;
            alphaOperation = newAlphaOperation;
            colorMask = newColorMask;

            return true;
        }
    } // namespace graphics
} // namespace ouzel
