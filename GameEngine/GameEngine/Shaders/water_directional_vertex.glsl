#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

uniform float D_Ai;
uniform vec2 D_Di;
uniform float D_Wi;
uniform float time;
uniform float D_phase;

void main()
{
	textureCoord = texCoord;
	float y = 2*D_Ai*pow(((sin(dot(D_Di,pos.xz)*D_Wi + time * D_phase) + 1)/2),2);
	
	
	fragPos = vec3(model * vec4(pos.x, pos.y, pos.z, 1.0f));
	norm = mat3(transpose(inverse(model)))*normals;
	gl_Position = MVP * vec4(pos.x, y, pos.z, 1.0f);
}