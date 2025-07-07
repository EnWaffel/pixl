#pragma once

/*
* !!! pixl Shaders !!!
* Do not change anything here unless you know what you're doing!
*/

/*
    Predefined uniforms and vars (only in shaders that are built with header on!):

    px_texture - input texture
    px_color - blend color
    px_uv_coord - texture uv coord
    px_uv_size - texture uv size
    px_flip_x - flag, whether the output should be flipped on the x axis
    px_flip_y - flag, whether the output should be flipped on the y axis
    px_resolution - camera resolution
    px_time - time in seconds

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

const char* PX_SHADER_FRAGMENT_HEADER = R"(
// From Vertex Shader
in vec2 px_uv;

// Uniforms
uniform sampler2D px_texture;
uniform vec2 px_uv_coord;
uniform vec2 px_uv_size;
uniform vec2 px_resolution;
uniform bool px_flip_x;
uniform bool px_flip_y;
uniform float px_time;
uniform vec4 px_color;
)";

const char* PX_SHADER_FRAGMENT_BASIC = R"(
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

    vec4 c = texture(px_texture, uv * px_uv_size + px_uv_coord);
    gl_FragColor = c * px_color;
}
)";

const char* PX_SHADER_FRAGMENT_WINDOW = R"(
void main()
{
    gl_FragColor = texture(px_texture, px_uv);
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

    gl_FragColor = vec4(1.0, 1.0, 1.0, texture(px_texture, uv).r) * px_color;
}
)";