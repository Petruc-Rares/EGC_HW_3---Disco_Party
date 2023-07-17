#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int is_spotlight;
uniform float cutoff_angle;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    //intensity = 0.25;
    
    out_color = vec4(object_color * 1, 0.5);
}
