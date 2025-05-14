#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/utility/Color.h"

#define PX_ERROR_SHADER_COMPILEv 0x40
#define PX_ERROR_SHADER_COMPILEf 0x41
#define PX_ERROR_SHADER_LINK 0x42

namespace px
{
    /*
    Predefined uniforms and vars:

    px_texture - input texture
    px_color - blend color
    px_uv_coord - texture uv coord
    px_uv_size - texture uv size
    px_flip_x - flag, whether the output should be flipped on the x axis
    px_flip_y - flag, whether the output should be flipped on the y axis
    px_resolution - camera resolution
    px_time - time in seconds

    */

    class ShaderImpl;

    class Shader
    {
    public:
        PX_INTERNAL Shader(CREFSTR vertexCode, CREFSTR fragmentCode, bool header = true);
        PX_API Shader(CREFSTR fragmentCode, bool header = true);
        PX_API ~Shader();

        PX_API void Use();
        PX_API void SetFloat(CREFSTR name, float value);
        PX_API void SetMatrix4(CREFSTR name, const Mat4& mat);
        PX_API void SetVec2(CREFSTR name, const Vec2& vec);
        PX_API void SetBool(CREFSTR name, bool value);
        PX_API void SetColor(CREFSTR name, const Color& color);
    private:
        ShaderImpl* m_Impl;
    };

    typedef Shader* SHADER;
}