
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"



namespace AutomationCurve {

struct ACurve;
struct CurveEditor;

struct AutomationViewSettingsCommon;
struct WidgetCoordinateConverter;

void __DrawGrid(ImGuiWindow* window, const ImRect& bb, const float& range_x = 1, const float& offset_x = 0, const float& mult_x = 1);
void __DrawSmoothCurve(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered), const float& offset_x = 0, const float& mult_x = 1, int lineWidth = 2, int nPoints = 256);
void __DrawEditorFeatures(ImGuiWindow* window, const ImRect& bb, CurveEditor& editor, const ImU32& color, const float& offset_x, const float& mult_x);

void __DrawSmoothCurve_Timed(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color, const float& offset_x , const float& mult_x , int lineWidth , int nPoints , float timeOffset, float timeScale);
void __DrawEditorFeatures_Timed(ImGuiWindow* window, const ImRect& bb, CurveEditor& editor, const ImU32& color, const float& offset_x, const float& mult_x, float timeOffset, float timeScale, float timeScaleSingle);

// NEW
void __DrawGrid_2(ImGuiWindow* window, const ImRect& bb, const AutomationViewSettingsCommon& settings);
void __DrawSmoothCurve_2(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color, const AutomationViewSettingsCommon& settings,const WidgetCoordinateConverter& converter);
void __DrawEditorFeatures_2(ImGuiWindow* window, const ImRect& bb, CurveEditor& editor, const ImU32& color, const AutomationViewSettingsCommon& settings, const WidgetCoordinateConverter& converter);

}
