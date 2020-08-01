// timeline & extras
#include "imgui.h"
#include <vector>

struct TimelineViewSettings {
    ImVec2 timeRange = ImVec2(0,60);
};

struct MeterViewSettings{
   ImVec2 size = ImVec2(200,30);

    inline bool IsHorizontal(){ return size.x > size.y; }
    
    float dbRangeMin = -70;
    float dbRangeMax = 0;
    int gridStep = 10;
};

//
namespace AutomationCurve {
    void ImWidgetTimeline(const TimelineViewSettings& settings);
    
    // forward to vec
    void ImWidgetMeter(const float& v, const MeterViewSettings& settings = MeterViewSettings());
    void ImWidgetMeter(const std::vector<float>& v, const MeterViewSettings& settings = MeterViewSettings());
};
