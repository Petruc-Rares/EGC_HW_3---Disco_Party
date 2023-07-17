#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 disco_ball_position;
uniform sampler2D texture_1;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int is_spotlight;
uniform float cutoff_angle;
uniform int noSpotlights;
uniform float time;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 PointLightContribution(vec3 lightPos, vec3 lightColor)
{
    return lightColor * 1.4;
}

void main()
{

    // light_dir  este directia de iluminare
    vec3 light_dir = world_position - disco_ball_position;
 
    // texcoord este coordonata de textura utilizata pentru esantionare
    vec2 texcoord;
    texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) + time * 0.2;
    texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
 
    // color este culoarea corespunzatoare pozitiei world_position
    vec3 color = texture(texture_1, texcoord).xyz;

    //color = 0;
    color = PointLightContribution(disco_ball_position, color);
    out_color = vec4(color, 1);
}
