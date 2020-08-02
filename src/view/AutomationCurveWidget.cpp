// optional: imgui widget

#include "AutomationCurve.hpp"
#include "AutomationCurveWidget.hpp"

#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "math.h"

#include "WidgetCommon.hpp"

namespace AutomationCurve {

// NEW
inline bool
__Editorinteractions_2(ImGuiWindow* window, const ImRect& bb, const std::string& name, CurveEditor& editor, const AutomationMultiViewSettings& settings, const WidgetCoordinateConverter& converter)
{

    if (editor.curve == nullptr)
        return false;

    bool ret = false;

    using namespace ImGui;

    const bool hovered = IsItemHovered();
    const ImGuiID id = window->GetID(name.c_str());

    if (hovered) {
        SetHoveredID(id);
    }
    int sel = -1;

    // hovered
    if (hovered) {

        // find selected control point
        ImVec2 pos = (GetIO().MousePos - bb.Min); // / (bb.Max - bb.Min);

        pos.y /= (bb.Max.y - bb.Min.y);
        pos.y = 1 - pos.y;
        pos.x += 7.; // / widgetWidth;

        auto curveFract = converter.PixelToCurveFraction(pos.x);
        auto curveFract2 = converter.PixelToCurveFraction(pos.x - 7);

        auto l_idx = editor.curve->LeftPointIndexAtFraction(curveFract);

        if (l_idx >= 0) {
            ImVec2 p;

            // ??
            p.x = editor.curve->RawPositions()[l_idx];
            p.y = editor.curve->RawValues()[l_idx];

            p.y = 1 - p.y;

            p.x = converter.CurveFractionToPixel(p.x);

            p.y = p.y * (bb.Max.y - bb.Min.y) + bb.Min.y;
            p.x += bb.Min.x;

            ImVec2 a = p - ImVec2(7, 7);
            ImVec2 b = p + ImVec2(7, 7);

            if ((GetIO().MousePos.x > a.x) && (GetIO().MousePos.x < b.x))
                if ((GetIO().MousePos.y > a.y) && (GetIO().MousePos.y < b.y))
                    sel = l_idx;
        }

        // add - remove
        if (IsMouseClicked(ImGuiMouseButton_Left) && (!IsPopupOpen("PointPopup"))) {

            if (GetIO().KeyMods & ImGuiKeyModFlags_Shift) {
                editor.curve->AddPoint(curveFract2, pos.y); //  - 7. / widgetWidth
                GetIO().MouseDown[0] = 0;
            }

            if (sel == -1) {
                editor.ClearSelection();
            }

            if (sel != -1) {
                if (editor.SelectionSize() == 1)
                    if (GetIO().KeyMods == ImGuiKeyModFlags_None)
                        editor.ClearSelection();
                editor.SelectPoint(sel);

                if (GetIO().KeyMods & ImGuiKeyModFlags_Alt) {
                    editor.DeleteSelection();
                }

                ret = true;
            }
        }

        // menu
        if (IsMouseClicked(ImGuiMouseButton_Right)) {
            OpenPopup("PointPopup");
        }

        // move
        ImVec2 dpos = GetMouseDragDelta() / (bb.Max - bb.Min);
        dpos.y *= -1;
        // ?
        dpos.x *= settings.zoom;
    
        editor.MoveSelectionTime(dpos.x);
        editor.MoveSelectionValue(dpos.y);

        ResetMouseDragDelta();
        ret = true;
        
        //    // add point lines crosshair
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

    return ret;
}

// ---
static inline void
__MainPopupMenu(CurveEditor& editor)
{
    if (editor.curve == nullptr)
        return;

    using namespace ImGui;

    if (BeginPopup("PointPopup")) {
        if (Selectable("Select all"))
            editor.SelectAll();

        if (Selectable("Clear"))
            editor.curve->InitConstant(.5);

        Separator();

        if (editor.SelectionSize()) {
            if (Selectable("Linear"))
                editor.SetSelectionTransitions("Linear");
            if (Selectable("Step"))
                editor.SetSelectionTransitions("Step");
            if (Selectable("Hold"))
                editor.SetSelectionTransitions("Hold");
            if (Selectable("SineInOut"))
                editor.SetSelectionTransitions("SineInOut");
            Separator();

            if (BeginMenu("All transitions ...")) {
                for (const auto& k : EaseFunctorFactory::Names()) {
                    if (Selectable(k.c_str())) {
                        editor.SetSelectionTransitions(k);
                        GetIO().MouseDown[0] = 0;
                        //                        printf("popup\n");
                    }
                }

                EndMenu();
            }
            Separator();

            if (Selectable("Unlock")) {
                editor.SetSelectionLocks(LockEdit::None);
            }

            if (BeginMenu("Lock ...")) {

                if (Selectable("Lock X")) {
                    editor.SetSelectionLocks(LockEdit::LockX);
                }
                if (Selectable("Lock Y")) {
                    editor.SetSelectionLocks(LockEdit::LockY);
                }
                if (Selectable("Lock Both")) {
                    editor.SetSelectionLocks(LockEdit::LockBoth);
                }
                EndMenu();
            }

            Separator();
        }

        if (BeginMenu("Cycle Left")) {

            if (Selectable((editor.curve->CycleLeft() == CycleType::Hold) ? "* Hold" : "Hold"))
                editor.curve->SetCycleLeft(CycleType::Hold);
            if (Selectable((editor.curve->CycleLeft() == CycleType::Zero) ? "* Zero" : "Zero"))
                editor.curve->SetCycleLeft(CycleType::Zero);
            if (Selectable((editor.curve->CycleLeft() == CycleType::Repeat) ? "* Repeat" : "Repeat"))
                editor.curve->SetCycleLeft(CycleType::Repeat);
            // yet disabled:
            //                if (Selectable((editor.curve->CycleLeft() == CycleType::Mirror) ? "* Mirror" : "Mirror"))
            //                    editor.curve->SetCycleLeft(CycleType::Mirror);
            EndMenu();
        }

        if (BeginMenu("Cycle Right")) {
            if (Selectable((editor.curve->CycleRight() == CycleType::Hold) ? "* Hold" : "Hold"))
                editor.curve->SetCycleRight(CycleType::Hold);
            if (Selectable((editor.curve->CycleRight() == CycleType::Zero) ? "* Zero" : "Zero"))
                editor.curve->SetCycleRight(CycleType::Zero);
            if (Selectable((editor.curve->CycleRight() == CycleType::Repeat) ? "* Repeat" : "Repeat"))
                editor.curve->SetCycleRight(CycleType::Repeat);
            //                if (Selectable((editor.curve->CycleRight() == CycleType::Mirror) ? "* Mirror" : "Mirror"))
            //                    editor.curve->SetCycleRight(CycleType::Mirror);
            EndMenu();
        }

        EndPopup();
    }
}

static inline bool
__Editorinteractions(ImGuiWindow* window, const ImRect& bb, const std::string& name, CurveEditor& editor, const float& range_x, const float& offset_x, const float& mult_x, float timeOffset = 0, float timeScale = 1, float timeScaleSingle = 1)
{
    if (editor.curve == nullptr)
        return false;

    bool ret = false;

    using namespace ImGui;

    const bool hovered = IsItemHovered();
    ; //IsMouseHoveringRect(bb.Min, bb.Max);
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

        //        fract -= timeOffset / timeScaleSingle;

        fract = fract * timeScale;
        fract -= timeOffset * timeScaleSingle;

        auto l_idx = editor.curve->LeftPointIndexAtFraction(fract);

        fract += timeOffset * timeScaleSingle;

        if (l_idx >= 0) {
            ImVec2 p;

            p.x = editor.curve->RawPositions()[l_idx];
            p.y = editor.curve->RawValues()[l_idx];

            p.y = 1 - p.y;
            p.x = (p.x - offset_x) * mult_x;

            p.x += timeOffset;
            p.x = p.x / timeScale;

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
                editor.curve->AddPoint(fract - 7. / widgetWidth, pos.y);
                GetIO().MouseDown[0] = 0;
            }

            if (sel == -1) {
                editor.ClearSelection();
            }

            if (sel != -1) {
                if (editor.SelectionSize() == 1)
                    if (GetIO().KeyMods == ImGuiKeyModFlags_None)
                        editor.ClearSelection();
                editor.SelectPoint(sel);

                if (GetIO().KeyMods & ImGuiKeyModFlags_Alt) {
                    editor.DeleteSelection();
                }

                ret = true;
            }
        }

        if (IsMouseClicked(ImGuiMouseButton_Right)) {
            OpenPopup("PointPopup");
        }

        // move
        ImVec2 dpos = GetMouseDragDelta() / (bb.Max - bb.Min);
        dpos.y *= -1;
        dpos.x *= range_x;
        dpos.x /= timeScale;

        editor.MoveSelectionTime(dpos.x);
        editor.MoveSelectionValue(dpos.y);

        ResetMouseDragDelta();
        ret = true;
    }

