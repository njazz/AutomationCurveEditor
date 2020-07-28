#include "imgui.h"
#include "AutomationCurve.hpp"

struct WidgetViewStyle {
    bool readOnly = false;
    bool hasSelection = false;
    bool hasPlayhead = false;
    
    float playhead = 0;
    float selectionStart = 0;
    float selectionEnd = 1;
};

namespace AutomationCurve{
    bool ImWidget(const std::string& name, const ImVec2& size, CurveEditor& curve, const ImVec2& viewRange);
    bool ImWidget(const std::string& name, const ImVec2& size, CurveEditor& curve, const float& scroll = 0.0f, const float& zoomRange = 1.0f);
    
    bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const float& scroll = 0.0f, const float& zoomRange = 1.0f);
    bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const ImVec2& viewRange);
    
    bool ImWidgetOverview(const std::string& name,const ImVec2& size,ACurve& curve, float* scroll = 0, float* zoomRange = 0 );
    bool ImWidgetOverviewMulti(const std::string& name,const ImVec2& size,MultiCurve& curve, float* scroll = 0, float* zoomRange = 0 );
    
    bool ImWidgetListView(const std::string& name, CurveEditor& curve);
};
