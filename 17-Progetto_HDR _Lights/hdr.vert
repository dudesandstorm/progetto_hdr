#version 330 core

// Layout 0: Position (-1.0 to 1.0)
// Layout 1: Texture Coordinates (0.0 to 1.0)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    // We pass the texture coordinates to the fragment shader
    TexCoords = aTexCoords;
    
    // We set the position directly. 
    // Since the quad is already defined in NDC (-1 to 1), 
    // we don't need any transformations.
    gl_Position = vec4(aPos, 1.0);
}