#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform vec3 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

// for lighting

varying mediump vec3 Normal;
varying mediump vec3 FragPos;
varying mediump vec3 LightPos;

// for textures
varying vec2 UV;


// Function definitions for transpose and inverse
mat3 transpose(mat3 m) {
    return mat3(
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
    );
}

mat3 inverse(mat3 m) {
    float det = determinant(m);
    if (det == 0.0) {
        return mat3(0.0); // Matrix is not invertible
    }

    mat3 adjoint = mat3(
        m[1][1]*m[2][2] - m[1][2]*m[2][1],
        m[0][2]*m[2][1] - m[0][1]*m[2][2],
        m[0][1]*m[1][2] - m[0][2]*m[1][1],

        m[1][2]*m[2][0] - m[1][0]*m[2][2],
        m[0][0]*m[2][2] - m[0][2]*m[2][0],
        m[0][2]*m[1][0] - m[0][0]*m[1][2],

        m[1][0]*m[2][1] - m[1][1]*m[2][0],
        m[0][1]*m[2][0] - m[0][0]*m[2][1],
        m[0][0]*m[1][1] - m[0][1]*m[1][0]
    );

    return adjoint / det;
}

void main() {
    mat4 modelViewMatrix =  viewMatrix * modelMatrix;
    FragPos = vec3(modelViewMatrix * vec4(position, 1.0));
    mat3 normalMatrix0 = mat3(transpose(inverse(mat3(modelViewMatrix))));
    Normal = mat3(normalMatrix0) * normal;
    LightPos = vec3(viewMatrix * vec4(lightPos, 1.0));
    UV = uv;
    gl_Position = projectionMatrix * vec4(FragPos, 1.0);
}
