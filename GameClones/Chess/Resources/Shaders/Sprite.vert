#version 330

layout(location = 0) in vec3 inVertexPos;
layout(location = 1) in vec2 inVertexUV;

uniform mat4 mvp;

out vec2 vertexUV;

void main() {
    gl_Position = mvp * vec4(inVertexPos, 1.0);
	
	vertexUV = inVertexUV;
}