    // add point lines crosshair
    if (IsItemHovered()) {
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

    return ret;
}

// ---

bool ImWidget(const std::string& name, const ImVec2& size, CurveEditor& curve, const float& scroll, const float& zoomRange)
{
    if (curve.curve == nullptr)
        return false;

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

bool ImWidgetListView(const std::string& name, CurveEditor& editor)
{
    if (editor.curve == nullptr)
        return false;

    bool ret = false;

    using namespace ImGui;

    int numColumns = 7;
    Columns(numColumns);

    SetColumnWidth(0, 30);
    SetColumnWidth(1, 60);
    SetColumnWidth(2, 60);
    SetColumnWidth(3, 30);
    SetColumnWidth(4, 30);
    SetColumnWidth(5, 120);
    auto removeIdx = -1;

    for (int i = 0; i < editor.curve->Size(); i++) {
        auto l = Codec::ToString(editor.curve->LockAt(i));

        TextUnformatted(((editor.IsSelected(i) ? "*" : "") + std::to_string(i)).c_str());
        NextColumn();

        float t = editor.curve->TimeAt(i);

        if (DragFloat(("##list_time" + std::to_string(i)).c_str(), &t, .01, 0, 1)) {
            editor.curve->SetTime(i, t);
            ret = true;
        }
        NextColumn();

        float val = editor.curve->ValueAt(i);

        if (DragFloat(("##list_value" + std::to_string(i)).c_str(), &val, .01, 0, 1)) {
            editor.curve->SetValue(i, val);
            ret = true;
        }
        NextColumn();

        if (Button(("+##list_add" + std::to_string(i)).c_str())) {
            auto idx = (i == (editor.curve->Size() - 1)) ? (editor.curve->Size() - 2) : i;

            auto t = (editor.curve->TimeAt(idx) + editor.curve->TimeAt(idx + 1)) * .5;
            auto v = editor.curve->ValueAtFraction(t);

            editor.curve->AddPoint(t, v);
            ret = true;
        }
        NextColumn();

        if (Button(("-##list_remove" + std::to_string(i)).c_str())) {
            removeIdx = i;
            ret = true;
        }
        NextColumn();

        if (BeginCombo(("##list_transition" + std::to_string(i)).c_str(), editor.curve->TransitionAt(i).c_str())) {
            for (const auto& k : EaseFunctorFactory::Names()) {
                if (Selectable(k.c_str())) {
                    //                        editor.SetSelectionTransitions(k);
                    editor.curve->SetTransitionToPoint(i, k);
                }
            }
            EndCombo();
            ret = true;
        }
        NextColumn();

        auto lock = Codec::ToString(editor.curve->LockAt(i));

        if ((i > 0) && (i != (editor.curve->Size() - 1))) {
            if (BeginCombo(("##list_locks" + std::to_string(i)).c_str(), lock.c_str())) {
                if (Selectable("None")) {
                    editor.curve->SetPointLock(i, LockEdit::None);
                    ret = true;
                }
                if (Selectable("Lock X")) {
                    editor.curve->SetPointLock(i, LockEdit::LockX);
                    ret = true;
                }
                if (Selectable("Lock Y")) {
                    editor.curve->SetPointLock(i, LockEdit::LockY);
                    ret = true;
                }
                if (Selectable("Lock Both")) {
                    editor.curve->SetPointLock(i, LockEdit::LockBoth);
                    ret = true;
                }

                EndCombo();
            }
        }

        NextColumn();
        Separator();
    }

    if (removeIdx != -1)
        editor.curve->RemovePointAt(removeIdx);

    return ret;
}

bool ImWidget(const std::string& name, const ImVec2& size, CurveEditor& curve, const ImVec2& viewRange)
{
    if (curve.curve == nullptr)
        return false;
    using namespace ImGui;

    bool ret = false;

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

    ret = __Editorinteractions(window, bb, name, curve, range_x, offset_x, mult_x);

    auto COLOR = IM_COL32(192, 168, 0, 255);

    __DrawSmoothCurve(window, bb, *curve.curve, COLOR, offset_x, mult_x);
    __DrawEditorFeatures(window, bb, curve, COLOR, offset_x, mult_x);

    // info text
    char buf[128];
    const char* str = name.c_str();
    const bool hovered = IsItemHovered();
    if (hovered) {
        ImVec2 pos = (GetIO().MousePos - bb.Min) / (bb.Max - bb.Min);
        pos.y = 1 - pos.y;

        sprintf(buf, "%s (%f,%f)", name.c_str(), pos.x, pos.y);
        str = buf;
    }
    RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y + style.FramePadding.y), bb.Max, str, NULL, NULL); //, ImGuiAlign_Center);

