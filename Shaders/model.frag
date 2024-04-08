#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D norm_tex;

//general
uniform vec3 lightColor;
uniform vec3 ambientColor;

//direction light properties
uniform vec3 lightDirection;
uniform vec3 dir_lightPos;
uniform float dir_brightness;
uniform float dir_ambientStr;
uniform float dir_specStr;
uniform float dir_specPhong;


//point light properties
uniform vec3 point_lightPos;
uniform float point_brightness;
uniform float point_ambientStr;
uniform float point_specStr;
uniform float point_specPhong;
uniform float intensity;


uniform vec3 cameraPos;

in vec3 normCoord;
in vec3 fragPos;
in vec2 texCoord;
in mat3 TBN;

 void main ()
 {	


	//variables for ATTENUATION, utilized for point light
	float constant = 1.0f;      
	float linear = 0.02f;
	float quadratic = 0.001f;
	float distance = length(point_lightPos - fragPos);
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));


	//CONVERTS RGB TO NORMAL
	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);


	//MULTIPLY THE NORMAL WITH THE TBN MATRIX
	normal = normalize(TBN * normal);




	//-----------------DIRECTION LIGHT-----------------
	vec3 dir_lightDir = normalize(-lightDirection);

	//DIFFUSE
	float dir_diff = max(dot(normal, dir_lightDir), 0.0f);			
	vec3 dir_diffuse = dir_diff * lightColor * dir_brightness;

	//AMBIENT
	vec3 dir_ambientCol = ambientColor * dir_ambientStr * dir_brightness;

	//SPECULAR
	vec3 dir_viewDir = normalize(cameraPos - fragPos);
	vec3 dir_reflectDir = reflect(-dir_lightDir, normal);
	float dir_spec = pow(max(dot(dir_reflectDir, dir_viewDir), 0.1), dir_specPhong);
	vec3 dir_specColor = dir_spec * dir_specStr * lightColor * dir_brightness;

	vec4 directionLight = vec4(dir_diffuse + dir_ambientCol + dir_specColor, 1.0);
	



	//-------------------POINT LIGHT-------------------------
	
	vec3 point_lightDir = normalize(point_lightPos - fragPos);


	//DIFFUSE
	float point_diff = max(dot(normal, point_lightDir), 0.0);			
	vec3 point_diffuse = point_diff * lightColor * point_brightness;

	//AMBIENT
	vec3 point_ambientCol = ambientColor * point_ambientStr * point_brightness;

	//SPECULAR
	vec3 point_viewDir = normalize(cameraPos - fragPos);
	vec3 point_reflectDir = reflect(-point_lightDir, normal);
	float point_spec = pow(max(dot(point_reflectDir, point_viewDir), 0.1), point_specPhong);
	vec3 point_specColor = point_spec * point_specStr * lightColor * point_brightness;

	point_ambientCol = point_ambientCol * (attenuation * intensity);
	point_diffuse = point_diffuse * (attenuation * intensity);
	point_specColor = point_specColor * (attenuation * intensity);

	
	vec4 pointLight = vec4(point_diffuse + point_ambientCol + point_specColor, 1.0);
	


	FragColor = (directionLight + pointLight) * texture(tex0, texCoord);
	
 }