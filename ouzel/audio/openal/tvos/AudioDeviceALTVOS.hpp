// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"

#if OUZEL_PLATFORM_TVOS && OUZEL_SUPPORTS_OPENAL

#include "audio/openal/AudioDeviceAL.hpp"

namespace ouzel
{
    namespace audio
    {
        class AudioDeviceALTVOS: public AudioDeviceAL
        {
        public:
            virtual bool init(bool debugAudio) override;
        };
    }
}

#endif
