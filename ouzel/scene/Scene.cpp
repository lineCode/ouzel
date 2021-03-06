// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "Scene.hpp"
#include "Layer.hpp"
#include "Camera.hpp"
#include "SceneManager.hpp"
#include "core/Engine.hpp"
#include "events/EventDispatcher.hpp"

namespace ouzel
{
    namespace scene
    {
        Scene::Scene():
            eventHandler(EventHandler::PRIORITY_MAX + 1)
        {
            eventHandler.windowHandler = std::bind(&Scene::handleWindow, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.mouseHandler = std::bind(&Scene::handleMouse, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.touchHandler = std::bind(&Scene::handleTouch, this, std::placeholders::_1, std::placeholders::_2);
        }

        Scene::~Scene()
        {
            if (sceneManger) sceneManger->removeScene(this);

            for (Layer* layer : layers)
            {
                if (entered) layer->leave();
                layer->scene = nullptr;
            }
        }

        void Scene::draw()
        {
            std::stable_sort(layers.begin(), layers.end(), [](Layer* a, Layer* b) {
                return a->getOrder() > b->getOrder();
            });

            for (Layer* layer : layers)
            {
                layer->draw();
            }
        }

        void Scene::addChildLayer(Layer* layer)
        {
            if (layer && !hasLayer(layer))
            {
                layers.push_back(layer);

                if (entered) layer->enter();

                layer->scene = this;
            }
        }

        bool Scene::removeChildLayer(Layer* layer)
        {
            bool result = false;

            std::vector<Layer*>::iterator layerIterator = std::find(layers.begin(), layers.end(), layer);

            if (layerIterator != layers.end())
            {
                if (entered)
                {
                    layer->leave();
                }

                layers.erase(layerIterator);

                layer->scene = nullptr;

                result = true;
            }

            std::vector<std::unique_ptr<Layer>>::iterator ownedIterator = std::find_if(ownedLayers.begin(), ownedLayers.end(), [layer](const std::unique_ptr<Layer>& other) {
                return other.get() == layer;
            });

            if (ownedIterator != ownedLayers.end())
            {
                ownedLayers.erase(ownedIterator);
            }

            return result;
        }

        void Scene::removeAllLayers()
        {
            if (entered)
            {
                for (Layer* layer : layers)
                {
                    layer->leave();
                }
            }

            layers.clear();
            ownedLayers.clear();
        }

        bool Scene::hasLayer(Layer* layer) const
        {
            std::vector<Layer*>::const_iterator i = std::find(layers.begin(), layers.end(), layer);

            return i != layers.end();
        }

        void Scene::recalculateProjection()
        {
            for (Layer* layer : layers)
            {
                layer->recalculateProjection();
            }
        }

        std::pair<Node*, ouzel::Vector3> Scene::pickNode(const Vector2& position, bool renderTargets) const
        {
            for (std::vector<Layer*>::const_reverse_iterator i = layers.rbegin(); i != layers.rend(); ++i)
            {
                Layer* layer = *i;
                std::pair<Node*, ouzel::Vector3> result = layer->pickNode(position, renderTargets);

                if (result.first)
                {
                    return result;
                }
            }

            return std::make_pair(nullptr, Vector3());
        }

        std::vector<std::pair<Node*, ouzel::Vector3>> Scene::pickNodes(const Vector2& position, bool renderTargets) const
        {
            std::vector<std::pair<Node*, ouzel::Vector3>> result;

            for (auto i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::vector<std::pair<Node*, ouzel::Vector3>> nodes = (*i)->pickNodes(position, renderTargets);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }

            return result;
        }

        std::vector<Node*> Scene::pickNodes(const std::vector<Vector2>& edges, bool renderTargets) const
        {
            std::vector<Node*> result;

            for (auto i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::vector<Node*> nodes = (*i)->pickNodes(edges, renderTargets);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }

            return result;
        }

        void Scene::enter()
        {
            entered = true;

            recalculateProjection();
            sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

            for (Layer* layer : layers)
            {
                layer->enter();
            }
        }

        void Scene::leave()
        {
            entered = false;

            eventHandler.remove();

            for (Layer* layer : layers)
            {
                layer->leave();
            }
        }

        bool Scene::handleWindow(Event::Type type, const WindowEvent&)
        {
            if (type == Event::Type::WINDOW_SIZE_CHANGE ||
                type == Event::Type::WINDOW_CONTENT_SCALE_CHANGE)
            {
                recalculateProjection();
            }

            return true;
        }

        bool Scene::handleMouse(Event::Type type, const MouseEvent& event)
        {
            switch (type)
            {
                case Event::Type::MOUSE_PRESS:
                {
                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerDownOnNode(0, node.first, event.position, node.second);
                    break;
                }
                case Event::Type::MOUSE_RELEASE:
                {
                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerUpOnNode(0, node.first, event.position);
                    break;
                }
                case Event::Type::MOUSE_MOVE:
                {
                    std::pair<Node*, ouzel::Vector3> previousNode = pickNode(event.position - event.difference);
                    pointerLeaveNode(0, previousNode.first, event.position);

                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerEnterNode(0, node.first, event.position);

                    auto i = pointerDownOnNodes.find(0);

                    if (i != pointerDownOnNodes.end())
                    {
                        pointerDragNode(0, i->second.first, event.position, event.difference, i->second.second);
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        bool Scene::handleTouch(Event::Type type, const TouchEvent& event)
        {
            switch (type)
            {
                case Event::Type::TOUCH_BEGIN:
                {
                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerDownOnNode(event.touchId, node.first, event.position, node.second);
                    break;
                }
                case Event::Type::TOUCH_END:
                {
                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerUpOnNode(event.touchId, node.first, event.position);
                    break;
                }
                case Event::Type::TOUCH_MOVE:
                {
                    std::pair<Node*, ouzel::Vector3> previousNode = pickNode(event.position - event.difference);
                    pointerLeaveNode(0, previousNode.first, event.position);

                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerEnterNode(0, node.first, event.position);

                    auto i = pointerDownOnNodes.find(event.touchId);

                    if (i != pointerDownOnNodes.end())
                    {
                        pointerDragNode(event.touchId, i->second.first, event.position, event.difference, i->second.second);
                    }
                    break;
                }
                case Event::Type::TOUCH_CANCEL:
                {
                    std::pair<Node*, ouzel::Vector3> node = pickNode(event.position);
                    pointerUpOnNode(event.touchId, node.first, event.position);
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        void Scene::pointerEnterNode(uint64_t pointerId, Node* node, const Vector2& position)
        {
            if (node)
            {
                Event event;
                event.type = Event::Type::ENTER_NODE;

                event.uiEvent.node = node;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.position = position;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerLeaveNode(uint64_t pointerId, Node* node, const Vector2& position)
        {
            if (node)
            {
                Event event;
                event.type = Event::Type::LEAVE_NODE;

                event.uiEvent.node = node;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.position = position;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerDownOnNode(uint64_t pointerId, Node* node, const Vector2& position, const Vector3& localPosition)
        {
            if (node)
            {
                pointerDownOnNodes[pointerId] = std::make_pair(node, localPosition);

                Event event;
                event.type = Event::Type::PRESS_NODE;

                event.uiEvent.node = node;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.position = position;
                event.uiEvent.localPosition = localPosition;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerUpOnNode(uint64_t pointerId, Node* node, const Vector2& position)
        {
            auto i = pointerDownOnNodes.find(pointerId);

            if (i != pointerDownOnNodes.end())
            {
                auto pointerDownOnNode = i->second;

                if (pointerDownOnNode.first)
                {
                    Event releaseEvent;
                    releaseEvent.type = Event::Type::RELEASE_NODE;

                    releaseEvent.uiEvent.node = pointerDownOnNode.first;
                    releaseEvent.uiEvent.touchId = pointerId;
                    releaseEvent.uiEvent.position = position;
                    releaseEvent.uiEvent.localPosition = pointerDownOnNode.second;

                    sharedEngine->getEventDispatcher()->postEvent(releaseEvent);

                    if (pointerDownOnNode.first == node)
                    {
                        Event clickEvent;
                        clickEvent.type = Event::Type::CLICK_NODE;

                        clickEvent.uiEvent.node = node;
                        clickEvent.uiEvent.touchId = pointerId;
                        clickEvent.uiEvent.position = position;

                        sharedEngine->getEventDispatcher()->postEvent(clickEvent);
                    }
                }
            }

            pointerDownOnNodes.erase(pointerId);
        }

        void Scene::pointerDragNode(uint64_t pointerId, Node* node, const Vector2& position,
                                    const Vector2& difference, const ouzel::Vector3& localPosition)
        {
            if (node)
            {
                Event event;
                event.type = Event::Type::DRAG_NODE;

                event.uiEvent.node = node;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.difference = difference;
                event.uiEvent.position = position;
                event.uiEvent.localPosition = localPosition;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }
    } // namespace scene
} // namespace ouzel
