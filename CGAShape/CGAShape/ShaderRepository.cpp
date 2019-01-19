#include "stdafx.h"
#include "ShaderRepository.h"

#define GLSL(src) "#version 150 core\n" #src

const char* ShaderRepository::vertex3D = GLSL(
	in vec3 position;
	in vec4 color;
	in vec3 normal;
	in vec2 texCoords;
	in float depth;

	out vec4 Color;
	out vec3 Normal;
	out vec2 TexCoords;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	const float DEPTH_EPSILON = 0.00005;

	void main() {
		gl_Position = projection * view * model * vec4(position, 1.0);
		gl_Position.z -= depth * DEPTH_EPSILON;

		Color = color;
		Normal = normal;
		TexCoords = texCoords;
	}
);

const char* ShaderRepository::fragment3D = GLSL(
	in vec4 Color;
	in vec3 Normal;
	in vec2 TexCoords;

	out vec4 outColor;

	uniform vec3 sunDirection;
	uniform sampler2D tex;

	const float ambientStrength = 0.2;
	const vec3 lightColor = vec3(1.0, 1.0, 1.0);

	void main() {
		//Ambient
		vec3 ambient = ambientStrength * lightColor;

		//Diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(sunDirection);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		vec3 result = (ambient + diffuse) * Color.rgb;
		outColor = texture(tex, TexCoords) * vec4(result, 1.0f);
	}
);

const char* ShaderRepository::singleColorVertex3D = GLSL(
	in vec3 position;
	in vec3 normal;
	in vec2 texCoords;
	in float depth;

	out vec3 Normal;
	out vec2 TexCoords;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	const float DEPTH_EPSILON = 0.00005;

	void main() {
		gl_Position = projection * view * model * vec4(position, 1.0);
		gl_Position.z -= depth * DEPTH_EPSILON;

		Normal = normal;
		TexCoords = texCoords;
	}
);

const char* ShaderRepository::singleColorFragment3D = GLSL(
	in vec3 Normal;
	in vec2 TexCoords;

	out vec4 outColor;

	uniform vec3 sunDirection;
	uniform vec4 color;
	uniform sampler2D tex;

	const float ambientStrength = 0.2;
	const vec3 lightColor = vec3(1.0, 1.0, 1.0);

	void main() {
		//Ambient
		vec3 ambient = ambientStrength * lightColor;

		//Diffuse 
		vec3 norm = normalize(Normal);
		if (!gl_FrontFacing) {
			norm *= -1;
		}

		vec3 lightDir = normalize(sunDirection);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		vec3 result = (ambient + diffuse) * color.rgb;
		outColor = texture(tex, TexCoords) * vec4(result, 1.0f);
	}
);