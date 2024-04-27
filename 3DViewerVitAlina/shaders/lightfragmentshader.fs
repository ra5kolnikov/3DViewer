#version 120
//uniform vec3 color;


// for lighting
varying vec3 FragPos;
varying vec3 Normal;
varying vec3 LightPos;

// for textures

varying vec2 UV;
uniform bool textureMode;
uniform sampler2D textureSampler;

uniform bool lightOnMode;
uniform vec3 lightColor;


uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess_exponent;
uniform vec3 viewPos;

void main() {
    vec3 a = ambient_color;
    vec3 d = diffuse_color;
    vec3 s = specular_color;
    if (textureMode) {
        vec4 texColor = texture2D(textureSampler, UV);
        a *= texColor.rgb;
        d *= texColor.rgb;
        s *= texColor.rgb;
    }
    // Ambient light
    float ambient_intensity = 0.2;

    vec3 ambient = lightColor * ambient_intensity * a;

    // Diffuse light
    vec3 Normal0 = normalize(Normal);


    float diffuse_intensity = 0.76;
    vec3 lightDir = normalize(LightPos - FragPos);
    float diffuse_factor = dot(Normal0, lightDir);
    diffuse_factor = max(diffuse_factor, 0.0);
    vec3 diffuse = lightColor * diffuse_intensity * d * diffuse_factor;

    // Specular light
    float specular_intensity = 0.4;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal0);
    float specular_factor = max(dot(viewDir, reflectDir), 0.0);
    specular_factor = pow(specular_factor, shininess_exponent);
    vec3 specular = lightColor * specular_intensity * s * specular_factor;

    // Final color
    vec3 result = clamp((ambient + diffuse + specular), 0, 1);
    gl_FragColor = vec4(result, 1.0);

}
