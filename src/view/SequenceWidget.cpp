#include "SequenceWidget.hpp"

namespace AutomationCurve {

bool ImWidgetSequence(const std::string& name, SequenceItem& item, const SequenceItemSettings& settings) { return false; }
bool ImWidgetSequence(const std::string& name, SequenceTrack& seq, const SequenceTrackSettings& settings) { return false; }

bool ImWidgetSequence(const std::string& name, const ImVec2& size, Sequence& seq, const ImVec2& viewRange) { return false; }
}
