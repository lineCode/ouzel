// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_OPENAL

#include <thread>

#if OUZEL_PLATFORM_MACOS || OUZEL_PLATFORM_IOS || OUZEL_PLATFORM_TVOS
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#undef OPENAL

#include "audio/AudioDevice.hpp"

namespace ouzel
{
    namespace audio
    {
        class Audio;

        class AudioDeviceAL: public AudioDevice
        {
            friend Audio;
        public:
            bool checkALCError(bool logError = true);
            static bool checkOpenALError(bool logError = true);

            virtual ~AudioDeviceAL();

            virtual bool update() override;

            ALCdevice* getDevice() const { return device; }
            ALCcontext* getContext() const { return context; }

        protected:
            AudioDeviceAL();
            virtual bool init(bool debugAudio) override;

            void run();

            ALCdevice* device = nullptr;
            ALCcontext* context = nullptr;

            ALenum format40 = 0;
            ALenum format51 = 0;
            ALenum format61 = 0;
            ALenum format71 = 0;

            ALuint sourceId = 0;
            ALenum format = 0;
            uint32_t nextBuffer = 0;
            ALuint buffers[2];

            bool running = true;

            std::vector<uint8_t> data;

#if OUZEL_MULTITHREADED
            std::thread audioThread;
#endif
        };
    } // namespace audio
} // namespace ouzel

#endif
