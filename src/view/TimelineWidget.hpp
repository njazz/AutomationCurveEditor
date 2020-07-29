// timeline & extras
#include "imgui.h"

struct TimelineViewSettings {

};

struct MeterViewSettings{
    float dbRangeMin = -70;
    float dbRangeMax = 0;
};

//
namespace AutomationCurve {
    bool ImWidgetTimeline(const TimelineViewSettings& settings);
    
    bool ImWidgetMeter(const float& v, const MeterViewSettings& settings);
};
