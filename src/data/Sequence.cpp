
#include "Sequence.hpp"

namespace AutomationCurve {

std::vector<SequenceItem> SequenceTrack::GetItemsAt(const float& t)
{
    std::vector<SequenceItem> ret;

    for (auto& e : items)
        if (( (e.time+e.length) >= t) && ((e.time) <= t))
            ret.push_back(e);
    return ret;
}

std::vector<SequenceItem> SequenceTrack::GetItemsAtRange(const float& t, const float& len)
{

    std::vector<SequenceItem> ret;

    for (auto& e : items)
        if (((e.time + e.length) >= t) && ((e.time) <= (t + len)))
            ret.push_back(e);
    return ret;
}
}
