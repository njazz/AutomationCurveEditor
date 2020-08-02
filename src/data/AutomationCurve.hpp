// curve for parameter automation

#ifndef AUTOMATION_CURVE_HPP
#define AUTOMATION_CURVE_HPP

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

#include "CommonTypes.hpp"

namespace AutomationCurve {

using EaseFunctor = std::function<float(const float&)>; // input value is 0..1


struct EaseFunctorFactory {
private:
    static const std::map<std::string, EaseFunctor> _map;

public:
    static EaseFunctor Create(const std::string& name);
    static std::vector<std::string> Names();
    // later: cached version
    // static EaseFunctor CreateWithLUT(const std::string& name, const size_t tableSize);
};

struct SplineControlPoints {
    float startFraction = 0;
    float startValueOffset = 0;
    float endFraction = 1;
    float endValueOffset = 0;
};

// LATER:
//struct SplineFunctor {
//private:
//    SplineControlPoints& _data;
//
//public:
//    SplineFunctor(SplineControlPoints& d)
//        : _data(d)
//    {
//    }
//
//    void operator()(float&, const float&) {}
//};



enum class CurveValueRange {
    Positive, // 0..1 for parameters
    Full // -1..1 for transfer functions
};

enum class LockEdit {
    None,
    LockX,
    LockY,
    LockBoth
};

template <typename VAL>
struct Optional final {
protected:
    VAL value;
    bool null = true;

    // ---
public:
    bool IsNull() const { return null; }

    void SetNull() { null = true; }

    void Set(const VAL& d)
    {
        value = d;
        null = false;
    }

    const VAL& Get() const
    {
        if (!null)
            return value;
        else
            throw("Optional value error.");
    }
    
    VAL& GetRef() const
    {
        if (!null)
            return value;
        else
            throw("Optional value error.");
    }

public:
    Optional() {}
    Optional(const VAL& v) { Set(v); }
    Optional& operator=(const VAL& v)
    {
        Set(v);
        return *this;
    }
};

//struct Codec;
//struct JSONCodec;

struct ACurve;
using ACurvePtr = std::shared_ptr<ACurve>;

struct ACurve {
protected:
    std::vector<float> _pointPositions;
    std::vector<float> _pointValues;

    std::vector<EaseFunctor> _transitionToPoint; // size, at(0) is ignored
    std::vector<std::string> _typenameOfTransitionToPoint;

    std::vector<LockEdit> _pointLock;

    std::map<size_t, SplineControlPoints> _splineControlPoints; // end poind holds both spline control point with relative values

    std::string _defaultTransitionType = "Linear";

//    CycleType _cycleType = CycleType::Hold;

    CycleType _cycleLeft = CycleType::Hold;
    CycleType _cycleRight = CycleType::Hold;
    
    static constexpr float TimeZero() { return -1; } ;
    
    CurveValueRange _valueRange = CurveValueRange::Positive;
    
    ConverterFn _convertToNormalised = [](const float&f) { return f;};
    ConverterFn _convertFromNormalised = [](const float&f) { return f;};
    
    float _timeOffset = 0;
    float _timeScale = 1;

    // ---

    void _ClampTime(float& t) const;  // may return ACurve::TimeZero() in "zero" cycle mode
    void _ClampValue(float& t) const;

//    friend struct Codec;
//    friend struct JSONCodec;
    // ---
public:
    ACurve() { InitConstant(0); }

    void AddPoint(const float& fract, const float& value);
    void RemovePointAt(const size_t& idx);

    void Clear(); // resets to two points at 0 and 1

    void InitConstant(const float& c); // const value
    void InitTF(); // -1..1

    size_t Size() const;
    
    void SetCycleLeft(const CycleType& v);
    void SetCycleRight(const CycleType& v);
    
    inline CycleType CycleLeft() const { return _cycleLeft;}
    inline CycleType CycleRight() const { return _cycleRight;}
    
    void SetPointLock(const size_t& idx, const LockEdit& l);

    void SetTransitionToPoint(const size_t& idx, const std::string& f);
    void SetAllTransitions(const std::string& f);

    const int32_t LeftPointIndexAtFraction(const float& f) const; // -1 for not found;
    const int32_t RightPointIndexAtFraction(const float& f) const; // -1 for not found;

    const FloatRange TimeRangeForPoint(const size_t& idx);

    void SetDefaultTransitionType(const std::string& tt) { _defaultTransitionType = tt; }
    const std::string DefaultTransitionType() const { return _defaultTransitionType; }

    std::vector<float> RawValues() const;
    std::vector<float> RawPositions() const;
    std::vector<EaseFunctor> RawTransitions() const { return _transitionToPoint; };
    std::vector<std::string> RawTransitionTypenames() const { return _typenameOfTransitionToPoint; };
    std::vector<LockEdit> RawPointLocks() const { return _pointLock;}
    CurveValueRange RawValueRange() const { return _valueRange; }
    
    // for deserialization
    void SetRawPoints(const std::vector<float>& pos, const std::vector<float>& v, std::vector<LockEdit> locks);
    void SetRawValueRange(const CurveValueRange& vr) { _valueRange = vr; }
    
    std::vector<std::pair<float, float> > RawPoints();  // -> GetRawPoints

    const float TimeAt(const size_t& idx) const;
    const float ValueAt(const size_t& idx) const;
    const LockEdit LockAt(const size_t& idx) const;
    std::string TransitionAt(const size_t& idx);

    float ValueAtFraction(const float& f) const;
    Optional<float> ValueAtScaledTime(const float& t) const;

    void SetValue(const size_t& idx, const float& v);
    void SetTime(const size_t& idx, const float& t);
    
