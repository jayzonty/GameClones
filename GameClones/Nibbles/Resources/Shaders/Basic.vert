#version 330

layout(location = 0) in vec3 inVertexPos;
layout(location = 1) in vec4 inVertexColor;

out vec4 vertexColor;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(inVertexPos, 1.0);
    vertexColor = inVertexColor;
}
