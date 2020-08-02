#include "ColorSequenceWidget.hpp"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

using namespace ImGui;

namespace AutomationCurve {

bool ImWidgetColorSeq(const std::string& name, MultiCurve& curve, const ColorSeqViewSettings& settings)
{
    // TODO:
    // multi editor with linked add/remove; curve selection
    
    
//
//    ImGuiWindow* window = GetCurrentWindow();
//    const ImGuiStyle& style = GetStyle();
//    if (window->SkipItems)
//        return false;
//
//    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + settings.size);
//    ItemSize(bb);
//    if (!ItemAdd(bb, 0))
//        return false;
//
//    //
//
//    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
//
//    PushClipRect(bb.Min, bb.Max, false);
//
//    // bad data format
//    if (curve.curves.size() < 1) {
//        TextUnformatted("No data");
//        PopClipRect();
//
//        return false;
//    }
//
//    // vertical lines
//    for (int i = 0; i < curve.curves.begin()->second->Size(); i++) {
//        auto f = curve.curves.begin()->second->TimeAt(i);
//        auto x = bb.Min.x + 10 + (f * (bb.Max.x - bb.Min.x - 20));
//
//        window->DrawList->AddLine(ImVec2(x, bb.Min.y), ImVec2(x, bb.Max.y), IM_COL32_WHITE, 2);
//
//        SetCursorPos(ImVec2(x - 5 - bb.Min.x, .5 *(settings.size.y+10)));
//        SetNextItemWidth(10);
//        // 3 first == r,g,b
//        if (curve.curves.size() >= 3) {
//            auto id_ = ("##color_" + std::to_string(i));
////            float col_ [4];
//
//
////            ColorPicker3(id_.c_str(), col_);
//            ColorButton(id_.c_str(), ImVec4(curve.curves["R"]->ValueAt(i) ,
//                                                                      curve.curves["G"]->ValueAt(i) ,
//                                                                      curve.curves["B"]->ValueAt(i) ,
//                                                                      255), ImGuiColorEditFlags_RGB, ImVec2(20,20));
//        }
//        else { TextUnformatted("-");}
//    }
//
//    PopClipRect();

    return false;
}
} // namespace
