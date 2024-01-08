#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal; // Include normal attribute

out vec3 fragColor;
out vec3 fragNormal; // Pass the normal to the fragment shader

uniform mat4 matrix;
uniform vec4 lightPosition; // Position of the light in world coordinates

void main() {
    gl_Position = matrix * vec4(inPosition, 1.0);
    fragColor = inColor;

    // Transform the normal to world space
    fragNormal = normalize(mat3(transpose(inverse(matrix))) * inNormal);

    // Calculate the direction from the vertex to the light source
    vec3 lightDirection = normalize(lightPosition.xyz - gl_Position.xyz);

    
}