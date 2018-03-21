#version 330 core

// This is a gouraud shader with some vertex manipulations

// Define constants
#define M_PI 3.141593

struct Light {
    vec3 position;
    vec3 color;
};

// -- Input attributes
// Note that for every vertex, all positions in its triangle are also included
// The is a cheat that ensures that the normals are always properly computed for
//   a face. At least it avoids reuploading all VBOs every frame.
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_position2;
layout (location = 2) in vec3 in_position3;

// -- Scene parameters
uniform Light u_lights[3];
uniform float u_spike;

// -- Matrices
uniform mat4 u_projectionMat;
uniform mat4 u_viewMat;
uniform mat4 u_modelMat;
uniform mat3 u_normalMat;

// -- Material
uniform vec3 u_color;
uniform float u_ka; // ambient multiplier
uniform float u_ks; // specular multiplier
uniform float u_kd; // diffuse multiplier
uniform float u_p; // specular power

// -- Output of vertex stage
out vec3 vertexColor;

vec3 getNormal( vec3 a, vec3 b, vec3 c ) {
  return normalize( cross( b - a, c - a ) );
}

vec3 fromHomogeneous( vec4 v ) {
    if ( v.w == 0 ) {
        return v.xyz;
    } else {
        return v.xyz / v.w;
    }
}

void main() {
    float xtraLen1 = max( 0, length( in_position ) - 1 );
    float xtraLen2 = max( 0, length( in_position2 ) - 1 );
    float xtraLen3 = max( 0, length( in_position3 ) - 1 );

    vec3 modP1 = normalize( in_position ) * pow( 1 + xtraLen1, u_spike );
    vec3 modP2 = normalize( in_position2 ) * pow( 1 + xtraLen2, u_spike );
    vec3 modP3 = normalize( in_position3 ) * pow( 1 + xtraLen3, u_spike );

    vec3 vertexPosition = fromHomogeneous( u_modelMat * vec4( modP1, 1.0 ) );
    vec3 N = normalize( u_normalMat * getNormal( modP1, modP2, modP3 ) );

    vec3 ambientLightColor;
    vec3 diffuseLightColor;
    vec3 specularLightColor;
    for ( int i = 0; i < 3; i++ ) {
        // Normalized vector pointing to the light
        vec3 L = normalize( u_lights[i].position - vertexPosition );
        vec3 R = 2 * dot( N, L ) * N - L; // Mirror of L along surface normal
        vec3 V = normalize( -vertexPosition ); // Points toward the camera
        ambientLightColor += ( u_lights[i].color * u_ka ) / 3; // Average ambient color
        diffuseLightColor += u_lights[i].color * u_kd * max( 0, dot( N, L ) );
        specularLightColor += u_lights[i].color * u_ks * pow( max( 0, dot( R, V ) ), u_p );
    }

    vertexColor = ( ambientLightColor + diffuseLightColor ) * u_color + specularLightColor;
    gl_Position = u_projectionMat * u_viewMat * vec4( vertexPosition, 1.0 );
}
