#include "AutomationCurve.hpp"

#include "AutomationCurveWidget.hpp"



namespace AutomationCurve {

struct ColorSeqViewSettings : public AutomationViewSettingsCommon {
    
};

    bool ImWidgetColorSeq(const std::string& name, MultiCurve& curve, const ColorSeqViewSettings& settings = ColorSeqViewSettings());
    bool ImWidgetColorSeqList(const std::string& name, MultiCurve& curve, const ColorSeqViewSettings& settings = ColorSeqViewSettings());
}
