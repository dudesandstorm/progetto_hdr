#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;

void main() {             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

    // 1. Apply Exposure
    // This scales the intensities before they are compressed.
    vec3 exposedColor = hdrColor * exposure;

    // 2. Reinhard Tone Mapping
    // Use the exposed color instead of the raw HDR color.
    vec3 mapped = exposedColor / (exposedColor + vec3(1.0));

    // 3. Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}