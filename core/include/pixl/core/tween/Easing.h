#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

#include <functional>

namespace px
{
    namespace Easing
    {
        typedef std::function<float(float)> EasingFunc;

        PX_API EasingFunc StringToEasing(CREFSTR name);

        PX_API float _linear(float x);
#define PX_LINEAR px::Easing::_linear

        PX_API float _quadraticIn(float x);
#define PX_QUAD_IN px::Easing::_quadraticIn

        PX_API float _quadraticOut(float x);
#define PX_QUAD_OUT px::Easing::_quadraticOut

        PX_API float _quadraticInOut(float x);
#define PX_QUAD_INOUT px::Easing::_quadraticInOut

        PX_API float _cubicIn(float x);
#define PX_CUBIC_IN px::Easing::_cubicIn

        PX_API float _cubicOut(float x);
#define PX_CUBIC_OUT px::Easing::_cubicOut

        PX_API float _cubicInOut(float x);
#define PX_CUBIC_INOUT px::Easing::_cubicInOut

        PX_API float _quartIn(float x);
#define PX_QUART_IN px::Easing::_quartIn

        PX_API float _quartOut(float x);
#define PX_QUART_OUT px::Easing::_quartOut

        PX_API float _quartInOut(float x);
#define PX_QUART_INOUT px::Easing::_quartInOut

        PX_API float _quintIn(float x);
#define PX_QUINT_IN px::Easing::_quintIn

        PX_API float _quintOut(float x);
#define PX_QUINT_OUT px::Easing::_quintOut

        PX_API float _quintInOut(float x);
#define PX_QUINT_INOUT px::Easing::_quintInOut

        PX_API float _sinIn(float x);
#define PX_SIN_IN px::Easing::_sinIn

        PX_API float _sinOut(float x);
#define PX_SIN_OUT px::Easing::_sinOut

        PX_API float _sinInOut(float x);
#define PX_SIN_INOUT px::Easing::_sinInOut

        PX_API float _expoIn(float x);
#define PX_EXPO_IN px::Easing::_expoIn

        PX_API float _expoOut(float x);
#define PX_EXPO_OUT px::Easing::_expoOut

        PX_API float _expoInOut(float x);
#define PX_EXPO_INOUT px::Easing::_expoInOut

        PX_API float _circIn(float x);
#define PX_CIRC_IN px::Easing::_circIn

        PX_API float _circOut(float x);
#define PX_CIRC_OUT px::Easing::_circOut

        PX_API float _circInOut(float x);
#define PX_CIRC_INOUT px::Easing::_circInOut

        PX_API float _bounceIn(float x);
#define PX_BOUNCE_IN px::Easing::_bounceIn

        PX_API float _bounceOut(float x);
#define PX_BOUNCE_OUT px::Easing::_bounceOut

        PX_API float _bounceInOut(float x);
#define PX_BOUNCE_INOUT px::Easing::_bounceInOut

        PX_API float _elasticIn(float x);
#define PX_ELASTIC_IN px::Easing::_elasticIn

        PX_API float _elasticOut(float x);
#define PX_ELASTIC_OUT px::Easing::_elasticOut

        PX_API float _elasticInOut(float x);
#define PX_ELASTIC_INOUT px::Easing::_elasticInOut

        PX_API float _backIn(float x);
#define PX_BACK_IN px::Easing::_backIn

        PX_API float _backOut(float x);
#define PX_BACK_OUT px::Easing::_backOut

        PX_API float _backInOut(float x);
#define PX_BACK_INOUT px::Easing::_backInOut
    }
}