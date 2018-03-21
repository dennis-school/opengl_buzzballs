#version 330 core

// Define constants
#define M_PI 3.141593

// Interpolated vertex attributes
in vec3 vertexColor;

// Output pixel color
out vec4 fColor;

void main( ) {
    fColor = vec4( vertexColor, 1.0 );
}
