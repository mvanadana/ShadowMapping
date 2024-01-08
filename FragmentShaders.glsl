#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
out vec4 outColor;

void main() {
   
    // For simplicity, just output the vertex color in this example
    outColor = vec4(fragColor, 1.0);
}