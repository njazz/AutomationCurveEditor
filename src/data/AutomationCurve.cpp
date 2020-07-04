#include "AutomationCurve.hpp"

#include <algorithm>

namespace AutomationCurve {

void ACurve::_ClampTime(float& t)
{
    if ((t >= 0.0) && (t <= 1))
        return;

    // hold
    if (_cycleType == CycleType::Hold) {
        if (t > 1)
            t = 1;
        if (t < 0)
            t = 0;
        return;
    }

    // zero
    if (_cycleType == CycleType::Zero) {
        t = 0;
        return;
    }

    // TODO: mirror, repeat
}

void ACurve::_ClampValue(float& v)
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
    auto idx = LeftPointIndexAtFraction(fract);
    if (idx < 0)
        idx = 0;

    auto value_ = value;
    _ClampValue(value_);

    auto fract_ = fract;
    _ClampTime(fract_);

    auto tf = EaseFunctorFactory::Create(_defaultTransitionType);

    printf("idx: %i (%lu)\n", idx, Size());
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

size_t ACurve::Size() { return _pointPositions.size(); }

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

const int32_t ACurve::LeftPointIndexAtFraction(const float& f)
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

const int32_t ACurve::RightPointIndexAtFraction(const float& f)
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

std::vector<float> ACurve::RawValues() { return _pointValues; }
std::vector<float> ACurve::RawPositions() { return _pointPositions; }

const float ACurve::TimeAt(const size_t& idx) { return _pointPositions[idx]; }
const float ACurve::ValueAt(const size_t& idx) { return _pointValues[idx]; }

const LockEdit ACurve::LockAt(const size_t& idx) { return _pointLock[idx]; }

// main:
float ACurve::ValueAtFraction(const float& f)
{
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
    _ClampTime(value);

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
const std::vector<float> ACurve::AsFloatVector(const size_t res)
{
    std::vector<float> ret;

    for (size_t i = 0; i < res; i++) {
        ret.push_back(ValueAtFraction(float(i) / float(res)));
    }
    return ret;
} // resolution

const ACurve ACurve::AsLineApproximation(const size_t res)
{
    ACurve ret;

    for (size_t i = 0; i < res; i++) {
        float fract = float(i) / float(res);
        ret.AddPoint(fract, ValueAtFraction(fract));
    }
    return ret;
}

const std::vector<std::pair<float, float> > ACurve::AsPoints(const size_t res)
{
    std::vector<std::pair<float, float> > ret;

    for (size_t i = 0; i < res; i++) {
        std::pair<float, float> v;
        float fract = float(i) / float(res);
        v.first = fract;
        v.second = ValueAtFraction(fract);
        ret.push_back(v);
    }
    return ret;
}

const ACurve ACurve::AsApproximation(const size_t numsteps)
{
    ACurve ret;

    float t_start = 0;
    for (int i = 1; i < Size(); i++) {
        float t_end = TimeAt(i);
        for (int j = 0; j < numsteps; j++) {
            float fract = float(j) / float(numsteps);
            float tv = (1 - fract) * t_start + fract * t_end;
            auto value = ValueAtFraction(tv);
            ret.AddPoint(fract, value);
        }
        t_start = t_end;
    }

    return ret;
}

// ---
// edits
void ACurve::SelectPoint(const size_t idx)
{
    if (!IsSelected(idx))
        _selectionIdx.push_back(idx);
}
void ACurve::DeselectPoint(const size_t idx)
{
    if (std::find(_selectionIdx.begin(), _selectionIdx.end(), idx) == _selectionIdx.end())
        return;
    _selectionIdx.erase(std::remove(_selectionIdx.begin(), _selectionIdx.end(), idx));
}
void ACurve::ClearSelection()
{
    _selectionIdx.clear();
}

void ACurve::SelectAll()
{
    _selectionIdx.clear();
    for (int i = 0; i < Size(); i++)
        _selectionIdx.push_back(i);
}

bool ACurve::IsSelected(const size_t idx)
{
    return std::find(_selectionIdx.begin(), _selectionIdx.end(), idx) != _selectionIdx.end();
}

void ACurve::MoveSelectionValue(const float& v)
{
    for (const auto& idx : _selectionIdx)
        SetValue(idx, ValueAt(idx) + v);
}
void ACurve::MoveSelectionTime(const float& v)
{
    for (const auto& idx : _selectionIdx)
        SetTime(idx, TimeAt(idx) + v);
}

void ACurve::SetSelectionTransitions(const std::string& t)
{
    for (const auto& idx : _selectionIdx)
        SetTransitionToPoint(idx, t);
}

void ACurve::SetSelectionLocks(const LockEdit& l)
{
    for (const auto& idx : _selectionIdx)
        SetPointLock(idx, l);
}

void ACurve::DeleteSelection()
{
    // TODO: clean
    for (int i = (Size() - 1); i >= 0; i--) {
        if (IsSelected(i))
            RemovePointAt(i);
    }

    ClearSelection();
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

}; // namespace //
