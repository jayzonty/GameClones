#version 330

in vec2 vertexUV;

uniform vec4 color;
uniform sampler2D tex;

out vec4 fragColor;

void main() {
	vec4 texColor = texture(tex, vertexUV);
	fragColor = texColor;
    //fragColor = texColor * color;
	//fragColor = vec4(texColor.r, texColor.g, texColor.b, 1.0);
}
