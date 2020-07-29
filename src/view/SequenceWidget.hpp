
#include "imgui.h"
#include "Sequence.hpp"

struct SequenceViewSettings{
    ImVec2 viewRange;
    ImVec2 size;
};

namespace AutomationCurve {
    bool ImWidgetSequence(const std::string& name, const ImVec2& size, Sequence& seq, const ImVec2& viewRange);
}