    PopClipRect();

    return ret;
}

bool ImWidgetOverview(const std::string& name, const ImVec2& size, CurveEditor& editor, float* scroll, float* zoomRange)
{
    if (editor.curve == nullptr)
        return false;
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

    const bool hovered = IsItemHovered();
    ; //IsMouseHoveringRect(bb.Min, bb.Max);

    //
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);

    float wd = bb.Max.x - bb.Min.x;

    __DrawGrid(window, bb);

    if (hovered) {
        SetHoveredID(id);
    }

    __DrawSmoothCurve(window, bb, *editor.curve);

    // overview selection

    bool ret = false;

    // old mode (alt):
    if (GetIO().KeyMods == ImGuiKeyModFlags_Alt) {
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

                *zoomRange = pow(v, .25);
            }
            ret = true;
        }
    }

    // new mode:

    // TODO

    ImVec2 s = ImVec2(bb.Min.x + wd * ((*scroll) * (1 - *zoomRange)), bb.Min.y);
    ImVec2 e = ImVec2(bb.Min.x + wd * (((*scroll) * (1 - *zoomRange)) + (*zoomRange)), bb.Max.y);

    window->DrawList->AddRectFilled(s, e, IM_COL32(192, 192, 192, 96));

    PopClipRect();

    return ret;
}

