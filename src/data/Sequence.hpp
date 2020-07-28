
#include "CommonTypes.hpp"
#include <vector>
namespace AutomationCurve{

struct SequenceItem {
    float time = 0;
    float length = 0;
    unsigned long id;
    float fadeIn;
    float fadeOut;
    float gain;
    
    //
    CycleType cycleLeft = CycleType::Hold;
    CycleType cycleRight = CycleType::Hold;
};

struct SequenceTrack {
    std::vector<SequenceItem> items;

    std::vector<SequenceItem> GetItemsAt(const float& t);
    std::vector<SequenceItem> GetItemsAtRange(const float& t, const float& len);
    
    size_t Size() const;
};

struct Sequence{
    std::vector<SequenceTrack> tracks;
};

} //namespace


