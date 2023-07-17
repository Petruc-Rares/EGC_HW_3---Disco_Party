#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int is_spotlight;
uniform float cutoff_angle;
uniform int noSpotlights;

uniform sampler2D texture_1;
uniform vec3 disco_ball_position;
uniform vec3 object_color;
uniform vec3 reflectorsPos[4];
uniform vec3 reflectorsColor[4];
uniform vec3 reflectorsDirection[4];
uniform float time;
uniform int globeOn;
uniform int reflectorsOn;
uniform int gridLightsOn;


// Output
layout(location = 0) out vec4 out_color;

vec3 PointLightContribution(vec3 lightPos, vec3 lightColor)
{
    float dist = distance(lightPos, world_position);
    float fa = 0;
    float light_radius = 8.f;
    if (dist < light_radius) {
        fa = pow(light_radius - dist, 2);
    }
    vec3 color;
    
    
    //float fa = 1 / (pow(distance(lightPos, world_position), 2) + 0.2);

    vec3 L = normalize( lightPos - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 R = reflect(-L, world_normal);

    
    float ambient_light =  0;
    float diffuse_light = material_kd * 0.2 * max(dot(world_normal, L), 0);
    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * 0.2 * pow(max(dot(V, R), 0), 3);
    }
    
    float intensity;
    
	intensity = ambient_light + fa * (diffuse_light + specular_light );
    
    // eventual aduni si emisiva
    return lightColor * intensity;
}

vec3 SpotLightContribution(vec3 lightPos, vec3 lightColor, vec3 lightDirection)
{
    float dist = distance(lightPos, world_position);
    float fa = 0;
    float light_radius = 0.2f;
    if (dist < light_radius) {
        fa = pow(light_radius - dist, 2);
    }

    
    vec3 L = normalize( lightPos - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 R = reflect(-L, world_normal);

    
    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light =  0;
    float diffuse_light = material_kd * 5 * max(dot(world_normal, L), 0);
    float specular_light = 0;


    if (diffuse_light > 0)
    {
        specular_light = material_ks * 3 * pow(max(dot(V, R), 0), material_shininess);
    }
    
    float cut_off = radians(cutoff_angle);
    float spot_light = dot(-L, lightDirection);
    float spot_light_limit = cos(cut_off);

    float intensity = 0;
        
    if (spot_light > cos(cut_off))
    {
	    // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	    // se calculeaza atenuarea luminii
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);
            
        intensity = ambient_light + light_att_factor * (diffuse_light + specular_light );
    }
    
       
    return lightColor * intensity;
}

void main()
{
    //intensity = 0.25;
    
    vec3 result_color = vec3(0, 0, 0);

    if (reflectorsOn != 0) {
        for(int i = 0; i < noSpotlights; i++) {
            result_color += (SpotLightContribution(reflectorsPos[i], reflectorsColor[i], reflectorsDirection[i]));
        }
    }

    vec3 light_dir = world_position - disco_ball_position;
 
    // texcoord este coordonata de textura utilizata pentru esantionare
    
    if (globeOn != 0) {
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) + time * 0.2;
        texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
 
        // color este culoarea corespunzatoare pozitiei world_position
        vec3 color = texture(texture_1, texcoord).xyz;
    
        //result_color += color;
        result_color += PointLightContribution(disco_ball_position, color);
    }

    if (gridLightsOn != 0) {
        // add emissive component
        result_color += object_color;
    }

    out_color = vec4(result_color, 1);
}
