#ifndef AUTOMATION_CURVE_WIDGET_HPP
#define AUTOMATION_CURVE_WIDGET_HPP

#include "imgui.h"
#include "AutomationCurve.hpp"

namespace AutomationCurve {

struct AutomationViewSettingsCommon {
    bool readOnly = false;
    bool hasSelection = false;
    bool hasPlayhead = false;

    float playhead = 0;
    float selectionStart = 0; // fraction
    float selectionEnd = 1;

    ImVec2 size = ImVec2(400, 300);
//    ImVec2 viewRange = ImVec2(0, 1);
    float scroll = 0;
    float zoom = 1;
    
    unsigned char lineWidth = 1;
    unsigned char selectedLineWidth = 2;
    unsigned int nPoints = 256;
};

struct AutomationViewSettings : public AutomationViewSettingsCommon {
};

struct AutomationMultiViewSettings : public AutomationViewSettingsCommon {
};

bool ImWidget(const std::string& name, const ImVec2& size, CurveEditor& curve, const ImVec2& viewRange);
bool ImWidget(const std::string& name, const ImVec2& size, CurveEditor& curve, const float& scroll = 0.0f, const float& zoomRange = 1.0f);

bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const float& scroll = 0.0f, const float& zoomRange = 1.0f);
bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const ImVec2& viewRange);

bool ImWidgetOverview(const std::string& name, const ImVec2& size, CurveEditor& curve, float* scroll = 0, float* zoomRange = 0);
bool ImWidgetOverviewMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, float* scroll = 0, float* zoomRange = 0);

bool ImWidgetListView(const std::string& name, CurveEditor& curve);

// new api style
bool ImWidget(const std::string& name, CurveEditor& curve, const AutomationViewSettings& = AutomationViewSettings());
bool ImWidgetMulti(const std::string& name, MultiCurve& curve, const AutomationMultiViewSettings& = AutomationMultiViewSettings());

bool ImWidgetOverview(const std::string& name, CurveEditor& curve, const AutomationViewSettings& = AutomationViewSettings());
bool ImWidgetOverviewMulti(const std::string& name, MultiCurve& curve, const AutomationMultiViewSettings& = AutomationMultiViewSettings());

bool ImWidgetListView(const std::string& name, CurveEditor& curve);
bool ImWidgetListViewMulti(const std::string& name, MultiCurve& curve);

} // namespace

#endif
