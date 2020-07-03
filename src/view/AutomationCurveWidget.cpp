// optional: imgui widget

#include "AutomationCurve.hpp"
#include "AutomationCurveWidget.hpp"

#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "math.h"

namespace AutomationCurve {

static inline void __DrawGrid(ImGuiWindow* window, const ImRect& bb, const float& range_x = 1, const float& offset_x = 0, const float& mult_x = 1)
{
    using namespace ImGui;

    float ht = bb.Max.y - bb.Min.y;
    float wd = bb.Max.x - bb.Min.x;

    window->DrawList->AddLine(
        ImVec2(bb.Min.x, bb.Min.y + ht / 2),
        ImVec2(bb.Max.x, bb.Min.y + ht / 2),
        GetColorU32(ImGuiCol_TextDisabled), 3);

    window->DrawList->AddLine(
        ImVec2(bb.Min.x, bb.Min.y + ht / 4),
        ImVec2(bb.Max.x, bb.Min.y + ht / 4),
        GetColorU32(ImGuiCol_TextDisabled));

    window->DrawList->AddLine(
        ImVec2(bb.Min.x, bb.Min.y + ht / 4 * 3),
        ImVec2(bb.Max.x, bb.Min.y + ht / 4 * 3),
        GetColorU32(ImGuiCol_TextDisabled));

    const int numsteps = 10;
    float steps = (numsteps * range_x);
    for (int i = 1; i < ceil(steps); i++) {
        auto w2 = float(wd / (steps));
        //        auto w2c = float(wd/ceil(steps));
        auto off2 = -fmodf(offset_x, 1.0f / (numsteps)) * wd * mult_x; //-fmodf((offset_x)*wd,(wd/11));
        window->DrawList->AddLine(
            ImVec2(bb.Min.x + w2 * (i) + off2, bb.Min.y),
            ImVec2(bb.Min.x + w2 * (i) + off2, bb.Max.y),
            GetColorU32(ImGuiCol_TextDisabled));
    }
};

static inline void
__DrawSmoothCurve(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered),const float& offset_x = 0, const float& mult_x = 1, int lineWidth = 2, int nPoints = 256)
{
    using namespace ImGui;
    
    for (int i = 0; i <= (nPoints - 1); ++i) {
        float px = (i + 0) / float(nPoints);
        float qx = (i + 1) / float(nPoints);
        float py = 1 - curve.ValueAtFraction(px);
        float qy = 1 - curve.ValueAtFraction(qx);
        ImVec2 p((px - offset_x) * (bb.Max.x - bb.Min.x) * mult_x + bb.Min.x, py * (bb.Max.y - bb.Min.y) + bb.Min.y);
         ImVec2 q((qx - offset_x) * (bb.Max.x - bb.Min.x) * mult_x + bb.Min.x, qy * (bb.Max.y - bb.Min.y) + bb.Min.y);

        window->DrawList->AddLine(p, q, color, lineWidth);
    }
}

