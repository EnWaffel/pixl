#include "pixl/core/tween/Easing.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace px::Easing;

EasingFunc px::Easing::StringToEasing(CREFSTR name)
{
	if (name == "QUAD_IN")
	{
		return PX_QUAD_IN;
	}
	else if (name == "QUAD_OUT")
	{
		return PX_QUAD_OUT;
	}
	else if (name == "QUAD_INOUT")
	{
		return PX_QUAD_INOUT;
	}
	else if (name == "CUBIC_IN")
	{
		return PX_CUBIC_IN;
	}
	else if (name == "CUBIC_OUT")
	{
		return PX_CUBIC_OUT;
	}
	else if (name == "CUBIC_INOUT")
	{
		return PX_CUBIC_INOUT;
	}
	else if (name == "QUART_IN")
	{
		return PX_QUART_IN;
	}
	else if (name == "QUART_OUT")
	{
		return PX_QUART_OUT;
	}
	else if (name == "QUART_INOUT")
	{
		return PX_QUART_INOUT;
	}
	else if (name == "QUINT_IN")
	{
		return PX_QUINT_IN;
	}
	else if (name == "QUINT_OUT")
	{
		return PX_QUINT_OUT;
	}
	else if (name == "QUINT_INOUT")
	{
		return PX_QUINT_INOUT;
	}
	else if (name == "SIN_IN")
	{
		return PX_SIN_IN;
	}
	else if (name == "SIN_OUT")
	{
		return PX_SIN_OUT;
	}
	else if (name == "SIN_INOUT")
	{
		return PX_SIN_INOUT;
	}
	else if (name == "EXPO_IN")
	{
		return PX_EXPO_IN;
	}
	else if (name == "EXPO_OUT")
	{
		return PX_EXPO_OUT;
	}
	else if (name == "EXPO_INOUT")
	{
		return PX_EXPO_INOUT;
	}
	else if (name == "CIRC_IN")
	{
		return PX_CIRC_IN;
	}
	else if (name == "CIRC_OUT")
	{
		return PX_CIRC_OUT;
	}
	else if (name == "CIRC_INOUT")
	{
		return PX_CIRC_INOUT;
	}
	else if (name == "BOUNCE_IN")
	{
		return PX_BOUNCE_IN;
	}
	else if (name == "BOUNCE_OUT")
	{
		return PX_BOUNCE_OUT;
	}
	else if (name == "BOUNCE_INOUT")
	{
		return PX_BOUNCE_INOUT;
	}
	else if (name == "ELASTIC_IN")
	{
		return PX_ELASTIC_IN;
	}
	else if (name == "ELASTIC_OUT")
	{
		return PX_ELASTIC_OUT;
	}
	else if (name == "ELASTIC_INOUT")
	{
		return PX_ELASTIC_INOUT;
	}
	else if (name == "BACK_IN")
	{
		return PX_BACK_IN;
	}
	else if (name == "BACK_OUT")
	{
		return PX_BACK_OUT;
	}
	else if (name == "BACK_INOUT")
	{
		return PX_BACK_INOUT;
	}
	else
	{
		return PX_LINEAR;
	}
}

float px::Easing::_linear(float x)
{
	return x;
}

float px::Easing::_quadraticIn(float x)
{
	return x * x;
}

float px::Easing::_quadraticOut(float x)
{
	return 1 - (1 - x) * (1 - x);
}

float px::Easing::_quadraticInOut(float x)
{
	return x < 0.5 ? 2 * x * x : 1 - powf(-2 * x + 2, 2) / 2;
}

float px::Easing::_cubicIn(float x)
{
	return x * x * x;
}

float px::Easing::_cubicOut(float x)
{
	return 1 - powf(1 - x, 3);
}

float px::Easing::_cubicInOut(float x)
{
	return x < 0.5 ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2;
}

float px::Easing::_quartIn(float x)
{
	return x * x * x * x;
}

float px::Easing::_quartOut(float x)
{
	return 1 - powf(1 - x, 4);
}

float px::Easing::_quartInOut(float x)
{
	return x < 0.5 ? 8 * x * x * x * x : 1 - powf(-2 * x + 2, 4) / 2;
}

float px::Easing::_quintIn(float x)
{
	return x * x * x * x * x;
}

float px::Easing::_quintOut(float x)
{
	return 1 - powf(1 - x, 5);
}

float px::Easing::_quintInOut(float x)
{
	return x < 0.5 ? 16 * x * x * x * x * x : 1 - powf(-2 * x + 2, 5) / 2;
}

float px::Easing::_sinIn(float x)
{
	return 1 - cos((x * M_PI) / 2);
}

float px::Easing::_sinOut(float x)
{
	return sin((x * M_PI) / 2);
}

float px::Easing::_sinInOut(float x)
{
	return -(cos(M_PI * x) - 1) / 2;
}

float px::Easing::_expoIn(float x)
{
	return x == 0 ? 0 : powf(2, 10 * x - 10);
}

float px::Easing::_expoOut(float x)
{
	return x == 1 ? 1 : 1 - powf(2, -10 * x);
}

float px::Easing::_expoInOut(float x)
{
	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5 ? powf(2, 20 * x - 10) / 2
		: (2 - powf(2, -20 * x + 10)) / 2;
}

float px::Easing::_circIn(float x)
{
	return 1 - sqrt(1 - powf(x, 2));
}

float px::Easing::_circOut(float x)
{
	return sqrt(1 - powf(x - 1, 2));
}

float px::Easing::_circInOut(float x)
{
	return x < 0.5
		? (1 - sqrt(1 - powf(2 * x, 2))) / 2
		: (sqrt(1 - powf(-2 * x + 2, 2)) + 1) / 2;
}

float px::Easing::_bounceIn(float x)
{
	return 1 - _bounceOut(1 - x);
}

float px::Easing::_bounceOut(float x)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

float px::Easing::_bounceInOut(float x)
{
	return x < 0.5
		? (1 - _bounceOut(1 - 2 * x)) / 2
		: (1 + _bounceOut(2 * x - 1)) / 2;
}

float px::Easing::_elasticIn(float x)
{
	const float c4 = (2 * M_PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: -powf(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

float px::Easing::_elasticOut(float x)
{
	const float c4 = (2 * M_PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: powf(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

float px::Easing::_elasticInOut(float x)
{
	const float c5 = (2 * M_PI) / 4.5;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(powf(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
		: (powf(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}

float px::Easing::_backIn(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

float px::Easing::_backOut(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return 1 + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
}

float px::Easing::_backInOut(float x)
{
	const float c1 = 1.70158;
	const float c2 = c1 * 1.525;

	return x < 0.5
		? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}
