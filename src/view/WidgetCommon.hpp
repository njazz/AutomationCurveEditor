
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace AutomationCurve {

struct ACurve;
struct CurveEditor;

void __DrawGrid(ImGuiWindow* window, const ImRect& bb, const float& range_x = 1, const float& offset_x = 0, const float& mult_x = 1);
void __DrawSmoothCurve(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered), const float& offset_x = 0, const float& mult_x = 1, int lineWidth = 2, int nPoints = 256);
void __DrawEditorFeatures(ImGuiWindow* window, const ImRect& bb, CurveEditor& editor, const ImU32& color, const float& offset_x, const float& mult_x);
}
