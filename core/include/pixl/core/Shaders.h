#pragma once

/*
* !!! pixl Shaders !!!
* Do not change anything here unless you know what you're doing!
*
* Actual shader code is located in src/Shaders.cpp
*/

/*
    Predefined uniforms and vars (only in shaders that are built with header on!):

    px_texture - input texture
    px_color - color
    px_uv_coord - texture uv coord
    px_uv_size - texture uv size
    px_flip_x - flag, whether the output should be flipped on the x axis
    px_flip_y - flag, whether the output should be flipped on the y axis
    px_resolution - camera resolution
    px_time - time in seconds

    */

extern const char* PX_SHADER_VERTEX_BASIC;
extern const char* PX_SHADER_VERTEX_WINDOW;
extern const char* PX_SHADER_VERTEX_3D;
extern const char* PX_SHADER_VERTEX_SKYBOX;

extern const char* PX_SHADER_FRAGMENT_HEADER;
extern const char* PX_SHADER_FRAGMENT_BASIC;
extern const char* PX_SHADER_FRAGMENT_WINDOW;
extern const char* PX_SHADER_FRAGMENT_TEXT_MSDF;
extern const char* PX_SHADER_FRAGMENT_TEXT_BITMAP;
extern const char* PX_SHADER_FRAGMENT_3D;
extern const char* PX_SHADER_FRAGMENT_SKYBOX;