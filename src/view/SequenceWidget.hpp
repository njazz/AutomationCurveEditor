
#include "imgui.h"
#include "Sequence.hpp"

struct SequenceViewSettings{
    ImVec2 viewRange = ImVec2(0,1);
    ImVec2 size = ImVec2(400,300);
};

struct SequenceTrackSettings{
    unsigned int laneCount = 0;  // 0 == automatic
};

struct SequenceItemSettings{
    
};

namespace AutomationCurve {

    bool ImWidgetSequence(const std::string& name,  SequenceItem& item, const SequenceItemSettings& settings);
    bool ImWidgetSequence(const std::string& name,  SequenceTrack& seq, const SequenceTrackSettings& settings);
    
    bool ImWidgetSequence(const std::string& name,  Sequence& seq, const SequenceViewSettings& settings);
}
