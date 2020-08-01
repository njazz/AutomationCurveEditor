
#include <vector>
//#include <map>
//#include <unordered_map>
#include <string>
//#include <functional>
#include <memory>

#include "CommonTypes.hpp"

//// common:
//
//enum class CycleType {
//    Hold,
//    Zero,
//    Mirror,
//    Repeat
//};
//
//struct FloatRange {
//    float min = 0;
//    float max = 1;
//};
//
//// ---
//struct Codec{
//     static std::string ToString(const CycleType& ct);
////    static std::string ToString(const CurveValueRange& ct);
////    static std::string ToString(const LockEdit& ct);
//
//    static bool FromString(CycleType& ct, const std::string& s);
////    static bool FromString(CurveValueRange& ct, const std::string& s);
////    static bool FromString(LockEdit& ct, const std::string& s);
//};

// ---
namespace AutomationCurve{

struct FVector;
using FVectorPtr = std::shared_ptr<FVector>;

struct FVector {
protected:
    std::vector<float> _pointPositions;
    std::vector<bool> _pointLock;
    
    CycleType _cycleLeft = CycleType::Hold;
    CycleType _cycleRight = CycleType::Hold;
    
    float _timeOffset = 0;
    float _timeScale = 1;
    
    ConverterFn _convertToNormalised = [](const float&f) { return f;};
    ConverterFn _convertFromNormalised = [](const float&f) { return f;};
    
    void _ClampTime(float& t) const;
    
//    friend struct Codec;
//    friend struct JSONCodec;

    bool _unsorted = false;     // is set when unsorted methods are called; reset on "Sort()"
    
    public:
    
    void InsertValueAt(const size_t& idx, const float& fract);   // unsorted
    void AddValue(const float& fract);                          // sorted
    void RemoveValueAt(const size_t& idx);
    
    void SetValue(const size_t& idx, const float& t);
    void SetSortedValue(const float& t);
    
    void Sort();
    
    void Clear();
    
    bool IsSorted() { return _unsorted; }
    
    void SetCycleLeft(const CycleType& v);
    void SetCycleRight(const CycleType& v);
    
    inline CycleType CycleLeft() const { return _cycleLeft;}
    inline CycleType CycleRight() const { return _cycleRight;}
    
    void SetPointLock(const size_t& idx, const bool& l);
    
    const int32_t LeftPointIndexAtFraction(const float& f) const; // -1 for not found;
    const int32_t RightPointIndexAtFraction(const float& f) const; // -1 for not found;

    const FloatRange ValueRangeForPoint(const size_t& idx);
    
    std::vector<float> RawValues() const;
    

    
    const float ValueAt(const size_t& idx) const;
    const bool LockAt(const size_t& idx) const;
    
    void SetScaledValue(const size_t& idx, const float& t);
    float ScaledValueAt(const size_t& idx);
    
    void SetValueConverters(const ConverterFn& convertToNormalized, const ConverterFn& convertFromNormalized);
    
    size_t Size() const;// { return _pointPositions.size(); }

    static FVectorPtr CreatePtr() { return std::make_shared<FVector>(); }
};

struct FVectorConverter {
    // todo:
    std::vector<float> ToVec() {return {};}   //
    void ToACurve();
    void MergeToACurve_1();   // FVector, FVector
    void MergeToACurve_2();   // FVector, FVector
    
    void AsFloatVector();        // with fixed resolution
    void Subdivide();            // subdivide
    
};

}; // namespace
