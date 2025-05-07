#pragma once

#include "pixl/utility/Defs.h"
#include "Vec2.h"
#include "Vec3.h"

#include <array>

namespace px
{
    class Mat4
	{
	private:
		std::array<float, 16> data;
	public:
		PX_API Mat4();
		PX_API Mat4(std::array<float, 16>& arr);
		PX_API ~Mat4();

		PX_API void Translate(const Vec2& vec);
		PX_API void Scale(const Vec2& vec);
		PX_API void Translate(const Vec3& vec);
		PX_API void Scale(const Vec3& vec);
		PX_API void Rotate(const Vec3& degrees = Vec3(0.0f, 0.0f, 0.0f)); // Z-Axis (last value) for 2D

		float& operator()(int col, int row);
		const float& operator()(int col, int row) const;

		PX_API Mat4 operator*(const Mat4& other);
		PX_API const float* GetRaw() const;
	public:
		PX_API static Mat4 Ortho(float left, float right, float bottom, float top);
		PX_API static Mat4 Perspective(float fov, float aspect, float near, float far);
		PX_API static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
	};
}