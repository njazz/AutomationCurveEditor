#include "AutomationCurve.hpp"

#include <algorithm>

#include "math.h"

namespace AutomationCurve {

void ACurve::_ClampTime(float& t) const
{
    if ((t >= 0.0) && (t <= 1))
        return;

    if (t > 1) {
        // hold
        if (_cycleRight == CycleType::Hold) {
            //            t = 1;
        }

        if (_cycleRight == CycleType::Zero) {
            //            t = ACurve::TimeZero();
        }

        if (_cycleRight == CycleType::Repeat) {
            //            t = fmodf(t, 1);
        }

        // todo: mirror
    }

    if (t < 0) {
        if (_cycleLeft == CycleType::Hold) {
            //            t = 0;
        }

        if (_cycleLeft == CycleType::Zero) {
            //            t = ACurve::TimeZero();
        }

        if (_cycleLeft == CycleType::Repeat) {
            //            t = 1 - fmodf(-t, 1);
        }

        // todo: mirror
    }
}

void ACurve::_ClampValue(float& v) const
{
    if (v > 1) {
        v = 1;
        return;
    }

    if ((v < 0) && (_valueRange == CurveValueRange::Positive)) {
        v = 0;
        return;
    }

    if (v < -1)
        v = -1;
}

// ---

void ACurve::AddPoint(const float& fract, const float& value)
{
    // todo: change
    auto fract_ = fract;
    _ClampTime(fract_);
    if (fract_ == ACurve::TimeZero())
        return;

    auto idx = LeftPointIndexAtFraction(fract_);
    if (idx < 0)
        idx = 0;

    auto value_ = value;
    _ClampValue(value_);

    auto tf = EaseFunctorFactory::Create(_defaultTransitionType);

    //    printf("idx: %i (%lu)\n", idx, Size());
    if (!Size()) {

        _pointPositions.push_back(fract_);
        _pointValues.push_back(value_);
        _pointLock.push_back(LockEdit::None);
        _transitionToPoint.push_back(tf);
        _typenameOfTransitionToPoint.push_back(_defaultTransitionType);

    } else {

        _pointPositions.insert(_pointPositions.begin() + idx + 1, fract_);
        _pointValues.insert(_pointValues.begin() + idx + 1, value_);
        _pointLock.insert(_pointLock.begin() + idx + 1, LockEdit::None);
        _transitionToPoint.insert(_transitionToPoint.begin() + idx + 1, tf);
        _typenameOfTransitionToPoint.insert(_typenameOfTransitionToPoint.begin() + idx + 1, _defaultTransitionType);
    }
}

void ACurve::RemovePointAt(const size_t& idx)
{
    if (idx == 0)
        return;
    if (idx == (Size() - 1))
        return;
    _pointPositions.erase(_pointPositions.begin() + idx);
    _pointValues.erase(_pointValues.begin() + idx);
    _pointLock.erase(_pointLock.begin() + idx);
    _transitionToPoint.erase(_transitionToPoint.begin() + idx);
    _typenameOfTransitionToPoint.erase(_typenameOfTransitionToPoint.begin() + idx);
}

void ACurve::Clear()
{
    _pointPositions.clear();
    _pointValues.clear();

    _transitionToPoint.clear();
    _typenameOfTransitionToPoint.clear();

    _splineControlPoints.clear();
}

void ACurve::InitConstant(const float& c)
{
    Clear();

    // resets to two points at 0 and 1

    _valueRange = CurveValueRange::Positive;
    AddPoint(0, c);
    AddPoint(1, c);
    SetPointLock(0, LockEdit::LockX);
    SetPointLock(1, LockEdit::LockX);

} // const value
void ACurve::InitTF()
{
    Clear();

    // resets to two points at -1 and 1

    _valueRange = CurveValueRange::Full;
    AddPoint(0, -1);
    AddPoint(1, 1);
    SetPointLock(0, LockEdit::LockX);
    SetPointLock(1, LockEdit::LockX);

} // -1..1

size_t ACurve::Size() const { return _pointPositions.size(); }

void ACurve::SetCycleLeft(const CycleType& v)
{
    _cycleLeft = v;
}
void ACurve::SetCycleRight(const CycleType& v)
{
    _cycleRight = v;
}

void ACurve::SetPointLock(const size_t& idx, const LockEdit& l)
{
    if (idx >= Size())
        return;
    _pointLock[idx] = l;
}

void ACurve::SetTransitionToPoint(const size_t& idx, const std::string& f)
{
    if (idx >= Size())
        return;

    _transitionToPoint[idx] = EaseFunctorFactory::Create(f);
    _typenameOfTransitionToPoint[idx] = f;
}
void ACurve::SetAllTransitions(const std::string& f)
{
    for (auto& e : _transitionToPoint) {
        e = EaseFunctorFactory::Create(f);
    }
    for (auto& e : _typenameOfTransitionToPoint)
        e = f;
}

std::string ACurve::TransitionAt(const size_t& idx)
{
    if (idx >= Size())
        return "";
    return _typenameOfTransitionToPoint[idx];
}

const int32_t ACurve::LeftPointIndexAtFraction(const float& f) const
{
    // array bounds
    if (Size() < 2)
        return -1;

    for (int i = 0; i < (_pointPositions.size() - 1); i++) {
        if ((f >= _pointPositions[i]) && (f < _pointPositions[i + 1]))
            return i;
    }

    if (f > _pointPositions[Size() - 1])
        return Size() - 1;

    return -1;

} // -1 for not found;

const int32_t ACurve::RightPointIndexAtFraction(const float& f) const
{
    auto l = LeftPointIndexAtFraction(f);
    if (l == -1)
        return -1;

    if ((l + 1) >= Size())
        return -1;

    return l + 1;

} // -1 for not found;

const FloatRange ACurve::TimeRangeForPoint(const size_t& idx)
{
    FloatRange ret;

    // >size
    if (idx >= Size())
        return ret;

    // <2
    if (Size() < 2)
        return ret;

    if (idx == 0) {
        ret.min = 0;
        ret.max = _pointPositions[1];
    } else if (idx == (Size() - 1)) {
        ret.min = 1;
        ret.max = 1;
    } else {
        ret.min = _pointPositions[idx];
        ret.max = _pointPositions[idx + 1];
    }

    return ret;
}

std::vector<float> ACurve::RawValues() const { return _pointValues; }
std::vector<float> ACurve::RawPositions() const { return _pointPositions; }

void ACurve::SetRawPoints(const std::vector<float>& pos, const std::vector<float>& v, std::vector<LockEdit> locks)
{
    _pointPositions = pos;
    _pointValues = v;
    _pointLock = locks;

    _transitionToPoint.clear();
    _typenameOfTransitionToPoint.clear();

    _transitionToPoint.resize(pos.size());
    _typenameOfTransitionToPoint.resize(pos.size());
}

std::vector<std::pair<float, float> > ACurve::RawPoints()
{
    std::vector<std::pair<float, float> > ret;
    ret.resize(_pointValues.size());
    for (int i = 0; i < _pointValues.size(); i++) {
        std::pair<float, float> p;
        p.second = _pointValues[i];
        p.first = _pointPositions[i];
        ret[i] = p;
    }
    return ret;
}

const float ACurve::TimeAt(const size_t& idx) const { return _pointPositions.at(idx); }
const float ACurve::ValueAt(const size_t& idx) const { return _pointValues.at(idx); }

const LockEdit ACurve::LockAt(const size_t& idx) const { return _pointLock.at(idx); }

// main:
float ACurve::ValueAtFraction(const float& f0) const
{
    float f = f0;
    _ClampTime(f);
    if (f == ACurve::TimeZero())
        return 0;

    auto lp = LeftPointIndexAtFraction(f);
    auto rp = RightPointIndexAtFraction(f);

    // TODO: check -1
    if (lp < 0)
        lp = 0;
    if (rp < 0)
        rp = Size() - 1;

    auto lt = (lp == -1) ? 0 : TimeAt(lp);
    auto rt = (rp == -1) ? 1 : TimeAt(rp);

    auto timeRange = rt - lt;
    auto interp = (timeRange) ? (f - lt) / timeRange : 0;

    //    return (interp<.5)?lt/Size():rt/Size();

    float vOffset = ValueAt(lp);
    float vRange = ValueAt(rp) - vOffset;

    return _transitionToPoint[rp](interp) * vRange + vOffset; //(interp<.5) ? ValueAt(lp) : ValueAt(rp);
}

//
Optional<float> ACurve::ValueAtScaledTime(const float& t) const
{
    if (t < _timeOffset)
        return Optional<float>();
    if (t > (_timeOffset + _timeScale))
        return Optional<float>();

    float s_t = (t - _timeOffset) / ((_timeScale) ? (_timeScale) : 0.000001);
    return Optional<float>(ValueAtFraction(s_t));
}

//
void ACurve::SetValue(const size_t& idx, const float& v)
{
    if (_pointLock[idx] == LockEdit::LockY)
        return;
    if (_pointLock[idx] == LockEdit::LockBoth)
        return;

    if (idx >= Size())
        return;
    auto value = v;
    _ClampValue(value);
    _pointValues[idx] = value;
}

void ACurve::SetTime(const size_t& idx, const float& t)
{
    if (_pointLock[idx] == LockEdit::LockX)
        return;
    if (_pointLock[idx] == LockEdit::LockBoth)
        return;

    if (idx >= Size())
        return;
    auto value = t;
    // todo: remove/change?
    _ClampTime(value);

    if (value == ACurve::TimeZero())
        return;

    auto idx2 = idx + 1;
    if (idx2 < Size())
        if (t >= _pointPositions[idx2])
            return;

    if (idx > 0)
        if (t < _pointPositions[idx - 1])
            return;

    _pointPositions[idx] = value;
}

// ---

void ACurve::SetScaledValue(const size_t& idx, const float& v)
{
    SetValue(idx, _convertToNormalised(v));
}
void ACurve::SetScaledTime(const size_t& idx, const float& t)
{
    SetTime(idx, (_timeScale == 0) ? 0 : (t - _timeOffset) / _timeScale);
}

float ACurve::ScaledTimeAt(const size_t& idx)
{
    return TimeAt(idx) * _timeScale + _timeOffset;
}

float ACurve::ScaledValueAt(const size_t& idx)
{
    return _convertFromNormalised(ValueAt(idx));
}

void ACurve::SetValueConverters(const ConverterFn& converterFrom, const ConverterFn& converterTo)
{
    _convertToNormalised = converterFrom;
    _convertFromNormalised = converterTo;
}

// ---

void MultiCurve::SetHidden(const std::string& name, const bool& v)
{
    _isHidden[name] = v;
}
bool MultiCurve::IsHidden(const std::string& name)
{
    if (_isHidden.find(name) == _isHidden.end())
        return false;
    return _isHidden[name];
}

MultiCurve::CurveColor MultiCurve::GetColor(const std::string& name)
{
    if (_colorForCurve.find(name) == _colorForCurve.end())
        return defaultColor;
    return _colorForCurve[name];
}
void MultiCurve::SetColor(const std::string& name, const CurveColor& c)
{
    _colorForCurve[name] = c;
}

//
float MultiCurve::GetMinTimeOffset()
{
    float mt = INFINITY;
    for (auto& c : curves) {
        if (c.second->TimeOffset() < mt)
            mt = c.second->TimeOffset();
    }
    return mt;
}

float MultiCurve::GetMaxTimeScale()
{
    float ms = 0;
    for (auto& c : curves) {
        if ((c.second->TimeScale() + c.second->TimeOffset()) > ms)
            ms = c.second->TimeScale() + c.second->TimeOffset();
    }
    return ms - GetMinTimeOffset();
}

// ---
float WidgetCoordinateConverter::FractionCurveToGlobal(const float& f){
    auto t = curveTimeOffset + f * curveTimeScale;
    auto ret = t / (timeScale - timeOffset);
    printf("%f %f\n",t,ret);
    return ret;
}
float WidgetCoordinateConverter::FractionGlobalToCurve(const float& f){
    auto t = f  * (timeScale - timeOffset);
     auto ret =  (t- curveTimeOffset) / curveTimeScale ;
     printf("%f %f\n",t,ret);
    return ret;
    
}

// ---

float WidgetCoordinateConverter::PixelToCurveFraction(const float& px)
{
    auto glF = px / widgetWidth;
    return FractionGlobalToCurve(glF);
}

float WidgetCoordinateConverter::PixelToSeconds(const float& px)
{

    return px / widgetWidth * WidthInSeconds() + timeOffset;
}

float WidgetCoordinateConverter::SecondsToPixel(const float& sec)
{
    return (sec-timeOffset) / WidthInSeconds() * widgetWidth;
}

float WidgetCoordinateConverter::SecondsToCurveFraction(const float& sec)
{
    auto glF = (sec  ) / timeScale - timeOffset;
    return FractionGlobalToCurve(glF);
}

float WidgetCoordinateConverter::CurveFractionToSeconds(const float& fr)
{
    auto glF = FractionCurveToGlobal(fr);
    return (glF+timeOffset) * timeScale;
}

float WidgetCoordinateConverter::CurveFractionToPixel(const float& fr)
{
    auto glF = FractionCurveToGlobal(fr);
    return glF * widgetWidth;
}

}; // namespace //
