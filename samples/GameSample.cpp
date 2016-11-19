// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "GameSample.h"

using namespace std;
using namespace ouzel;

GameSample::GameSample(Samples& aSamples):
    samples(aSamples)
{
    eventHandler.keyboardHandler = bind(&GameSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    eventHandler.mouseHandler = bind(&GameSample::handleMouse, this, placeholders::_1, placeholders::_2);
    eventHandler.touchHandler = bind(&GameSample::handleTouch, this, placeholders::_1, placeholders::_2);
    eventHandler.gamepadHandler = bind(&GameSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&GameSample::handleUI, this, placeholders::_1, placeholders::_2);

    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    addLayer(&layer);
    layer.addCamera(&camera);
}

bool GameSample::handleKeyboard(Event::Type type, const KeyboardEvent& event)
{
    return true;
}

bool GameSample::handleMouse(Event::Type type, const MouseEvent& event)
{
    return true;
}

bool GameSample::handleTouch(Event::Type, const TouchEvent& event)
{
    return true;
}

bool GameSample::handleGamepad(Event::Type type, const GamepadEvent& event)
{
    return true;
}

bool GameSample::handleUI(Event::Type type, const UIEvent& event) const
{
    return true;
}
