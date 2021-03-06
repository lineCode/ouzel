// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "WindowAndroid.hpp"
#include "EngineAndroid.hpp"
#include "utils/Log.hpp"

namespace ouzel
{
    WindowAndroid::WindowAndroid()
    {
        EngineAndroid* engineAndroid = static_cast<EngineAndroid*>(sharedEngine);
        JavaVM* javaVM = engineAndroid->getJavaVM();
        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return;
        }

        window = ANativeWindow_fromSurface(jniEnv, engineAndroid->getSurface());
    }

    WindowAndroid::~WindowAndroid()
    {
        if (window)
        {
            ANativeWindow_release(window);
        }
    }

    void WindowAndroid::handleResize(const Size2& newSize)
    {
        Event event;
        event.type = Event::Type::WINDOW_SIZE_CHANGE;

        event.windowEvent.window = this;
        event.windowEvent.size = newSize;

        sharedEngine->getEventDispatcher()->postEvent(event);
    }
}
