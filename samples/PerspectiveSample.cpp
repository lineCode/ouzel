// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "PerspectiveSample.hpp"
#include "MainMenu.hpp"

using namespace std;
using namespace ouzel;
using namespace graphics;

PerspectiveSample::PerspectiveSample():
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", 0, Color::BLACK, Color::BLACK, Color::BLACK)
{
    cursor.init(input::SystemCursor::CROSS);
    sharedEngine->getInput()->setCursor(&cursor);

    eventHandler.keyboardHandler = bind(&PerspectiveSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    eventHandler.mouseHandler = bind(&PerspectiveSample::handleMouse, this, placeholders::_1, placeholders::_2);
    eventHandler.touchHandler = bind(&PerspectiveSample::handleTouch, this, placeholders::_1, placeholders::_2);
    eventHandler.gamepadHandler = bind(&PerspectiveSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&PerspectiveSample::handleUI, this, placeholders::_1, placeholders::_2);

    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    sharedEngine->getRenderer()->setClearDepthBuffer(true);

    camera.setDepthTest(true);
    camera.setDepthWrite(true);

    camera.setType(scene::Camera::Type::PERSPECTIVE);
    camera.setFarPlane(1000.0f);
    camera.setPosition(Vector3(0.0f, 0.0f, -400.0f));

    layer.addChild(&camera);
    addLayer(&layer);

    // floor
    floorSprite.reset(new scene::Sprite());
    floorSprite->init("floor.jpg");
    floorSprite->getMaterial()->textures[0]->setMaxAnisotropy(4);

    floor.reset(new scene::Node());
    floor->addComponent(floorSprite.get());
    layer.addChild(floor.get());
    floor->setPosition(Vector2(0.0f, -50.0f));
    floor->setRotation(Vector3(TAU_4, TAU / 8.0f, 0.0f));
    
    // character
    characterSprite.reset(new scene::Sprite());
    characterSprite->init("run.json");
    characterSprite->play(true);
    characterSprite->getMaterial()->textures[0]->setMaxAnisotropy(4);
    characterSprite->getMaterial()->cullMode = graphics::Renderer::CullMode::NONE;

    character.reset(new scene::Node());
    character->addComponent(characterSprite.get());
    layer.addChild(character.get());
    character->setPosition(Vector2(10.0f, 0.0f));

    jumpSound.reset(new audio::Sound());
    std::shared_ptr<ouzel::audio::SoundDataWave> soundData = std::make_shared<ouzel::audio::SoundDataWave>();
    soundData->init("jump.wav");
    jumpSound->init(soundData);
    jumpSound->setPosition(character->getPosition());

    rotate.reset(new scene::Rotate(10.0f, Vector3(0.0f, TAU, 0.0f)));
    character->addComponent(rotate.get());
    rotate->start();

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));

    guiLayer.addChild(&guiCamera);
    addLayer(&guiLayer);

    guiLayer.addChild(&menu);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(&backButton);
}

bool PerspectiveSample::handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event)
{
    if (type == Event::Type::CLICK_NODE)
    {
        if (event.node == &backButton)
        {
            sharedEngine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
        }
    }

    return true;
}

bool PerspectiveSample::handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event)
{
    if (type == Event::Type::KEY_PRESS ||
        type == Event::Type::KEY_REPEAT)
    {
        switch (event.key)
        {
            case input::KeyboardKey::UP:
                cameraRotation.x -= TAU / 100.0f;
                break;
            case input::KeyboardKey::DOWN:
                cameraRotation.x += TAU / 100.0f;
                break;
            case input::KeyboardKey::LEFT:
                cameraRotation.y -= TAU / 100.0f;
                break;
            case input::KeyboardKey::RIGHT:
                cameraRotation.y += TAU / 100.0f;
                break;
            case input::KeyboardKey::ESCAPE:
            case input::KeyboardKey::MENU:
                sharedEngine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
                return true;
            case input::KeyboardKey::TAB:
                jumpSound->play();
                break;
            case input::KeyboardKey::S:
                sharedEngine->getRenderer()->saveScreenshot("test.png");
                break;
            default:
                break;
        }

        if (cameraRotation.x < -TAU / 6.0f) cameraRotation.x = -TAU / 6.0f;
        if (cameraRotation.x > TAU / 6.0f) cameraRotation.x = TAU / 6.0f;

        camera.setRotation(cameraRotation);

        sharedEngine->getAudio()->setListenerRotation(camera.getRotation());
    }

    return true;
}

bool PerspectiveSample::handleMouse(ouzel::Event::Type type, const ouzel::MouseEvent& event)
{    
    if (event.modifiers & LEFT_MOUSE_DOWN)
    {
        if (type == Event::Type::MOUSE_MOVE)
        {
            cameraRotation.x += event.difference.y;
            cameraRotation.y -= event.difference.x;

            if (cameraRotation.x < -TAU / 6.0f) cameraRotation.x = -TAU / 6.0f;
            if (cameraRotation.x > TAU / 6.0f) cameraRotation.x = TAU / 6.0f;

            camera.setRotation(cameraRotation);
        }
    }

    return true;
}

bool PerspectiveSample::handleTouch(ouzel::Event::Type type, const ouzel::TouchEvent& event)
{
    if (type == Event::Type::TOUCH_MOVE)
    {
        cameraRotation.x += event.difference.y;
        cameraRotation.y -= event.difference.x;

        if (cameraRotation.x < -TAU / 6.0f) cameraRotation.x = -TAU / 6.0f;
        if (cameraRotation.x > TAU / 6.0f) cameraRotation.x = TAU / 6.0f;

        camera.setRotation(cameraRotation);
    }

    return true;
}

bool PerspectiveSample::handleGamepad(Event::Type type, const GamepadEvent& event)
{
    if (type == Event::Type::GAMEPAD_BUTTON_CHANGE)
    {
        if (event.pressed &&
            event.button == input::GamepadButton::FACE_RIGHT)
        {
            sharedEngine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
        }
    }

    return true;
}
