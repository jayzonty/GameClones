#version 330

in vec4 vertexColorOut;

out vec4 fragColor;

void main() {
    fragColor = vertexColorOut;
}
