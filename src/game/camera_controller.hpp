#pragma once

#include "components/custom.hpp"

namespace components {
	class Transform;
}

class CameraController : public components::CustomComponent {

public:
	CameraController(Object* parent);
	void onUpdate(glm::mat4 t) override;

	float m_cameraSensitivity = 0.007f;

private:
	components::Transform* tcomp;
	float m_yaw = 0.0f;
	float m_pitch = 0.0f;

	float walk_speed = 4.0f;
	
	bool isJumping = false;
	float dy = 0.0f;
	float standingHeight = 0.0f;
	float thrust = 25.0f;

};
