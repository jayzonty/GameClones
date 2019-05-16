#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

out vec4 vertexColorOut;

uniform mat4 projectionViewModelMatrix;

void main() {
    gl_Position = projectionViewModelMatrix * vec4(vertexPosition, 1.0);
    vertexColorOut = vertexColor;
}
