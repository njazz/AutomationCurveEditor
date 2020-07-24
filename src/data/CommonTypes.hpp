
namespace AutomationCurve{

struct FloatRange {
    float min = 0;
    float max = 1;
};

enum class CycleType {
    Hold,
    Zero,
    Mirror,
    Repeat
};

// ---
namespace Codec {
     std::string ToString(const CycleType& ct);
     bool FromString(CycleType& ct, const std::string& s);
};

}
