
#include "FVector.hpp"

#include <algorithm>

#include "math.h"
#include <utility>

namespace AutomationCurve {

void FVector::_ClampTime(float& t) const
{
    if ((t >= 0.0) && (t <= 1))
        return;

    if (t > 1) {
        // hold
        if (_cycleRight == CycleType::Hold) {
//            t = 1;
        }

        if (_cycleRight == CycleType::Zero) {
//            t = FVector::TimeZero();
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
//            t = FVector::TimeZero();
        }

        if (_cycleLeft == CycleType::Repeat) {
//            t = 1 - fmodf(-t, 1);
        }

        // todo: mirror
    }
}

//void FVector::_ClampValue(float& v) const
//{
//    if (v > 1) {
//        v = 1;
//        return;
//    }
//
//    if ((v < 0) && (_valueRange == CurveValueRange::Positive)) {
//        v = 0;
//        return;
//    }
//
//    if (v < -1)
//        v = -1;
//}

// ---

void FVector::InsertValueAt(const size_t& idx, const float& fract){
    // TODO
    _pointPositions.insert(_pointPositions.begin() + idx , fract);
        _pointLock.insert(_pointLock.begin() + idx , false);
    
    _unsorted = true;
}

void FVector::AddValue(const float& fract)//, const float& value)
{
    // todo: change
    auto fract_ = fract;
//    _ClampTime(fract_);
//    if (fract_ == FVector::TimeZero())
//        return;
    
    auto idx = LeftPointIndexAtFraction(fract_);
    if (idx < 0)
        idx = 0;

//    auto value_ = value;
//    _ClampValue(value_);

    

    

//    auto tf = EaseFunctorFactory::Create(_defaultTransitionType);

//    printf("idx: %i (%lu)\n", idx, Size());
    if (!Size()) {

        _pointPositions.push_back(fract_);
//        _pointValues.push_back(value_);
        _pointLock.push_back(false);
//        _transitionToPoint.push_back(tf);
//        _typenameOfTransitionToPoint.push_back(_defaultTransitionType);

    } else {

        _pointPositions.insert(_pointPositions.begin() + idx + 1, fract_);
//        _pointValues.insert(_pointValues.begin() + idx + 1, value_);
        _pointLock.insert(_pointLock.begin() + idx + 1, false);
//        _transitionToPoint.insert(_transitionToPoint.begin() + idx + 1, tf);
//        _typenameOfTransitionToPoint.insert(_typenameOfTransitionToPoint.begin() + idx + 1, _defaultTransitionType);
    }
}

void FVector::RemoveValueAt(const size_t& idx)
{
//    if (idx == 0)
//        return;
//    if (idx == (Size() - 1))
//        return;

    _pointPositions.erase(_pointPositions.begin() + idx);
//    _pointValues.erase(_pointValues.begin() + idx);
    _pointLock.erase(_pointLock.begin() + idx);
//    _transitionToPoint.erase(_transitionToPoint.begin() + idx);
//    _typenameOfTransitionToPoint.erase(_typenameOfTransitionToPoint.begin() + idx);
}

void FVector::Clear()
{
    _pointPositions.clear();
//    _pointValues.clear();

//    _transitionToPoint.clear();
//    _typenameOfTransitionToPoint.clear();

//    _splineControlPoints.clear();
}

size_t FVector::Find(const float& v, const float& eps){
    for (int i=0;i<Size();i++)
        if ( (ValueAt(i) >= (v-eps)) && (ValueAt(i) < (v+eps)) )
            return i;
    return (size_t) -1;
}

void FVector::Sort(){
    //std::sort (_pointPositions.begin(), myvector.begin()+4);
    
    std::vector<std::pair<float, size_t>> indexed;
    
    for (int i=0;i<_pointPositions.size();i++){
        indexed.push_back(std::pair<float, size_t>(_pointPositions[i],i));
    }
    
    std::sort(indexed.begin(), indexed.end(), [](const std::pair<float, size_t>& a, const std::pair<float, size_t>& b) -> bool{ return a.first < b.first; });
    
    decltype(_pointPositions) _swap_positions;
    decltype(_pointLock) _swap_lock;
    
    
    _swap_positions.reserve(_pointPositions.size());
    _swap_lock.reserve(_pointLock.size());
    
    for (const auto& e: indexed){
        _swap_positions.push_back(e.first);
        _swap_lock.push_back(_pointLock[e.second]);
    }
    
    _pointPositions = _swap_positions;
    _pointLock = _swap_lock;
    
    _unsorted = false;
}

//void FVector::InitConstant(const float& c)
//{
//    Clear();
//
//    // resets to two points at 0 and 1
//
//    _valueRange = CurveValueRange::Positive;
//    AddPoint(0, c);
//    AddPoint(1, c);
//    SetPointLock(0, LockEdit::LockX);
//    SetPointLock(1, LockEdit::LockX);
//

//} // const value
//void FVector::InitTF()
//{
//    Clear();
//
//    // resets to two points at -1 and 1
//
//    _valueRange = CurveValueRange::Full;
//    AddPoint(0, -1);
//    AddPoint(1, 1);
//    SetPointLock(0, LockEdit::LockX);
//    SetPointLock(1, LockEdit::LockX);
//
//} // -1..1

size_t FVector::Size() const { return _pointPositions.size(); }

void FVector::SetCycleLeft(const CycleType& v)
{
    _cycleLeft = v;
}
void FVector::SetCycleRight(const CycleType& v)
{
    _cycleRight = v;
}

void FVector::SetPointLock(const size_t& idx, const bool& l)
{
    if (idx >= Size())
        return;
    _pointLock[idx] = l;
}

//void FVector::SetTransitionToPoint(const size_t& idx, const std::string& f)
//{
//    if (idx >= Size())
//        return;
//
//    _transitionToPoint[idx] = EaseFunctorFactory::Create(f);
//    _typenameOfTransitionToPoint[idx] = f;
//}
//void FVector::SetAllTransitions(const std::string& f)
//{
//    for (auto& e : _transitionToPoint) {
//        e = EaseFunctorFactory::Create(f);
//    }
//    for (auto& e : _typenameOfTransitionToPoint)
//        e = f;
//}

//std::string FVector::TransitionAt(const size_t& idx)
//{
//    if (idx >= Size())
//        return "";
//    return _typenameOfTransitionToPoint[idx];
//}

const int32_t FVector::LeftPointIndexAtFraction(const float& f) const
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

const int32_t FVector::RightPointIndexAtFraction(const float& f) const
{
    auto l = LeftPointIndexAtFraction(f);
    if (l == -1)
        return -1;

    if ((l + 1) >= Size())
        return -1;

    return l + 1;

} // -1 for not found;

const FloatRange FVector::ValueRangeForPoint(const size_t& idx)
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

//std::vector<float> FVector::RawValues() { return _pointValues; }
std::vector<float> FVector::RawValues() const { return _pointPositions; }
//std::vector<std::pair<float, float> > FVector::RawPoints()
//{
//    std::vector<std::pair<float, float> > ret;
//    for (int i = 0; i < _pointValues.size(); i++) {
//        std::pair<float, float> p;
//        p.first = _pointValues[i];
//        p.second = _pointPositions[i];
//        ret[i] = p;
//    }
//    return ret;
//}

const float FVector::ValueAt(const size_t& idx) const { return _pointPositions.at(idx); }


//const float FVector::ValueAt(const size_t& idx) const { return _pointValues.at(idx); }

const bool FVector::LockAt(const size_t& idx) const { return _pointLock.at(idx); }


// main:
// TODO:
//float FVector::ValueAtFraction(const float& f0) const
//{
//    float f = f0;
//    _ClampTime(f);
////    if (f == FVector::TimeZero())
////        return 0;
//
//    auto lp = LeftPointIndexAtFraction(f);
//    auto rp = RightPointIndexAtFraction(f);
//
//    // TODO: check -1
//    if (lp < 0)
//        lp = 0;
//    if (rp < 0)
//        rp = Size() - 1;
//
//    auto lt = (lp == -1) ? 0 : TimeAt(lp);
//    auto rt = (rp == -1) ? 1 : TimeAt(rp);
//
//    auto timeRange = rt - lt;
//    auto interp = (timeRange) ? (f - lt) / timeRange : 0;
//
//    //    return (interp<.5)?lt/Size():rt/Size();
//
////    float vOffset = ValueAt(lp);
////    float vRange = ValueAt(rp) - vOffset;
//
//    return vOffset;//_transitionToPoint[rp](interp) * vRange + vOffset; //(interp<.5) ? ValueAt(lp) : ValueAt(rp);
//}

//
//void FVector::SetValue(const size_t& idx, const float& v)
//{
//    if (_pointLock[idx] == LockEdit::LockY)
//        return;
//    if (_pointLock[idx] == LockEdit::LockBoth)
//        return;
//
//    if (idx >= Size())
//        return;
//    auto value = v;
//    _ClampValue(value);
//    _pointValues[idx] = value;
//}

void FVector::SetValue(const size_t& idx, const float& t)
{
    if (idx>= Size()) return;
    if (_pointLock[idx] == true)
        return;
//    if (_pointLock[idx] == LockEdit::LockBoth)
//        return;

    if (idx >= Size())
        return;
    auto value = t;
    // todo: remove/change?
    _ClampTime(value);

//    if (value == FVector::TimeZero())
//        return;

    if (idx>=Size())
        return;
    
//    auto idx2 = idx + 1;
//    if (idx2 < Size())
//        if (t >= _pointPositions[idx2])
//            return;
//
//    if (idx > 0)
//        if (t < _pointPositions[idx - 1])
//            return;

    _pointPositions[idx] = value;
}

void FVector::SetSortedValue(const float& t){
    
}

// ---

//void FVector::SetScaledValue(const size_t& idx, const float& v)
//{
//    SetValue(idx, _convertToNormalised(v));
//}
void FVector::SetScaledValue(const size_t& idx, const float& t)
{
    SetValue(idx, (_timeScale == 0) ? 0 : (t - _timeOffset) / _timeScale);
}

float FVector::ScaledValueAt(const size_t& idx)
{
    return ValueAt(idx) * _timeScale + _timeOffset;
}

//float FVector::ScaledValueAt(const size_t& idx)
//{
//    return _convertFromNormalised(TimeAt(idx));
//}

//void FVector::SetValueConverters(const ConverterFn& converterFrom, const ConverterFn& converterTo)
//{
//    _convertToNormalised = converterFrom;
//    _convertFromNormalised = converterTo;
//}

// ---
//
//void MultiCurve::SetHidden(const std::string& name, const bool& v)
//{
//    _isHidden[name] = v;
//}
//bool MultiCurve::IsHidden(const std::string& name)
//{
//    if (_isHidden.find(name) == _isHidden.end())
//        return false;
//    return _isHidden[name];
//}
//
//MultiCurve::CurveColor MultiCurve::GetColor(const std::string& name)
//{
//    if (_colorForCurve.find(name) == _colorForCurve.end())
//        return defaultColor;
//    return _colorForCurve[name];
//}
//void MultiCurve::SetColor(const std::string& name, const CurveColor& c)
//{
//    _colorForCurve[name] = c;
//}

}; // namespace //