// ---

// NEW
bool ImWidgetMulti(const std::string& name, MultiCurve& multiCurve, const AutomationMultiViewSettings& settings)
{

    using namespace ImGui;

    bool ret = false;

    // common
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    if (window->SkipItems)
        return false;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + settings.size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);
    

    // converter
    AutomationCurve::WidgetCoordinateConverter coordConverter;

    coordConverter.widgetWidth = settings.size.x;
    coordConverter.scrollOffset = settings.scroll;
    coordConverter.zoomValue = settings.zoom; //viewRange.y - settings.viewRange.x;
    coordConverter.timeOffset = multiCurve.GetMinTimeOffset();
    coordConverter.timeScale = multiCurve.GetMaxTimeScale();
    
    //temp
    coordConverter.curveTimeOffset = coordConverter.timeOffset;
    coordConverter.curveTimeScale = coordConverter.timeScale;
    
    __DrawGrid_2(window, bb, settings, coordConverter);
    
    if (multiCurve.HasActiveCurve()) {
        __MainPopupMenu(multiCurve.editor);
        
        coordConverter.curveTimeOffset = multiCurve.ActiveCurve()->TimeOffset();
        coordConverter.curveTimeScale = multiCurve.ActiveCurve()->TimeScale();
        ret = __Editorinteractions_2(window, bb, name, multiCurve.editor, settings, coordConverter);
        
    } else {
        // empty menu
        if (BeginPopup("EmptyMenu")) {
            Text("Nothing selected");
            EndPopup();
        }

        if (IsItemHovered() && IsItemClicked(1)) {
            OpenPopup("EmptyMenu");
        }
    }

    for (auto& e : multiCurve.curves) {
        auto ec = multiCurve.GetColor(e.first);
        auto COLOR = IM_COL32(ec.r, ec.g, ec.b, 255);

        // TODO
        coordConverter.curveTimeOffset = e.second->TimeOffset();
        coordConverter.curveTimeScale = e.second->TimeScale();
        
        

        __DrawSmoothCurve_2(window, bb, *e.second, COLOR, settings, coordConverter);

        if (multiCurve.ActiveCurveName().compare(e.first) == 0) {

            // TODO
            coordConverter.curveTimeOffset = multiCurve.ActiveCurve()->TimeOffset();
            coordConverter.curveTimeScale = multiCurve.ActiveCurve()->TimeScale();
            __DrawEditorFeatures_2(window, bb, multiCurve.editor, COLOR, settings, coordConverter);
        }
    }

    // info text
    char buf[128];
    const char* str = name.c_str();

    const bool hovered = IsItemHovered();
    ; //IsMouseHoveringRect(bb.Min, bb.Max);
    if (hovered) {
        ImVec2 pos = (GetIO().MousePos - bb.Min) / (bb.Max - bb.Min);
        pos.y = 1 - pos.y;

        sprintf(buf, "%s (%f,%f)", name.c_str(), pos.x, pos.y);
        str = buf;
    }

    RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y + style.FramePadding.y), bb.Max, str, NULL, NULL); //

    PopClipRect();

    return ret;
}

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

