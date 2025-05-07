#include "pixl/core/math/Mat4.h"
#include "pixl/core/math/MathFunctions.h"

#include <math.h>

using namespace px;

px::Mat4::Mat4()
{
	data = { 1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1 };
}

px::Mat4::Mat4(std::array<float, 16>& arr) : data(arr)
{
}

px::Mat4::~Mat4()
{
}

float& px::Mat4::operator()(int col, int row) {
	return data[col * 4 + row];
}

const float& px::Mat4::operator()(int col, int row) const {
	return data[col * 4 + row];
}

void px::Mat4::Translate(const Vec3& vec)
{
	Mat4 translationMatrix;
	translationMatrix(3, 0) = vec.x;
	translationMatrix(3, 1) = vec.y;
	translationMatrix(3, 2) = vec.z;
	
	operator*(translationMatrix).data = data;
}

void px::Mat4::Scale(const Vec3& vec)
{
	Mat4 scaleMatrix;
	scaleMatrix(0, 0) = vec.x;
	scaleMatrix(1, 1) = vec.y;
	scaleMatrix(2, 2) = vec.z;

	operator*(scaleMatrix).data = data;
}

void px::Mat4::Translate(const Vec2& vec)
{
	Mat4 translationMatrix;
	translationMatrix(3, 0) = vec.x;
	translationMatrix(3, 1) = vec.y;

	operator*(translationMatrix).data = data;
}

void px::Mat4::Scale(const Vec2& vec)
{
	Mat4 scaleMatrix;
	scaleMatrix(0, 0) = vec.x;
	scaleMatrix(1, 1) = vec.y;

	operator*(scaleMatrix).data = data;
}

void px::Mat4::Rotate(const Vec3& degrees)
{
	float angleX = Math::ToRadians(degrees.x);
	float angleY = Math::ToRadians(degrees.y);
	float angleZ = Math::ToRadians(degrees.z);

	Mat4 rotationX;
	float cosX = cos(angleX);
	float sinX = sin(angleX);
	rotationX(1, 1) = cosX;
	rotationX(1, 2) = sinX;
	rotationX(2, 1) = -sinX;
	rotationX(2, 2) = cosX;

	Mat4 rotationY;
	float cosY = cos(angleY);
	float sinY = sin(angleY);
	rotationY(0, 0) = cosY;
	rotationY(0, 2) = -sinY;
	rotationY(2, 0) = sinY;
	rotationY(2, 2) = cosY;

	Mat4 rotationZ;
	float cosZ = cos(angleZ);
	float sinZ = sin(angleZ);
	rotationZ(0, 0) = cosZ;
	rotationZ(0, 1) = sinZ;
	rotationZ(1, 0) = -sinZ;
	rotationZ(1, 1) = cosZ;

	data = operator*(rotationZ).operator*(rotationY).operator*(rotationX).data;
}

Mat4 px::Mat4::operator*(const Mat4& other)
{
	Mat4 result;

	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			result(col, row) = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result(col, row) += (*this)(k, row) * other(col, k);
			}
		}
	}

	*this = result;
	return *this;
}

const float* px::Mat4::GetRaw() const
{
	return data.data();
}

Mat4 px::Mat4::Ortho(float left, float right, float bottom, float top)
{
	Mat4 orthoMatrix;

	orthoMatrix(0, 0) = 2.0f / (right - left);
	orthoMatrix(1, 1) = 2.0f / (top - bottom);
	orthoMatrix(2, 2) = -2.0f / (1.0 - -1.0);
	orthoMatrix(3, 0) = -(right + left) / (right - left);
	orthoMatrix(3, 1) = -(top + bottom) / (top - bottom);
	orthoMatrix(3, 2) = -(1.0 + -1.0) / (1.0 - -1.0f);

	return orthoMatrix;
}

Mat4 px::Mat4::Perspective(float fov, float aspect, float near, float far)
{
	float fovRad = Math::ToRadians(fov);
	float tanHalfFov = tanf(fovRad / 2.0f);

	Mat4 perspectiveMatrix;
	perspectiveMatrix(0, 0) = 1.0f / (aspect * tanHalfFov);
	perspectiveMatrix(1, 1) = 1.0f / tanHalfFov;
	perspectiveMatrix(2, 2) = -(far + near) / (far - near);
	perspectiveMatrix(2, 3) = -1.0f;
	perspectiveMatrix(3, 2) = -(2.0f * far * near) / (far - near);
	perspectiveMatrix(3, 3) = 0.0f;

	return perspectiveMatrix;
}

Mat4 px::Mat4::LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
	Vec3 zAxis = eye;
	zAxis -= center;
	zAxis = zAxis.Normalize();
	Vec3 xAxis = up.Cross(zAxis).Normalize();
	Vec3 yAxis = zAxis.Cross(xAxis);

	Mat4 lookAtMatrix;
	lookAtMatrix(0, 0) = xAxis.x;
	lookAtMatrix(0, 1) = xAxis.y;
	lookAtMatrix(0, 2) = xAxis.z;
	lookAtMatrix(1, 0) = yAxis.x;
	lookAtMatrix(1, 1) = yAxis.y;
	lookAtMatrix(1, 2) = yAxis.z;
	lookAtMatrix(2, 0) = zAxis.x;
	lookAtMatrix(2, 1) = zAxis.y;
	lookAtMatrix(2, 2) = zAxis.z;
	lookAtMatrix(3, 0) = -xAxis.Dot(eye);
	lookAtMatrix(3, 1) = -yAxis.Dot(eye);
	lookAtMatrix(3, 2) = -zAxis.Dot(eye);

	return lookAtMatrix;
}