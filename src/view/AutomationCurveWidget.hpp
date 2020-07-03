#include "imgui.h"
#include "AutomationCurve.hpp"

namespace AutomationCurve{
    bool ImWidget(const std::string& name, const ImVec2& size, ACurve& curve, const ImVec2& viewRange);
    bool ImWidget(const std::string& name, const ImVec2& size, ACurve& curve, const float& scroll = 0.0f, const float& zoomRange = 1.0f);
    
    bool ImWidgetOverview(const std::string& name,const ImVec2& size,ACurve& curve, float* scroll = 0, float* zoomRange = 0 );
    
    void ImWidgetListView(const std::string& name,ACurve& curve);
    
    bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const float& scroll = 0.0f, const float& zoomRange = 1.0f);
    bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const ImVec2& viewRange);
    bool ImWidgetOverviewMulti(const std::string& name,const ImVec2& size,MultiCurve& curve, float* scroll = 0, float* zoomRange = 0 );
    
};
