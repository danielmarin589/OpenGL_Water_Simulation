#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

uniform float C_Ai;
uniform vec2 Ci;
uniform float C_Wi;
uniform float time;
uniform float C_phase;

void main()
{
	textureCoord = texCoord;

	vec2 Di = (Ci - pos.xz)/length(Ci - pos.xz);
	float y = 2*C_Ai*pow(((sin(dot(Di,pos.xz)*C_Wi + time * C_phase) + 1)/2),2);
	
	
	fragPos = vec3(model * vec4(pos.x, pos.y, pos.z, 1.0f));
	norm = mat3(transpose(inverse(model)))*normals;
	gl_Position = MVP * vec4(pos.x, y, pos.z, 1.0f);
}