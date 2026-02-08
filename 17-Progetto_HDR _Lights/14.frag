#version 330

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

// Struttura dati di lavoro per contenere le informazioni sulla luce
// ambientale
struct AmbientLightStruct {
	vec3 color;
	float intensity;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// direzionale
struct DirectionalLightStruct {
	vec3 color;
	vec3 direction;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// diffusiva
struct DiffusiveLightStruct {
	float intensity;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// speculare
struct SpecularLightStruct {
	float intensity;
	float shininess;
};
// NCODE
struct PointLightStruct {
	vec3 color; // Color of the light
    vec3 position;  // Position of the light
    float intensity; // Intensity of the light
    float range;    // Distance where the light reaches zero
};
// ECODE
// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
in vec3 fragment_color;

// Vettori della normali ricevuti dal vertex shader
in vec3 fragment_normal;

// Coordinate spaziali dei punti ricervuti dal vertex shader
in vec3 fragment_position;

// Coordinate di texture dei punti ricervuti dal vertex shader
in vec2 fragment_textcoord;

// Informazioni di luce ambientale 
uniform AmbientLightStruct AmbientLight;

// Informazioni di luce direzionale 
uniform DirectionalLightStruct DirectionalLight;

// Informazioni di luce diffusiva 
uniform DiffusiveLightStruct DiffusiveLight;

// Informazioni di luce speculare 
uniform SpecularLightStruct SpecularLight;

// NCODE
uniform PointLightStruct PointLights[MAX_POINT_LIGHTS];
uniform int NumPointLights;
// ECODE

// Posizione della camera in coordinate mondo
uniform vec3 CameraPosition;

uniform sampler2D ColorTextSampler;

out vec4 out_color;

vec3 computePointLight(PointLightStruct light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor)
{
    // Direction from surface to light
    vec3 light_vec = light.position - fragPos;
    float distance = length(light_vec);
    vec3 light_dir = normalize(light_vec);

    // Attenuation calculation
    float attenuation = clamp(1.0 - (distance / light.range), 0.0, 1.0);

    // Diffuse (Lambert)
    float cosTheta = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = baseColor * (light.color * light.intensity) * cosTheta;

    // Specular (Phong)
    vec3 reflect_dir = reflect(-light_dir, normal);
    float cosAlpha = max(dot(viewDir, reflect_dir), 0.0);
    vec3 specular = baseColor * (light.color * SpecularLight.intensity) * pow(cosAlpha, SpecularLight.shininess);

    return (diffuse + specular) * attenuation;
}

void main()
{
	// La funzione texture ritorna un vec4. Nel codice noi rappresentiamo
	// i colori con vec3 e dobbiamo quindi estrarre solo 3 componenti.
	vec4 material_color = texture(ColorTextSampler, fragment_textcoord);

	vec3 I_amb =  material_color.rgb * (AmbientLight.color * AmbientLight.intensity);

	vec3 I_dif = vec3(0,0,0);

	vec3 normal = normalize(fragment_normal);

	float cosTheta = dot(normal,-DirectionalLight.direction);

	if (cosTheta>0) {
		I_dif = material_color.rgb * (DirectionalLight.color * DiffusiveLight.intensity) * cosTheta;
	}

	vec3 I_spec = vec3(0,0,0);

	vec3 view_dir    = normalize(CameraPosition - fragment_position);
	vec3 reflect_dir = normalize(reflect(DirectionalLight.direction,normal));

	float cosAlpha = dot(view_dir,reflect_dir);
	if (cosAlpha>0) {
		I_spec = material_color.rgb * (DirectionalLight.color * SpecularLight.intensity) * pow(cosAlpha,SpecularLight.shininess);
	}

	vec3 point_light_res = vec3(0);
	for (int i = 0; i < NumPointLights; i++) {
		point_light_res += computePointLight(PointLights[i], normal, fragment_position, view_dir, material_color.rgb);
	}

	//out_color = vec4(I_amb + I_dif + I_spec + point_light_res, material_color.a);
	out_color = vec4(point_light_res, material_color.a);
}