bool ImWidgetMulti(const std::string& name, const ImVec2& size, MultiCurve& multiCurve, const ImVec2& viewRange)
{
    using namespace ImGui;

    bool ret = false;

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

    // converter
    AutomationCurve::WidgetCoordinateConverter coordConverter;

    coordConverter.widgetWidth = size.x;
    coordConverter.scrollOffset = viewRange.x;
    coordConverter.zoomValue = viewRange.y - viewRange.x;
    coordConverter.timeOffset = multiCurve.GetMinTimeOffset();
    coordConverter.timeScale = multiCurve.GetMaxTimeScale();

    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);

    __DrawGrid(window, bb, range_x, offset_x, mult_x);

    if (multiCurve.HasActiveCurve()) {
        __MainPopupMenu(multiCurve.editor);

        // TODO
        ret = __Editorinteractions(window, bb, name, multiCurve.editor, range_x, offset_x, mult_x, multiCurve.editor.curve->TimeOffset() - multiCurve.GetMinTimeOffset(), multiCurve.GetMaxTimeScale() / multiCurve.editor.curve->TimeScale(), multiCurve.editor.curve->TimeScale());
    } else {
        // empty menu
        if (BeginPopup("EmptyMenu")) {
            Text("Nothing selected");
            EndPopup();
        }

        if (IsItemHovered() && IsItemClicked(1)) {
            OpenPopup("EmptyMenu");
        }
    }

    for (auto& e : multiCurve.curves) {
        auto ec = multiCurve.GetColor(e.first);
        auto COLOR = IM_COL32(ec.r, ec.g, ec.b, 255);

        // TODO
        __DrawSmoothCurve_Timed(window, bb, *e.second, COLOR, offset_x, mult_x, int(multiCurve.ActiveCurveName().compare(e.first) == 0) + 1, 256, multiCurve.GetMinTimeOffset(), multiCurve.GetMaxTimeScale());

        if (multiCurve.ActiveCurveName().compare(e.first) == 0) {

            // TODO
            __DrawEditorFeatures_Timed(window, bb, multiCurve.editor, COLOR, offset_x, mult_x, e.second->TimeOffset() - multiCurve.GetMinTimeOffset(), multiCurve.GetMaxTimeScale() / e.second->TimeScale(), e.second->TimeScale());
        }
    }

    // info text
    char buf[128];
    const char* str = name.c_str();

    const bool hovered = IsItemHovered();
    ; //IsMouseHoveringRect(bb.Min, bb.Max);
    if (hovered) {
        ImVec2 pos = (GetIO().MousePos - bb.Min) / (bb.Max - bb.Min);
        pos.y = 1 - pos.y;

        sprintf(buf, "%s (%f,%f)", name.c_str(), pos.x, pos.y);
        str = buf;
    }

    RenderTextClipped(ImVec2(bb.Min.x, bb.Min.y + style.FramePadding.y), bb.Max, str, NULL, NULL); //, ImGuiAlign_Center);

    PopClipRect();

    return ret;
}

