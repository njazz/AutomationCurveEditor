#include "WidgetCommon.hpp"
#include "AutomationCurve.hpp"

namespace AutomationCurve {

void __DrawGrid(ImGuiWindow* window, const ImRect& bb, const float& range_x , const float& offset_x , const float& mult_x )
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

void __DrawSmoothCurve(ImGuiWindow* window, const ImRect& bb, ACurve& curve, const ImU32& color, const float& offset_x , const float& mult_x , int lineWidth , int nPoints )
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

void __DrawEditorFeatures(ImGuiWindow* window, const ImRect& bb, CurveEditor& editor, const ImU32& color, const float& offset_x, const float& mult_x)
{
    using namespace ImGui;

    if (editor.curve == nullptr)
        return;

    // locks
    for (int i = 1; i < editor.curve->Size(); i++) {
        ImVec2 a;
        a.x = editor.curve->RawPositions()[i - 1];
        a.y = editor.curve->RawValues()[i - 1];
        ImVec2 b;
        b.x = editor.curve->RawPositions()[i];
        b.y = editor.curve->RawValues()[i];

        a.y = 1 - a.y;
        b.y = 1 - b.y;

        a.x = (a.x - offset_x) * mult_x;
        b.x = (b.x - offset_x) * mult_x;

        a = a * (bb.Max - bb.Min) + bb.Min;
        b = b * (bb.Max - bb.Min) + bb.Min;

        //x

        if ((editor.curve->LockAt(i) == LockEdit::LockX) || (editor.curve->LockAt(i) == LockEdit::LockBoth)) {
            window->DrawList->AddLine(ImVec2(b.x, 0), ImVec2(b.x, bb.Max.y), GetColorU32(ImGuiCol_PlotLines));
        }
        // y
        if ((editor.curve->LockAt(i) == LockEdit::LockY) || (editor.curve->LockAt(i) == LockEdit::LockBoth)) {
            window->DrawList->AddLine(ImVec2(0, b.y), ImVec2(bb.Max.x, b.y), GetColorU32(ImGuiCol_PlotLines));
        }
    }

    // control points
    for (int i = 0; i < editor.curve->Size(); i++) {
        ImVec2 p;
        p.x = editor.curve->RawPositions()[i];
        p.y = editor.curve->RawValues()[i];

        p.y = 1 - p.y;
        p.x = (p.x - offset_x) * mult_x;

        p = p * (bb.Max - bb.Min) + bb.Min;

        ImVec2 a = p - ImVec2(4, 4);
        ImVec2 b = p + ImVec2(4, 4);

        //            auto idx = editor.curve->LeftPointIndexAtFraction(p.x);
        if (editor.IsSelected(i)) {
            window->DrawList->AddRectFilled(a - ImVec2(2, 2), b + ImVec2(2, 2), GetColorU32(IM_COL32(192, 255, 0, 255)));
        }
        if (IsMouseHoveringRect(a, b)) {
            window->DrawList->AddRectFilled(a - ImVec2(1, 1), b + ImVec2(1, 1), color);
        } else
            window->DrawList->AddRect(a, b, GetColorU32(ImGuiCol_PlotLinesHovered), 0, 0, 2);
    }
}
}
