#version 120

// base color
uniform vec3 color;


// for lighting
varying vec3 FragPos;
varying vec3 Normal;
varying vec3 LightPos;

// for textures
varying vec2 UV;
uniform bool texture_mode;
uniform bool material_mode;
uniform sampler2D textureSampler;


uniform vec3 view_pos;

struct Light{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float ambient_intensity;
    float diffuse_intensity;
    float specular_intensity;
};

struct Material {
    bool has_ambient_texture;
    bool has_diffuse_texture;
    bool has_specular_texture;
    sampler2D ambient_sampler;
    sampler2D diffuse_sampler;
    sampler2D specular_sampler;
    sampler2D bump_sampler;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
    float bump_multiplier;
    float dissolve;
};


uniform Light light;
uniform Material material;

void main() {

    float default_shininess = 16.0;

    vec3 ambient_contribution = light.ambient_color;
    vec3 diffuse_contribution = light.diffuse_color;
    vec3 specular_contribution = light.specular_color;

    vec3 ambient_color = vec3(1.0);
    vec3 diffuse_color = vec3(1.0);
    vec3 specular_color = vec3(1.0);
    float shininess = 16.0;

    float bump_value = texture2D(material.bump_sampler, UV).r;
    float ambient_intensity = 1.0;
    float diffuse_intensity = 1.0;
    float specular_intensity = 1.0;
    if (material_mode) {

        if (material.has_diffuse_texture) {
            diffuse_color *= texture2D(material.diffuse_sampler, UV).rgb;
        }

        if (material.diffuse_color != vec3(0.0, 0.0, 0.0)) {
            diffuse_color *= material.diffuse_color;
        }

        if (material.has_ambient_texture) {
            ambient_color *= texture2D(material.ambient_sampler, UV).rgb;
            ambient_intensity = light.ambient_intensity;
        } else if (material.ambient_color == vec3(0.0, 0.0, 0.0) || material.ambient_color == vec3(1.0, 1.0, 1.0)){
            ambient_color *= diffuse_color;
        }

        if (material.has_specular_texture) {
            specular_color *= texture2D(material.specular_sampler, UV).rgb;
        } else if (material.specular_color == vec3(0.0, 0.0, 0.0) || material.specular_color == vec3(1.0, 1.0, 1.0)){
            specular_color *= diffuse_color;
            specular_intensity = light.specular_intensity;
        }

        if (material.specular_color != vec3(0.0, 0.0, 0.0)) {
           specular_color *= material.specular_color;
        }

        if (material.ambient_color != vec3(0.0, 0.0, 0.0)) {
           ambient_color *= material.ambient_color;
        }

        // float ambient_intensity = (0.212671*ambient_color.r + 0.715160*ambient_color.g+ 0.072169*ambient_color.b)/(0.212671*diffuse_color.r + 0.715160*diffuse_color.g + 0.072169*diffuse_color.b);

        shininess = material.shininess;
    } else {
        vec3 base_color = (texture_mode) ? texture2D(textureSampler, UV).rgb : color;
        ambient_intensity = light.ambient_intensity;
        diffuse_intensity = light.diffuse_intensity;
        specular_intensity = light.specular_intensity;
        ambient_color = base_color;
        diffuse_color = base_color;
        specular_color = base_color;
    }

    diffuse_contribution *=  diffuse_color * diffuse_intensity;
    ambient_contribution *= ambient_color * ambient_intensity;
    specular_contribution *=  specular_color * specular_intensity;

    // Actual calculations
    vec3 Normal0 = normalize(Normal + bump_value * material.bump_multiplier);
    if (!gl_FrontFacing) {
        Normal0 = -Normal0;
    }
    vec3 light_direction = normalize(LightPos - FragPos);
    vec3 view_direction = normalize(view_pos - FragPos);

    // Diffuse contribution
   float diffuse_factor = max(dot(Normal0, light_direction), 0.0);
   diffuse_contribution *= diffuse_factor;

    // Specular contribution
   vec3 reflect_direction = reflect(-light_direction, Normal0);
   float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
   specular_contribution *= specular_factor;


   // illum models

   // Total light contribution
   vec3 lighting = ambient_contribution + diffuse_contribution + specular_contribution;

   gl_FragColor = vec4(lighting, 1.0);

    // neon mode
    // gl_FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);

}