    // scaled
    void SetTimeOffset(const float f) { _timeOffset = f; }
    void SetTimeScale(const float& f) { _timeScale = f; }
    
    const float TimeOffset() const { return _timeOffset; }
    const float TimeScale() const { return _timeScale; }
    
    void SetScaledValue(const size_t& idx, const float& v);
    void SetScaledTime(const size_t& idx, const float& t);
    
    float ScaledTimeAt(const size_t& idx);
    float ScaledValueAt(const size_t& idx);
    
    void SetValueConverters(const ConverterFn& convertToNormalized, const ConverterFn& convertFromNormalized);

    //
    static ACurvePtr CreatePtr() { return std::make_shared<ACurve>(); }
};

/// @brief main converter static class
struct CurveConverter {
    ACurvePtr curve;
    CurveConverter(ACurvePtr v)
        : curve(v){};

    const std::vector<float> AsFloatVector(const size_t res); // resolution
    const ACurve AsLineApproximation(const size_t res); // constant resolution
    const ACurve AsApproximation(const size_t numsteps); // num steps
    const std::vector<std::pair<float, float> > AsPoints(const size_t res);
};

/// @brief extended class that reports progress fraction, can cancel task etc
struct ThreadedConverter {
    // TODO: shared ptr
    ACurvePtr _value;
    ThreadedConverter(const ACurvePtr v)
        : _value(v){};

    const float GetProgressFraction();

    struct OptionalFloatVector {
        std::vector<float> data;
        bool ready = false;
    };

    struct OptionalACurve {
        ACurve data;
        bool ready = false;
    };

    struct OptionalPointsVector {
        std::vector<std::pair<float, float> > data;
        bool ready = false;
    };

    void Cancel_T();

    const void AsFloatVector_T(OptionalFloatVector& obj, const FloatRange& timeRange, const size_t res); // resolution
    const void AsLineApproximation_T(OptionalACurve& obj, const FloatRange& timeRange, const size_t res); // constant resolution
    const void AsApproximation_T(OptionalACurve& obj, const FloatRange& timeRange, const size_t numsteps); // num steps
    const void AsPoints_T(OptionalPointsVector& obj, const FloatRange& timeRange, const size_t res);
};

/// @brief editor functions,  point selection is still stored in main class
struct CurveEditor {
    std::vector<size_t> _selectionIdx;

    ACurvePtr curve;

    CurveEditor(ACurvePtr v)
        : curve(v){};

    bool IsSelected(const size_t idx);
    size_t SelectionSize() const { return _selectionIdx.size(); }

    void SelectPoint(const size_t idx);
    void DeselectPoint(const size_t idx);
    void ClearSelection();

    void SelectAll();

    // relative
    void MoveSelectionValue(const float& v);
    void MoveSelectionTime(const float& v);

    void SetSelectionTransitions(const std::string& t);
    void SetSelectionLocks(const LockEdit& l);

    void DeleteSelection();
};

// ---

struct MultiCurve {
    struct CurveColor {
        unsigned char r = (unsigned char)192;
        unsigned char g = (unsigned char)168;
        unsigned char b = (unsigned char)0;

        CurveColor() {}
        CurveColor(const unsigned char r_, const unsigned char g_, const unsigned char b_)
            : r(r_)
            , g(g_)
            , b(b_)
        {
        }
    };

private:
    std::map<std::string, CurveColor> _colorForCurve;
    std::map<std::string, bool> _isHidden;
    std::string _activeCurve = "";

public:
    CurveColor defaultColor;
    std::map<std::string, ACurvePtr> curves;

    CurveEditor editor = CurveEditor(nullptr);

    void SetActiveCurveName(const std::string& ac)
    {
        _activeCurve = ac;
        if (HasActiveCurve())
            editor = CurveEditor(curves[_activeCurve]);
    }
    std::string ActiveCurveName() { return _activeCurve; }
    ACurvePtr ActiveCurve() { return (HasActiveCurve()) ? curves[_activeCurve] : nullptr; }

    inline bool HasActiveCurve() { return curves.find(_activeCurve) != curves.end(); }

    void SetHidden(const std::string& name, const bool& v);
    bool IsHidden(const std::string& name); // default false

    CurveColor GetColor(const std::string& name);
    void SetColor(const std::string& name, const CurveColor& c);
    
    //
    float GetMinTimeOffset();
    float GetMaxTimeScale();
};

// ---

    struct WidgetCoordinateConverter{
        float widgetWidth = 400;
        
        float timeOffset = 0;
        float timeScale = 1;
        
        float curveTimeOffset = 0;
        float curveTimeScale = 1;
        
        float scrollOffset = 0;
        float zoomValue = 1;
        
        inline float WidthInSeconds() { return timeOffset + timeScale; }
        
        
        //
        float FractionCurveToGlobal(const float& f);
        float FractionGlobalToCurve(const float& f);
        
        float PixelToCurveFraction(const float& px);
        float PixelToSeconds(const float& px);
        
        float SecondsToPixel(const float& sec);
        float SecondsToCurveFraction(const float& sec);
        
        float CurveFractionToSeconds(const float& fr);
        float CurveFractionToPixel(const float& fr);
    };

// ---

namespace Codec {
//    static std::string ToString(const CycleType& ct);
     std::string ToString(const CurveValueRange& ct);
     std::string ToString(const LockEdit& ct);

//    static bool FromString(CycleType& ct, const std::string& s);
     bool FromString(CurveValueRange& ct, const std::string& s);
     bool FromString(LockEdit& ct, const std::string& s);

    // mc color - json (impl)
    // mc json
};

}; // namespace

#endif
