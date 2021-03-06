// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include "utils/Noncopyable.hpp"
#include "math/Vector2.hpp"
#include "events/EventHandler.hpp"

namespace ouzel
{
    namespace scene
    {
        class SceneManager;
        class Layer;

        class Scene: public Noncopyable
        {
            friend SceneManager;
        public:
            Scene();
            virtual ~Scene();

            virtual void draw();

            void addLayer(Layer* layer)
            {
                addChildLayer(layer);
            }
            template<class T> void addLayer(const std::unique_ptr<T>& layer)
            {
                addChildLayer(layer.get());
            }
            template<class T> void addLayer(std::unique_ptr<T>&& layer)
            {
                addChildLayer(layer.get());
                ownedLayers.push_back(std::move(layer));
            }

            bool removeLayer(Layer* layer)
            {
                return removeChildLayer(layer);
            }
            template<class T> bool removeLayer(const std::unique_ptr<T>& layer)
            {
                return removeChildLayer(layer.get());
            }
            void removeAllLayers();
            bool hasLayer(Layer* layer) const;
            const std::vector<Layer*>& getLayers() const { return layers; }

            virtual void recalculateProjection();

            std::pair<Node*, ouzel::Vector3> pickNode(const Vector2& position, bool renderTargets = false) const;
            std::vector<std::pair<Node*, ouzel::Vector3>> pickNodes(const Vector2& position, bool renderTargets = false) const;
            std::vector<Node*> pickNodes(const std::vector<Vector2>& edges, bool renderTargets = false) const;

        protected:
            virtual void addChildLayer(Layer* layer);
            virtual bool removeChildLayer(Layer* layer);

            virtual void enter();
            virtual void leave();

            bool handleWindow(Event::Type type, const WindowEvent& event);
            bool handleMouse(Event::Type type, const MouseEvent& event);
            bool handleTouch(Event::Type type, const TouchEvent& event);

            void pointerEnterNode(uint64_t pointerId, Node* node, const Vector2& position);
            void pointerLeaveNode(uint64_t pointerId, Node* node, const Vector2& position);
            void pointerDownOnNode(uint64_t pointerId, Node* node, const Vector2& position, const Vector3& localPosition);
            void pointerUpOnNode(uint64_t pointerId, Node* node, const Vector2& position);
            void pointerDragNode(uint64_t pointerId, Node* node, const Vector2& position,
                                 const Vector2& difference, const ouzel::Vector3& localPosition);

            SceneManager* sceneManger = nullptr;

            std::vector<Layer*> layers;
            std::vector<std::unique_ptr<Layer>> ownedLayers;
            ouzel::EventHandler eventHandler;

            std::unordered_map<uint64_t, std::pair<Node*, ouzel::Vector3>> pointerDownOnNodes;

            bool entered = false;
        };
    } // namespace scene
} // namespace ouzel
