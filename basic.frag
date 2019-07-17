#version 430

in vec3 vecColor;
in vec3 vecPosition;
in vec3 vecNormal;
in vec3 lightPosition;

out vec4 fragColor;

vec3 ads()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 ambient = 0.6 * lightColor;

	vec3 n = normalize(vecNormal);
	vec3 s = normalize(lightPosition - vecPosition);
	vec3 diffuse = 0.5 * max(dot(n,s), 0.0) * lightColor;

	vec3 v = normalize(-vecPosition);
	vec3 r = reflect(-s,n);
	float shininess = 5.0;
	vec3 specular = 0.5 * pow(max(dot(r,v), 0.0), shininess) * lightColor;
	
	return (ambient + diffuse + specular) * vecColor;
}

void main() 
{
	fragColor = vec4(ads(), 1.0);
}