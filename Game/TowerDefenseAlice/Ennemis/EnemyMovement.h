#pragma once
#include "Core/Component.h"
#include "Maths/Vector2.h"
#include <vector>

namespace TowerDefenseAlice
{
    class EnemyMovement : public Component {
    public:
        std::vector<Maths::Vector2f> waypoints;
        int currentWaypointIndex = 0;
        float speed = 100.0f;

        void Update(float _delta_time) {
            if (waypoints.empty() || currentWaypointIndex >= waypoints.size()) {
                return;
            }

            Maths::Vector2f target = waypoints[currentWaypointIndex];
            Maths::Vector2f pos = GetOwner()->GetPosition();
            Maths::Vector2f direction = target - pos;
            float distanceSq = direction.x * direction.x + direction.y * direction.y;

            if (distanceSq < 25.0f) {
                currentWaypointIndex++;
            }
            else {
                float distance = std::sqrt(distanceSq);
                direction.x /= distance; direction.y /= distance;
                pos.x += direction.x * speed * _delta_time;
                pos.y += direction.y * speed * _delta_time;
                GetOwner()->SetPosition(pos);
            }
        }

        void Reset() {
            currentWaypointIndex = 0;
        }
    };
}