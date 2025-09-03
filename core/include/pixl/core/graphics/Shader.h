#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/utility/Color.h"

#define PX_ERROR_SHADER_COMPILEv 0x40
#define PX_ERROR_SHADER_COMPILEf 0x41
#define PX_ERROR_SHADER_LINK 0x42

#define PX_ERROR_SHADER_CODE_ORDER 0x43

namespace px
{
    enum class ShaderProfile
    {
        CORE,
        COMPATIBILITY,
        ES // --- !!! Currently not supported!
    };

    class ShaderImpl;

    class Shader
    {
    public:
        PX_INTERNAL Shader(CREFSTR vertexCode, CREFSTR fragmentCode);
        PX_API ~Shader();

        PX_API void Use();
        PX_API void SetFloat(CREFSTR name, float value);
        PX_API void SetMatrix4(CREFSTR name, const Mat4& mat);
        PX_API void SetVec2(CREFSTR name, const Vec2& vec);
        PX_API void SetBool(CREFSTR name, bool value);
        PX_API void SetColor(CREFSTR name, const Color& color);
        PX_API void SetInt(CREFSTR name, int value);

        PX_API static Shader* Compile(CREFSTR vertexCode, CREFSTR fragmentCode);
    private:
        ShaderImpl* m_Impl;
    };

    typedef Shader* SHADER;

    class ShaderCodeBuilder
    {
    public:
        PX_INTERNAL ShaderCodeBuilder() = default;

        PX_API static ShaderCodeBuilder New();
        PX_API static ShaderCodeBuilder NewDefault(); // Calls Version() for you with the default args

        PX_API ShaderCodeBuilder Version(ShaderProfile profile = ShaderProfile::CORE, int major = PX_OPENGL_VERSION_MAJOR, int minor = PX_OPENGL_VERSION_MINOR);
        PX_API ShaderCodeBuilder BasicVertex();
        PX_API ShaderCodeBuilder WindowVertex();
        PX_API ShaderCodeBuilder Vertex(CREFSTR code);
        PX_API ShaderCodeBuilder BasicFragment();
        PX_API ShaderCodeBuilder WindowFragment();
        PX_API ShaderCodeBuilder Fragment(CREFSTR code, bool header = true);
        PX_API std::pair<std::string, std::string> Build();
        PX_API SHADER Compile();
    private:
        int m_Steps;
        std::string m_VCode;
        std::string m_FCode;
    };
}