bool ImWidgetOverviewMulti(const std::string& name, const ImVec2& size, MultiCurve& curve, float* scroll, float* zoomRange)
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

    const bool hovered = IsItemHovered();
    ; // IsMouseHoveringRect(bb.Min, bb.Max);

    //
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);

    float wd = bb.Max.x - bb.Min.x;

    __DrawGrid(window, bb);

    if (hovered) {
        SetHoveredID(id);
    }

    for (auto& e : curve.curves) {
        auto ec = curve.GetColor(e.first);
        auto COLOR = IM_COL32(ec.r, ec.g, ec.b, 255);

        __DrawSmoothCurve_Timed(window, bb, *e.second, COLOR, 0, 1, 2, 256, curve.GetMinTimeOffset(), curve.GetMaxTimeScale());

        //        __DrawSmoothCurve(window, bb, *e.second, COLOR, 0, 1, 2, 256);

        //if (editor.curve->activeCurve.compare(e.first)==0){
        //    __DrawEditorFeatures(window, bb, e.second, COLOR, offset_x, mult_x);
        // }
    }

    // overview selection

    bool ret = false;

    // old
    if (GetIO().KeyMods == ImGuiKeyModFlags_Alt) {
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

                *zoomRange = pow(v, .25);
            }
            ret = true;
        }
    }

    if (GetIO().KeyMods == ImGuiKeyModFlags_Shift) {
        if (hovered && (scroll != nullptr) && (zoomRange != nullptr) && IsMouseDragging(ImGuiMouseButton_Left)) {
            *scroll += GetMouseDragDelta().x / (bb.Max.x - bb.Min.x);
            ResetMouseDragDelta();
            if (*scroll < 0)
                *scroll = 0;
        }
    }

    // new
    if (GetIO().KeyMods == ImGuiKeyModFlags_None) {
        if (hovered && (scroll != nullptr) && (zoomRange != nullptr) && IsMouseClicked(ImGuiMouseButton_Left && !IsMouseDragging(ImGuiMouseButton_Left))) {
            *scroll = ((GetIO().MousePos - bb.Min) / (bb.Max - bb.Min)).x;
            *zoomRange = .01;
            printf("click\n");
        }

        if (hovered && (scroll != nullptr) && (zoomRange != nullptr) && IsMouseDragging(ImGuiMouseButton_Left)) {
            //            *scroll = ((GetIO().MousePos - bb.Min) / (bb.Max - bb.Min)).x;
            auto v = ((GetIO().MousePos - GetIO().MouseClickedPos[ImGuiMouseButton_Left] /*- bb.Min*/) / (bb.Max - bb.Min)).x; // - *scroll;

            printf("v %f\n", v);

            v *= 1.2;
            if (v < 0)
                v = 0;
            if (v > 1)
                v = 1;

            *zoomRange = v; //pow(v, .25);
            auto invZ = 1 - *zoomRange;
            if (invZ < 0.001)
                invZ = 0.001;
            if (invZ > 1)
                invZ = 1;

            *scroll = ((GetIO().MouseClickedPos[0] - bb.Min) / (bb.Max - bb.Min)).x / invZ;

            //            GetIO().Mouse
        }

        if (hovered && (scroll != nullptr) && (zoomRange != nullptr) && IsMouseDoubleClicked(0)) {
            *scroll = 0;
            *zoomRange = 1;
        }
    }

    ImVec2 s = ImVec2(bb.Min.x + wd * ((*scroll) * (1 - *zoomRange)), bb.Min.y);
    ImVec2 e = ImVec2(bb.Min.x + wd * (((*scroll) * (1 - *zoomRange)) + (*zoomRange)), bb.Max.y);

    window->DrawList->AddRectFilled(s, e, IM_COL32(192, 192, 192, 96));

    PopClipRect();

    return ret;
}

// ---
};
