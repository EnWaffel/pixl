#include "pixl/core/graphics/Shader.h"
#include "pixl/core/Shaders.h"

#include <glad/glad.h>
#include <fmt/format.h>

using namespace px;

#define MIN_STEPS 4

static std::string ProfileToStr(ShaderProfile profile)
{
    switch (profile)
    {
        default:
        case ShaderProfile::CORE: return "core";
        case ShaderProfile::COMPATIBILITY: return "compatibility";
        case ShaderProfile::ES: return "es";
    }
}

static std::string VersionToStr(int major, int minor)
{
    return fmt::format("{}{}0", major, minor);
}

// ShaderCodeBuilder

ShaderCodeBuilder px::ShaderCodeBuilder::New()
{
    ShaderCodeBuilder builder;
    builder.m_Steps = 0;
    return builder;
}

ShaderCodeBuilder px::ShaderCodeBuilder::NewDefault()
{
    ShaderCodeBuilder builder;
    builder.m_Steps = 0;
    builder.Version(ShaderProfile::CORE, PX_OPENGL_VERSION_MAJOR, PX_OPENGL_VERSION_MINOR);
    return builder;
}

ShaderCodeBuilder px::ShaderCodeBuilder::Version(ShaderProfile profile, int major, int minor)
{
    m_VCode += fmt::format("#version {} {}\n", VersionToStr(major, minor), ProfileToStr(profile));
    m_FCode = m_VCode;
    return *this;
}

ShaderCodeBuilder px::ShaderCodeBuilder::BasicVertex()
{
    m_VCode += PX_SHADER_VERTEX_BASIC;
    return *this;
}

ShaderCodeBuilder px::ShaderCodeBuilder::WindowVertex()
{
    m_VCode += PX_SHADER_VERTEX_WINDOW;
    return *this;
}

ShaderCodeBuilder px::ShaderCodeBuilder::BasicFragment()
{
    m_FCode += PX_SHADER_FRAGMENT_HEADER;
    m_FCode += PX_SHADER_FRAGMENT_BASIC;
    return *this;
}

ShaderCodeBuilder px::ShaderCodeBuilder::WindowFragment()
{
    m_FCode += PX_SHADER_FRAGMENT_HEADER;
    m_FCode += PX_SHADER_FRAGMENT_WINDOW;
    return *this;
}

ShaderCodeBuilder px::ShaderCodeBuilder::Fragment(CREFSTR code, bool header)
{
    if (header) m_FCode += PX_SHADER_FRAGMENT_HEADER;
    m_FCode += code;
    return *this;
}

std::pair<std::string, std::string> px::ShaderCodeBuilder::Build()
{
    return std::make_pair(m_VCode, m_FCode);
}

SHADER px::ShaderCodeBuilder::Compile()
{
    return Shader::Compile(m_VCode, m_FCode);
}

// ShaderImpl

namespace px
{
    class ShaderImpl
    {
    private:
        GLuint m_Prog = 0;
    public:
        ShaderImpl(CREFSTR vertexCode, CREFSTR fragmentCode)
        {
            Compile(vertexCode, fragmentCode);
        }

        ~ShaderImpl()
        {
            if (m_Prog > 0) glDeleteProgram(m_Prog);
        }
        
        void Use()
        {
            glUseProgram(m_Prog);
        }

        GLuint GetLocation(CREFSTR name)
        {
            return glGetUniformLocation(m_Prog, name.c_str());
        }

        void SetFloat(CREFSTR name, float value)
        {
            glUniform1f(GetLocation(name), value);
        }

        void SetMatrix4(CREFSTR name, const Mat4& mat)
        {
            glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, mat.GetRaw());
        }

        void SetVec2(CREFSTR name, const Vec2& vec)
        {
            glUniform2f(GetLocation(name), vec.x, vec.y);
        }

        void SetBool(CREFSTR name, bool value)
        {
            glUniform1i(GetLocation(name), value);
        }

        void SetColor(CREFSTR name, const Color& color)
        {
            glUniform4f(GetLocation(name), color.r, color.g, color.b, color.a);
        }

        void SetInt(CREFSTR name, int value)
        {
            glUniform1i(GetLocation(name), value);
        }
    private:
        void Compile(CREFSTR vertexCode, CREFSTR fragmentCode)
        {
            int success;
            char infoLog[1024];
        
            const char* _vertexCode = vertexCode.c_str();
            const char* _fragmentCode = fragmentCode.c_str();
        
            GLuint vertex, fragment;
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &_vertexCode, NULL);
            glCompileShader(vertex);
        
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
                Error::Throw(PX_ERROR_SHADER_COMPILEv, infoLog);
                return;
            }
        
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &_fragmentCode, NULL);
            glCompileShader(fragment);
        
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
                Error::Throw(PX_ERROR_SHADER_COMPILEf, infoLog);
                return;
            }
        
            m_Prog = glCreateProgram();
            glAttachShader(m_Prog, vertex);
            glAttachShader(m_Prog, fragment);
            glLinkProgram(m_Prog);
        
            glGetProgramiv(m_Prog, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(m_Prog, 1024, NULL, infoLog);
                Error::Throw(PX_ERROR_SHADER_LINK, infoLog);
                return;
            }
        
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
    };
};

px::Shader::Shader(CREFSTR vertexCode, CREFSTR fragmentCode)
{
    m_Impl = new ShaderImpl(vertexCode, fragmentCode);
}

px::Shader::~Shader()
{
    delete m_Impl;
}

void px::Shader::Use()
{
    m_Impl->Use();
}

void px::Shader::SetFloat(CREFSTR name, float value)
{
    m_Impl->SetFloat(name, value);
}

void px::Shader::SetMatrix4(CREFSTR name, const Mat4& mat)
{
    m_Impl->SetMatrix4(name, mat);
}

void px::Shader::SetVec2(CREFSTR name, const Vec2& vec)
{
    m_Impl->SetVec2(name, vec);
}

void px::Shader::SetBool(CREFSTR name, bool value)
{
    m_Impl->SetBool(name, value);
}

void px::Shader::SetColor(CREFSTR name, const Color& color)
{
    m_Impl->SetColor(name, color);
}

void px::Shader::SetInt(CREFSTR name, int value)
{
    m_Impl->SetInt(name, value);
}

Shader* px::Shader::Compile(CREFSTR vertexCode, CREFSTR fragmentCode)
{
    Shader* shd = new Shader(vertexCode, fragmentCode);
    if (Error::HasError())
    {
        delete shd;
        return nullptr;
    }

    return shd;
}
