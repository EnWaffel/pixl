#include "pixl/core/Shaders.h"

/*
* !!! pixl Shaders !!!
* Do not change anything here unless you know what you're doing!
*/

/*
-------------------- VERTEX SHADERS --------------------
*/

const char* PX_SHADER_VERTEX_BASIC = R"(
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 px_uv;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos, 1.0);
	px_uv = texCoord;
}
)";

const char* PX_SHADER_VERTEX_WINDOW = R"(
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 px_uv;

void main()
{
    gl_Position = vec4(pos, 1.0);
    px_uv = texCoord;
}
)";

const char* PX_SHADER_VERTEX_SKYBOX = R"(
layout (location = 0) in vec3 pos;

out vec3 px_uv;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos, 1.0);
    px_uv = pos;
}  
)";

/*
-------------------- FRAGMENT SHADERS --------------------
*/

const char* PX_SHADER_FRAGMENT_HEADER = R"(
// From Vertex Shader
in vec2 px_uv;

// Out
out vec4 fragColor;

// Uniforms
uniform sampler2D px_texture;
uniform vec2 px_uv_coord;
uniform vec2 px_uv_size;
uniform vec2 px_resolution;
uniform bool px_flip_x;
uniform bool px_flip_y;
uniform float px_time;
uniform vec4 px_color;
uniform bool px_only_color;
)";

const char* PX_SHADER_FRAGMENT_BASIC = R"(
void main()
{
	if (px_only_color)
	{
		fragColor = px_color;
	}
	else
	{
		vec2 uv = px_uv;
		if (px_flip_x)
		{
			uv.x = 1.0 - uv.x;
		}
		if (px_flip_y)
		{
			uv.y = 1.0 - uv.y;
		}

		vec4 c = texture(px_texture, uv * px_uv_size + px_uv_coord);
		fragColor = c * px_color;
	}
}
)";

const char* PX_SHADER_FRAGMENT_WINDOW = R"(
void main()
{
    fragColor = texture(px_texture, px_uv);
}
)";

const char* PX_SHADER_FRAGMENT_TEXT = R"(
void main()
{
	vec2 uv = px_uv;
	if (px_flip_x)
	{
		uv.x = 1.0 - uv.x;
	}
	if (px_flip_y)
	{
		uv.y = 1.0 - uv.y;
	}

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(px_texture, uv).r);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
    fragColor *= px_color;
}
)";

const char* PX_SHADER_VERTEX_3D = R"(
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec2 px_uv;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos, 1.0);
	px_uv = uv;
}
)";

const char* PX_SHADER_FRAGMENT_3D = R"(
in vec2 px_uv;

out vec4 fragColor;

uniform sampler2D px_diffuse1;
uniform sampler2D px_specular1;
uniform sampler2D px_normals1;
uniform sampler2D px_height1;

void main()
{
	fragColor = texture(px_diffuse1, px_uv);
}
)";

const char* PX_SHADER_FRAGMENT_SKYBOX = R"(
in vec3 px_uv;

out vec4 fragColor;

uniform samplerCube skybox;

void main()
{    
	fragColor = texture(skybox, px_uv);
}
)";