static inline void
__DrawEditorFeatures(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color, const float& offset_x, const float& mult_x)
{
    using namespace ImGui;
    
    // locks
    for (int i = 1; i < curve.Size(); i++) {
        ImVec2 a;
        a.x = curve.RawPositions()[i - 1];
        a.y = curve.RawValues()[i - 1];
        ImVec2 b;
        b.x = curve.RawPositions()[i];
        b.y = curve.RawValues()[i];

        a.y = 1 - a.y;
        b.y = 1 - b.y;

        a.x = (a.x - offset_x) * mult_x;
        b.x = (b.x - offset_x) * mult_x;

        a = a * (bb.Max - bb.Min) + bb.Min;
        b = b * (bb.Max - bb.Min) + bb.Min;

        //x
        if ((curve.LockAt(i) == LockEdit::LockX) || (curve.LockAt(i) == LockEdit::LockBoth)) {
            window->DrawList->AddLine(ImVec2(b.x, 0), ImVec2(b.x, bb.Max.y), GetColorU32(ImGuiCol_PlotLines));
        }
        // y
        if ((curve.LockAt(i) == LockEdit::LockY) || (curve.LockAt(i) == LockEdit::LockBoth)) {
            window->DrawList->AddLine(ImVec2(0, b.y), ImVec2(bb.Max.x, b.y), GetColorU32(ImGuiCol_PlotLines));
        }
    }

    // control points
    for (int i = 0; i < curve.Size(); i++) {
        ImVec2 p;
        p.x = curve.RawPositions()[i];
        p.y = curve.RawValues()[i];

        p.y = 1 - p.y;
        p.x = (p.x - offset_x) * mult_x;

        p = p * (bb.Max - bb.Min) + bb.Min;

        ImVec2 a = p - ImVec2(4, 4);
        ImVec2 b = p + ImVec2(4, 4);

        //            auto idx = curve.LeftPointIndexAtFraction(p.x);
        if (curve.IsSelected(i)) {
            window->DrawList->AddRectFilled(a - ImVec2(2, 2), b + ImVec2(2, 2), GetColorU32(IM_COL32(192, 255, 0, 255)));
        }
        if (IsMouseHoveringRect(a, b)) {
            window->DrawList->AddRectFilled(a - ImVec2(1, 1), b + ImVec2(1, 1), color);
        } else
            window->DrawList->AddRect(a, b, GetColorU32(ImGuiCol_PlotLinesHovered), 0, 0, 2);
    }
}

static inline void
__MainPopupMenu(ACurve& curve)
{
    using namespace ImGui;
    if (BeginPopup("PointPopup")) {
        if (Selectable("Select all"))
            curve.SelectAll();

        if (Selectable("Clear"))
            curve.InitConstant(.5);

        Separator();

        if (curve.SelectionSize()) {
            if (Selectable("Linear"))
                curve.SetSelectionTransitions("Linear");
            if (Selectable("Step"))
                curve.SetSelectionTransitions("Step");
            if (Selectable("Hold"))
                curve.SetSelectionTransitions("Hold");
            if (Selectable("SineInOut"))
                curve.SetSelectionTransitions("SineInOut");
            Separator();

            if (BeginMenu("All transitions ...")) {
                for (const auto& k : EaseFunctorFactory::Names()) {
                    if (Selectable(k.c_str())) {
                        curve.SetSelectionTransitions(k);
                        GetIO().MouseDown[0] = 0;
                        //                        printf("popup\n");
                    }
                }

                EndMenu();
            }
            Separator();

            if (Selectable("Unlock")) {
                curve.SetSelectionLocks(LockEdit::None);
            }

            if (BeginMenu("Lock ...")) {

                if (Selectable("Lock X")) {
                    curve.SetSelectionLocks(LockEdit::LockX);
                }
                if (Selectable("Lock Y")) {
                    curve.SetSelectionLocks(LockEdit::LockY);
                }
                if (Selectable("Lock Both")) {
                    curve.SetSelectionLocks(LockEdit::LockBoth);
                }
                EndMenu();
            }
        }
        EndPopup();
    }
}

