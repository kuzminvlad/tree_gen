#version 430

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexColor;
layout (location=1) in vec3 vertexNormal;

uniform mat4 camera;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;
uniform vec3 LightPosition;

out vec3 vecColor;
out vec3 vecPosition;
out vec3 vecNormal;
out vec3 lightPosition;

void main()
{	
	vecNormal = normal * vertexNormal; 
	vecColor = vertexColor;
	vecPosition = vec3(view * model * vec4(vertexPosition,1.0));
	lightPosition = vec3(view * vec4(LightPosition,1.0));
	gl_Position = camera * model * vec4(vertexPosition, 1.0f);
}