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
uniform vec3 disco_ball_position;
uniform sampler2D texture_1;
uniform vec3 object_color;
uniform vec3 pointLightPos[9];
uniform vec3 pointLightColor[9];
uniform vec3 reflectorsPos[4];
uniform vec3 reflectorsColor[4];
uniform vec3 reflectorsDirection[4];
uniform int noLightSources;
uniform int noSpotlights;
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
    float light_radius = 1.1f;
    if (lightPos == disco_ball_position) {
        light_radius = 7.2f;
    }
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
    float diffuse_light = material_kd * 5 * max(dot(world_normal, L), 0);
    if (lightPos == disco_ball_position) {
        diffuse_light = material_kd * 0.2 * max(dot(world_normal, L), 0);
    }
    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * 1 * pow(max(dot(V, R), 0), material_shininess);
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
    vec3 result_color = vec3(0, 0, 0);

    if (gridLightsOn != 0) {
        for(int i = 0 ; i < noLightSources; i++) {
            result_color += (PointLightContribution(pointLightPos[i], pointLightColor[i]));
        }
    }

    if (reflectorsOn != 0) {
        for(int i = 0; i < noSpotlights; i++) {
            result_color += (SpotLightContribution(reflectorsPos[i], reflectorsColor[i], reflectorsDirection[i]));
        }
    }

    if (globeOn != 0) {
        // light_dir  este directia de iluminare
        vec3 light_dir = world_position - disco_ball_position;
 
        // texcoord este coordonata de textura utilizata pentru esantionare
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) + time * 0.2;
        texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
 
        // color este culoarea corespunzatoare pozitiei world_position
        vec3 color = texture(texture_1, texcoord).xyz;
    

        result_color += PointLightContribution(disco_ball_position, color);
        //result_color += color;
    }

    // just for walls
    if (noLightSources <= 1) {
        out_color = vec4(result_color, 1);
        return;
    }

    // for dancers
    
    if ((result_color[0] < 0.1) && (result_color[1] < 0.1) && (result_color[2] < 0.1)) {
    out_color = vec4(result_color, 1);
	    //discard;
    } else {
        out_color = vec4(result_color, 1);
    }

    /*if ((result_color[0] < 0.1) && (result_color[1] < 0.1) && (result_color[2] < 0.1)) {
	    out_color = vec4(result_color, 1);
    } else {
        out_color = vec4(result_color, 1);
    }*/
}