static inline void
__Editorinteractions(ImGuiWindow* window, const ImRect& bb, const std::string& name, ACurve& curve, const float& range_x, const float& offset_x, const float& mult_x)
{
    using namespace ImGui;

    const bool hovered = IsItemHovered();;//IsMouseHoveringRect(bb.Min, bb.Max);
    const ImGuiID id = window->GetID(name.c_str());
    const float widgetWidth = bb.Max.x - bb.Min.x;
    int sel = -1;

    //
    if (hovered) {
        SetHoveredID(id);

        // find selected control point

        ImVec2 pos = (GetIO().MousePos - bb.Min) / (bb.Max - bb.Min);
        pos.y = 1 - pos.y;
        pos.x += 7. / widgetWidth;
        auto fract = ((pos.x * range_x) + offset_x);

        auto l_idx = curve.LeftPointIndexAtFraction(fract);

        if (l_idx >= 0) {
            ImVec2 p;

            p.x = curve.RawPositions()[l_idx];
            //        p.x = fract;
            p.y = curve.RawValues()[l_idx];

            p.y = 1 - p.y;
            p.x = (p.x - offset_x) * mult_x;

            p = p * (bb.Max - bb.Min) + bb.Min;

            ImVec2 a = p - ImVec2(7, 7);
            ImVec2 b = p + ImVec2(7, 7);

            if ((GetIO().MousePos.x > a.x) && (GetIO().MousePos.x < b.x))
                if ((GetIO().MousePos.y > a.y) && (GetIO().MousePos.y < b.y))
                    sel = l_idx;
        }

        // add - remove
        if (IsMouseClicked(ImGuiMouseButton_Left) && (!IsPopupOpen("PointPopup"))) {

            if (GetIO().KeyMods & ImGuiKeyModFlags_Shift) {
                //auto px =
                curve.AddPoint(fract - 7. / widgetWidth, pos.y);
                GetIO().MouseDown[0] = 0;
            }

            if (sel == -1) {
                curve.ClearSelection();
            }

            if (sel != -1) {
                if (curve.SelectionSize() == 1)
                    if (GetIO().KeyMods == ImGuiKeyModFlags_None)
                        curve.ClearSelection();
                curve.SelectPoint(sel);

                if (GetIO().KeyMods & ImGuiKeyModFlags_Alt) {
                    curve.DeleteSelection();
                }
            }
        }

        if (IsMouseClicked(ImGuiMouseButton_Right)) {
            OpenPopup("PointPopup");
        }

        // move
        ImVec2 dpos = GetMouseDragDelta() / (bb.Max - bb.Min);
        dpos.y *= -1;
        dpos.x *= range_x;

        curve.MoveSelectionTime(dpos.x);
        curve.MoveSelectionValue(dpos.y);

        ResetMouseDragDelta();
    }

    // add point lines crosshair
    if (GetIO().KeyMods & ImGuiKeyModFlags_Shift) {
        window->DrawList->AddLine(
            ImVec2(GetIO().MousePos.x, 0),
            ImVec2(GetIO().MousePos.x, bb.Max.y),
            GetColorU32(ImGuiCol_TextDisabled), 1);

        window->DrawList->AddLine(
            ImVec2(0, GetIO().MousePos.y),
            ImVec2(bb.Max.x, GetIO().MousePos.y),
            GetColorU32(ImGuiCol_TextDisabled), 1);
    }
}

// ---

bool ImWidget(const std::string& name, const ImVec2& size, ACurve& curve, const float& scroll, const float& zoomRange)
{
    ImVec2 vr;

    auto c_zoomRange = zoomRange;
    if (zoomRange < 0.001)
        c_zoomRange = 0.001;

    vr.x = scroll * (1 - c_zoomRange);
    vr.y = vr.x + (c_zoomRange); // * (1 - scroll));

    if (vr.x > 1)
        vr.x = 1;
    if (vr.x < 0)
        vr.x = 0;

    if (vr.y > 1)
        vr.y = 1;
    if (vr.y < 0)
        vr.y = 0;

    return ImWidget(name, size, curve, vr);
}

void ImWidgetListView(const std::string& name, ACurve& curve)
{
    using namespace ImGui;
    Begin(name.c_str());
    for (int i = 0; i < curve.Size(); i++) {
        auto l = Codec::ToString(curve.LockAt(i));
        Text("[%s%i] @ %f : %f %s lock: %s", (curve.IsSelected(i) ? "*" : ""), i, curve.TimeAt(i), curve.ValueAt(i), curve.TransitionAt(i).c_str(), l.c_str());
    }
    End();
}

