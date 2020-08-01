#include "FVectorWidget.hpp"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace AutomationCurve {

// new api style
bool ImWidgetBars(const std::string& name, FVector& vec, const BarsViewSettings& settings)
{
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    if (window->SkipItems)
        return false;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + settings.size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    //

    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);

    PushClipRect(bb.Min, bb.Max, false);

    // todo
    //    if (vec.Size() < 1) {
    //        PopClipRect();
    //        return false;
    //    }

    float w = settings.size.x / vec.Size();

    auto fillColor = IM_COL32(192, 160, 128, 255);
    auto lineColor = IM_COL32(208, 192, 160, 255);

    if (GetIO().KeyMods & ImGuiKeyModFlags_Alt) {
        fillColor = IM_COL32(128, 160, 192, 255);
        lineColor = IM_COL32(160, 192, 208, 255);
    }

    float barSize = (bb.Max.x - bb.Min.x);
    if (vec.Size())
        barSize = barSize / vec.Size();

    unsigned int hoverIndex = 0;
    if (vec.Size())
        hoverIndex = (GetIO().MousePos.x - bb.Min.x) / barSize;

    float addBarSize = (bb.Max.x - bb.Min.x);
    if (vec.Size())
        addBarSize = addBarSize / (vec.Size() + 1);
    unsigned int addIndex = 0;
    if (vec.Size())
        addIndex = (GetIO().MousePos.x - bb.Min.x) / addBarSize;
    if (addIndex >= (vec.Size()))
        addIndex = (vec.Size());

    auto fillColorSel = IM_COL32(208, 192, 128, 255);
    auto lineColorSel = IM_COL32(224, 208, 160, 255);

    for (int i = 0; i < vec.Size(); i++) {
        float h = vec.ValueAt(i) * settings.size.y;

        auto fillColor_ = (i == hoverIndex) ? fillColorSel : fillColor;
        auto lineColor_ = (i == hoverIndex) ? lineColorSel : lineColor;

        if (GetIO().KeyMods & ImGuiKeyModFlags_Shift) {
            fillColor_ = fillColor; // ( i == addIndex) ? fillColor : fillColor;
            lineColor_ = lineColor; //( i == addIndex) ? lineColorSel : lineColor;

            // last bar
            //            if ( (i== vec.Size()-1) && (addIndex == vec.Size()) ){
            //                fillColor_ = IM_COL32(224, 208, 128, 255);
            //                lineColor_ = IM_COL32(224, 208, 128, 255);
            //            }
        }

        window->DrawList->AddRectFilled(bb.Min + ImVec2(i * w, bb.Max.y - h - bb.Min.y), bb.Min + ImVec2((i + 1) * w, bb.Max.y - bb.Min.y), fillColor_);
        window->DrawList->AddRect(bb.Min + ImVec2(i * w, bb.Max.y - h - bb.Min.y), bb.Min + ImVec2((i + 1) * w, bb.Max.y - bb.Min.y), lineColor_);
    }

    float hoverValue = 1. - float(((GetIO().MousePos.y - bb.Min.y) / (bb.Max.y - bb.Min.y)));
    
    const ImGuiID id = window->GetID(name.c_str());

    //
    bool hovered = IsItemHovered();
    if (hovered) {
        SetHoveredID(id);
    }
    
    // new bar
    float w2 = settings.size.x / (vec.Size() + 1.0f);
    if (GetIO().KeyMods & ImGuiKeyModFlags_Shift) {
        for (int i = 0; i < (vec.Size() + 1); i++) {
            float h = settings.size.y * (hoverValue);

            //        window->DrawList->AddRectFilled(bb.Min + ImVec2(i * w, bb.Max.y - h - bb.Min.y), bb.Min + ImVec2((i + 1) * w, bb.Max.y- bb.Min.y), fillColor_);
            auto lineColor_ = IM_COL32(255, 208, 128, 255);
            if (i == addIndex){
                window->DrawList->AddRect(bb.Min + ImVec2(i * w2, bb.Max.y - h - bb.Min.y), bb.Min + ImVec2((i + 1) * w2, bb.Max.y - bb.Min.y), lineColor_);
//                auto cur = GetCursorPos();
                SetCursorPos(bb.Min - GetWindowPos());
            Text("add : %i", addIndex);
//            SetCursorPos(cur);
                }
        }
    }

    //    printf("%f\n", hoverValue);

    

    // mouse - move y
    if (hovered) {
        if (IsMouseDragging(ImGuiMouseButton_Left)) {
            vec.SetValue(hoverIndex, hoverValue);
            //            ResetMouseDragDelta();
            //            GetIO().MouseDown[0] = false;
            SetCursorPos(bb.Min - GetWindowPos());
            Text("%i : %f", hoverIndex, hoverValue);
        }
    };

    // add with shift
    if (hovered && IsMouseClicked(ImGuiMouseButton_Left) && !(IsMouseDragging(ImGuiMouseButton_Left))) {
        if (GetIO().KeyMods & ImGuiKeyModFlags_Shift) {
            vec.InsertValueAt(addIndex, hoverValue);
        }
    }

    // remove with alt
    if (hovered) {
        if (GetIO().KeyMods & ImGuiKeyModFlags_Alt) {
        SetCursorPos(bb.Min - GetWindowPos());
            Text("remove : %i", hoverIndex);
        if (  IsMouseClicked(ImGuiMouseButton_Left) && !(IsMouseDragging(ImGuiMouseButton_Left))) {
            vec.RemoveValueAt(hoverIndex);
            
        }}
    }

    PopClipRect();

    return false;
}

bool ImWidgetSlices(const std::string& name, FVector& vec, const SlicesViewSettings& settings)
{

    // mouse - move x

    // add with shift

    // remove with alt

    return false;
}

// draw curve values. replace time or equal

bool ImWidgetBars(const std::string& name, ACurve& vec, const BarsViewSettings& settings) { return false; }
bool ImWidgetSlices(const std::string& name, ACurve& vec, const SlicesViewSettings& settings) { return false; }
};
