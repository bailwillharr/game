#version 450

layout(binding = 0) uniform UBO {
	mat4 transform;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragColor;

void main() {
	gl_Position = vec4(inPosition.x/10, inPosition.y/10, 0.0, 1.0);
	fragColor = vec3(1.0, 0.0, 0.0);
}
