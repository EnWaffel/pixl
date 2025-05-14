#include "pixl/core/graphics/Shader.h"

#include <glad/glad.h>

using namespace px;

static const char* __pixl_vertex = R"(
#version 330 core

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

static const char* __pixl_fragment_header = R"(
#version 330 core

in vec2 px_uv;

uniform sampler2D px_texture;
uniform vec4 px_color;
uniform vec2 px_uv_coord;
uniform vec2 px_uv_size;
uniform bool px_flip_x;
uniform bool px_flip_y;
uniform vec2 px_resolution;
uniform float px_time;
)";

namespace px
{
    class ShaderImpl
    {
    private:
        GLuint m_Prog = 0;
    public:
        ShaderImpl(CREFSTR vertexCode, CREFSTR fragmentCode, bool header)
        {
            std::string frag = fragmentCode;
            if (header)
            {
                frag = __pixl_fragment_header + fragmentCode;
            }

            Compile(vertexCode, frag);
        }

        ShaderImpl(CREFSTR fragmentCode, bool header)
        {
            std::string frag = fragmentCode;
            if (header)
            {
                frag = __pixl_fragment_header + fragmentCode;
            }

            Compile(__pixl_vertex, frag);
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

px::Shader::Shader(CREFSTR vertexCode, CREFSTR fragmentCode, bool header)
{
    m_Impl = new ShaderImpl(vertexCode, fragmentCode, header);
}

px::Shader::Shader(CREFSTR fragmentCode, bool header)
{
    m_Impl = new ShaderImpl(fragmentCode, header);
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
