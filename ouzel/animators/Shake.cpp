// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <limits>
#include "Shake.hpp"
#include "scene/Node.hpp"
#include "math/MathUtils.hpp"
#include "utils/Utils.hpp"

namespace ouzel
{
    namespace scene
    {
        Shake::Shake(float aLength, const Vector3& aDistance, float aTimeScale):
            Animator(aLength), distance(aDistance), timeScale(aTimeScale)
        {
            seedX = std::uniform_int_distribution<uint32_t>{0, std::numeric_limits<uint32_t>::max()}(randomEngine);
            seedY = std::uniform_int_distribution<uint32_t>{0, std::numeric_limits<uint32_t>::max()}(randomEngine);
            seedZ = std::uniform_int_distribution<uint32_t>{0, std::numeric_limits<uint32_t>::max()}(randomEngine);
        }

        void Shake::play()
        {
            Animator::play();

            if (targetNode)
            {
                startPosition = targetNode->getPosition();
            }
        }

        void Shake::updateProgress()
        {
            Animator::updateProgress();

            if (targetNode)
            {
                float x = length * progress * timeScale;

                uint64_t x1 = static_cast<uint32_t>(x);
                uint64_t x2 = x1 + 1;
                float t = x - static_cast<float>(x1);

                Vector3 previousPosition;
                Vector3 nextPosition;

                if (x1 != 0)
                {
                    previousPosition.x = (2.0f * (static_cast<float>(fnvHash(seedX | (x1 << 32))) / std::numeric_limits<uint64_t>::max()) - 1.0f) * distance.x;
                    previousPosition.y = (2.0f * (static_cast<float>(fnvHash(seedY | (x1 << 32))) / std::numeric_limits<uint64_t>::max()) - 1.0f) * distance.y;
                    previousPosition.z = (2.0f * (static_cast<float>(fnvHash(seedZ | (x1 << 32))) / std::numeric_limits<uint64_t>::max()) - 1.0f) * distance.z;
                }

                if (x2 != static_cast<uint32_t>(timeScale))
                {
                    nextPosition.x = (2.0f * (static_cast<float>(fnvHash(seedX | (x2 << 32))) / std::numeric_limits<uint64_t>::max()) - 1.0f) * distance.x;
                    nextPosition.y = (2.0f * (static_cast<float>(fnvHash(seedY | (x2 << 32))) / std::numeric_limits<uint64_t>::max()) - 1.0f) * distance.y;
                    nextPosition.z = (2.0f * (static_cast<float>(fnvHash(seedZ | (x2 << 32))) / std::numeric_limits<uint64_t>::max()) - 1.0f) * distance.z;
                }

                Vector3 noise(smoothStep(previousPosition.x, nextPosition.x, t),
                              smoothStep(previousPosition.y, nextPosition.y, t),
                              smoothStep(previousPosition.z, nextPosition.z, t));

                targetNode->setPosition(startPosition + noise);
            }
        }
    } // namespace scene
} // namespace ouzel