bool ImWidget(const std::string& name, const ImVec2& size, ACurve& curve, const ImVec2& viewRange)
{
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    if (window->SkipItems)
        return false;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    //
    const float offset_x = viewRange.x;
    const float range_x = viewRange.y - viewRange.x;
    const float mult_x = (range_x > .0001) ? 1.0 / range_x : 0.0001;
    
    
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);

    PushClipRect(bb.Min, bb.Max, false);

    __DrawGrid(window, bb, range_x, offset_x, mult_x);
    __MainPopupMenu(curve);

    __Editorinteractions(window, bb, name, curve, range_x, offset_x, mult_x);

    auto COLOR = IM_COL32(192, 168, 0, 255);

    __DrawSmoothCurve(window, bb, curve, COLOR, offset_x, mult_x);
    __DrawEditorFeatures(window, bb, curve, COLOR, offset_x, mult_x);

    // info text
    char buf[128];
    const char* str = name.c_str();
    
    const bool hovered = IsItemHovered();;//IsMouseHoveringRect(bb.Min, bb.Max);
    if (hovered) {
        ImVec2 pos = (GetIO().MousePos - bb.Min) / (bb.Max - bb.Min);
        pos.y = 1 - pos.y;

        sprintf(buf, "%s (%f,%f)", name.c_str(), pos.x, pos.y);
        str = buf;
    }

    RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y + style.FramePadding.y), bb.Max, str, NULL, NULL); //, ImGuiAlign_Center);

    PopClipRect();

    return true;
}

bool ImWidgetOverview(const std::string& name, const ImVec2& size, ACurve& curve, float* scroll, float* zoomRange)
{
    using namespace ImGui;

    //

    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    const ImGuiID id = window->GetID(name.c_str());
    if (window->SkipItems)
        return false;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    const bool hovered = IsItemHovered();;//IsMouseHoveringRect(bb.Min, bb.Max);

    //
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);

    float wd = bb.Max.x - bb.Min.x;
    
    __DrawGrid(window, bb);
    
    if (hovered) {
        SetHoveredID(id);}

    __DrawSmoothCurve(window, bb, curve);

    // overview selection

    bool ret = false;

    if (hovered && (scroll != nullptr)) {
        if (IsMouseClicked(ImGuiMouseButton_Left) || IsMouseDragging(ImGuiMouseButton_Left))
            *scroll = ((GetIO().MousePos - bb.Min) / (bb.Max - bb.Min)).x;
        ret = true;
    }

    if (hovered && (zoomRange != nullptr)) {
        if (IsMouseClicked(ImGuiMouseButton_Left) || IsMouseDragging(ImGuiMouseButton_Left)) {
            auto v = 1 - ((GetIO().MousePos - bb.Min) / (bb.Max - bb.Min)).y;
            const auto coeff = 1.8;
            v = (v - (0.5 / coeff)) * coeff;
            if (v < 0)
                v = 0;
            if (v > 1)
                v = 1;
            
            *zoomRange = pow(v,.25);
        }
        ret = true;
    }

    ImVec2 s = ImVec2(bb.Min.x + wd * ((*scroll) * (1 - *zoomRange)), bb.Min.y);
    ImVec2 e = ImVec2(bb.Min.x + wd * (((*scroll) * (1 - *zoomRange)) + (*zoomRange)), bb.Max.y);

    window->DrawList->AddRectFilled(s, e, IM_COL32(192, 192, 192, 96));

    PopClipRect();

    return ret;
}

// ---

bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const float& scroll, const float& zoomRange)
{
    ImVec2 vr;

    auto c_zoomRange = zoomRange;
    if (zoomRange < 0.001)
        c_zoomRange = 0.001;

    vr.x = scroll * (1 - c_zoomRange);
    vr.y = vr.x + (c_zoomRange); // * (1 - scroll));

    if (vr.x > 1)
        vr.x = 1;
    if (vr.x < 0)
        vr.x = 0;

    if (vr.y > 1)
        vr.y = 1;
    if (vr.y < 0)
        vr.y = 0;

    return ImWidgetMulti(name, size, curve, vr);
}

bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, const ImVec2& viewRange){
        using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    if (window->SkipItems)
        return false;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    //
    const float offset_x = viewRange.x;
    const float range_x = viewRange.y - viewRange.x;
    const float mult_x = (range_x > .0001) ? 1.0 / range_x : 0.0001;
    
    
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);

    PushClipRect(bb.Min, bb.Max, false);

    __DrawGrid(window, bb, range_x, offset_x, mult_x);
    
    if (curve.HasActiveCurve()){
    __MainPopupMenu(curve.curves[curve.activeCurve]);

    __Editorinteractions(window, bb, name, curve.curves[curve.activeCurve], range_x, offset_x, mult_x);
    }
    
    for (auto& e: curve.curves){
        auto ec = curve.GetColor(e.first);
        auto COLOR = IM_COL32(ec.r, ec.g, ec.b, 255);

        __DrawSmoothCurve(window, bb, e.second, COLOR, offset_x, mult_x, int(curve.activeCurve.compare(e.first)==0) + 1);
        
        if (curve.activeCurve.compare(e.first)==0){
            __DrawEditorFeatures(window, bb, e.second, COLOR, offset_x, mult_x);
        }
    }
    

    // info text
    char buf[128];
    const char* str = name.c_str();
    
    const bool hovered = IsItemHovered();;//IsMouseHoveringRect(bb.Min, bb.Max);
    if (hovered) {
        ImVec2 pos = (GetIO().MousePos - bb.Min) / (bb.Max - bb.Min);
        pos.y = 1 - pos.y;

        sprintf(buf, "%s (%f,%f)", name.c_str(), pos.x, pos.y);
        str = buf;
    }

    RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y + style.FramePadding.y), bb.Max, str, NULL, NULL); //, ImGuiAlign_Center);

    PopClipRect();

    return true;
}

bool ImWidgetOverviewMulti(const std::string& name,const ImVec2& size,MultiCurve& curve, float* scroll , float* zoomRange ){

 using namespace ImGui;

    //

    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    const ImGuiID id = window->GetID(name.c_str());
    if (window->SkipItems)
        return false;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    const bool hovered = IsItemHovered();;// IsMouseHoveringRect(bb.Min, bb.Max);
    
    //
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);

    float wd = bb.Max.x - bb.Min.x;
    
    __DrawGrid(window, bb);
    
    if (hovered) {
        SetHoveredID(id);}

    for (auto& e: curve.curves){
        auto ec = curve.GetColor(e.first);
        auto COLOR = IM_COL32(ec.r, ec.g, ec.b, 255);

        __DrawSmoothCurve(window, bb, e.second, COLOR, 0, 1, 2);
        
        //if (curve.activeCurve.compare(e.first)==0){
        //    __DrawEditorFeatures(window, bb, e.second, COLOR, offset_x, mult_x);
       // }
    }
    
    //__DrawSmoothCurve(window, bb, curve);

    // overview selection

    bool ret = false;

    if (hovered && (scroll != nullptr)) {
        if (IsMouseClicked(ImGuiMouseButton_Left) || IsMouseDragging(ImGuiMouseButton_Left))
            *scroll = ((GetIO().MousePos - bb.Min) / (bb.Max - bb.Min)).x;
        ret = true;
    }

    if (hovered && (zoomRange != nullptr)) {
        if (IsMouseClicked(ImGuiMouseButton_Left) || IsMouseDragging(ImGuiMouseButton_Left)) {
            auto v = 1 - ((GetIO().MousePos - bb.Min) / (bb.Max - bb.Min)).y;
            const auto coeff = 1.8;
            v = (v - (0.5 / coeff)) * coeff;
            if (v < 0)
                v = 0;
            if (v > 1)
                v = 1;
            
            *zoomRange = pow(v,.25);
        }
        ret = true;
    }

    ImVec2 s = ImVec2(bb.Min.x + wd * ((*scroll) * (1 - *zoomRange)), bb.Min.y);
    ImVec2 e = ImVec2(bb.Min.x + wd * (((*scroll) * (1 - *zoomRange)) + (*zoomRange)), bb.Max.y);

    window->DrawList->AddRectFilled(s, e, IM_COL32(192, 192, 192, 96));

    PopClipRect();

    return ret;
    
}

};

    ;

