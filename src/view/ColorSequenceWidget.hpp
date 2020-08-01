#include "AutomationCurve.hpp"

#include "AutomationCurveWidget.hpp"

struct ColorSeqViewSettings : public AutomationViewSettingsCommon {
    
};

namespace AutomationCurve {
    bool ImWidgetColorSeq(const std::string& name, MultiCurve& curve, const ColorSeqViewSettings& settings = ColorSeqViewSettings());
    bool ImWidgetColorSeqList(const std::string& name, MultiCurve& curve, const ColorSeqViewSettings& settings = ColorSeqViewSettings());
}
