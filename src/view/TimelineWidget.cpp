#include "math.h"

#include "TimelineWidget.hpp"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <string>

#include <vector>

namespace AutomationCurve {
void ImWidgetTimeline()
{
}

template <typename TF>
static inline TF clamp(TF v, TF v_Min, TF v_Max)
{
    return (v < v_Min) ? v_Min : ((v > v_Max) ? v_Max : v);
}

void ImWidgetMeter(const float& v, const MeterViewSettings& settings)
{
    std::vector<float> vec = {v};
    ImWidgetMeter(vec, settings);
}
void ImWidgetMeter(const std::vector<float>& vl, const MeterViewSettings& settings)
{

    //
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GetStyle();
    if (window->SkipItems)
        return;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + settings.size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;
    bool hovered = IsItemHovered();
    // hover
    const ImGuiID id = window->GetID(std::to_string((uintptr_t)&vl).c_str());
    if (hovered) {
        SetHoveredID(id);
    }

    //

    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
    PushClipRect(bb.Min, bb.Max, false);

    auto range = (settings.dbRangeMax - settings.dbRangeMin);

    //
    auto h = (vl.size()) ? (bb.Max.y - bb.Min.y) / vl.size() : (bb.Max.y - bb.Min.y);
    int idx = 0;
    float db0 = settings.dbRangeMin;
    for (const auto& v : vl) {
        auto db = 10 * log10(abs(v));
        db = clamp(db, settings.dbRangeMin, settings.dbRangeMax);
        if (db>db0) db0 = db;

        auto fract = 1 + (db / range);

        window->DrawList->AddRectFilled(ImVec2(bb.Min.x, idx * h+bb.Min.y), ImVec2((bb.Max.x - bb.Min.x) * fract + bb.Min.x, (idx + 1) * h+bb.Min.y), IM_COL32(0, 160, 32, 255));

        idx++;
    }

    // db0 /= (vl.size()) ? vl.size() : 1;

    for (int i = 0; i < (range / settings.gridStep); i++) {
        auto fract2 = float(i + 1) / (range / settings.gridStep) * settings.size.x;
        window->DrawList->AddLine(bb.Min + ImVec2(fract2, 0), bb.Min + ImVec2(fract2, 5), IM_COL32_WHITE); //32(0, 192, 24, 255));
    }

    auto pos = GetCursorPos();
    SetCursorPos(bb.Min - GetWindowPos() + ImVec2(bb.Max.x - bb.Min.x - 32, 9)); //ImVec2(0,0));
    Text("%i", int(db0));

    PopClipRect();
    SetCursorPos(pos);
}
};

// old:

//
//  VESeqTimeline.cpp
//  nseq
//
//  Created by Alex on 13/05/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

//#include "VESeqTimeline.hpp"
//
//#include "_timeline.h"
//
//#include "_nType_t.hpp"
//
//void VESeqTimeline::_drawTempoTimeline()
//{
//    _ttimeline* tl = (_ttimeline*)dataValueObject;
//
//    float tempo = 120;
//    int tsig[2] = { 4, 4 };
//    if (tl) {
//
//        tempo = tl->property<_tfloat>("Tempo")->value();
//        tsig[0] = tl->property<_tintvec<2> >("Time Signature")->value().v[0];
//        tsig[1] = tl->property<_tintvec<2> >("Time Signature")->value().v[1];
//    }
//
//    //
//    if (doScroll) {
//        ImGui::SetScrollX(scroll);
//        ImGui::GetCurrentWindow()->Scroll.x = scroll;
//        doScroll = false;
//    }
//
//    //
//    if (scroll != ImGui::GetScrollX()) {
//        scroll = ImGui::GetScrollX();
//        scrollAction.userObject = (void*)this;
//        if (!scrollLock)
//            scrollAction();
//        scrollLock = false;
//    }
//    //
//
//    ImGui::SetCursorPos(ImVec2(2, 0));
//    if (tl)
//        _gridSize = 5 + 30 * tl->property<_tfloat>("_zoom")->value();
//
//    float _tempoGridSize = _gridSize / (tempo / (float(tsig[1] / 4.0f)) / 60.0f); //* float(tsig[1]);//float(tsig[1]) * float(tsig[0]);
//    if (_tempoGridSize < 3)
//        _tempoGridSize = 3;
//
//    const int draw_width = 32000;
//    const ImU32 _color = IM_COL32(200, 200, 200, 40);
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    ImVec2 scrolling;
//    ImVec2 win_pos = ImGui::GetCursorScreenPos();
//    ImVec2 canvas_sz = ImVec2(draw_width, maxHeight()); //ImGui::GetWindowSize();
//    //canvas_sz.x = ImGui::GetWindowContentRegionWidth();
//    for (float x = fmodf(scrolling.x, _tempoGridSize); x < canvas_sz.x; x += _tempoGridSize)
//        draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, canvas_sz.y + win_pos.y), _color);
//
//    ImGui::SetCursorPos(ImVec2(4, 0));
//    ImGui::Text("%02.2f BPM", tempo);
//
//    int w = ImGui::CalcTextSize("0.0").x + 2;
//    int c = (ceil(w / _tempoGridSize) + 1) * _tempoGridSize;
//    if (c < _tempoGridSize)
//        c = _tempoGridSize;
//
//    // todo: width
//    for (int i = 0; i < draw_width; i += c) {
//        ImGui::SetCursorPos(ImVec2(i + 2, 20));
//
//        int bar = (i / _tempoGridSize) / (tsig[0]) + 1;
//        int beat = int(i / _tempoGridSize) % (tsig[0]) + 1;
//        ImGui::Text("%i.%i", bar, beat);
//    }
//}
//
//void VESeqTimeline::_drawSecondsTimeline()
//{
//    _ttimeline* tl = (_ttimeline*)dataValueObject;
//
//    if (tl)
//        _gridSize = 5 + 30 * tl->property<_tfloat>("_zoom")->value();
//
//    //
//    if (doScroll) {
//        ImGui::SetScrollX(scroll);
//        ImGui::GetCurrentWindow()->Scroll.x = scroll;
//        doScroll = false;
//    }
//
//    //
//    if (scroll != ImGui::GetScrollX()) {
//        scroll = ImGui::GetScrollX();
//        scrollAction.userObject = (void*)this;
//        if (!scrollLock)
//            scrollAction();
//        scrollLock = false;
//    }
//    //
//
//    ImGui::SetCursorPos(ImVec2(2, 0));
//    const int draw_width = 32000;
//    const ImU32 _color = IM_COL32(200, 200, 200, 40);
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    ImVec2 scrolling; // = ImVec2(offset,0);
//    ImVec2 win_pos = ImGui::GetCursorScreenPos();
//    ImVec2 canvas_sz = ImVec2(draw_width, maxHeight()); //ImGui::GetWindowSize();
//    //canvas_sz.x = ImGui::GetWindowContentRegionWidth();
//    for (float x = fmodf(scrolling.x, _gridSize); x < canvas_sz.x; x += _gridSize)
//        draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, canvas_sz.y + win_pos.y), _color);
//
//    //ImGui::Text("scroll: %f", s2);
//
//    //    int _gridSize = 20;
//
//    int w = ImGui::CalcTextSize("0:00").x + 2;
//    int c = (ceil(w / _gridSize) + 1) * _gridSize;
//    if (c < _gridSize)
//        c = _gridSize;
//
//    // todo: width
//    for (int i = 0; i < draw_width; i += c) {
//        ImGui::SetCursorPos(ImVec2(i + 4, 0));
//
//        int m = (i / _gridSize) / 60;
//        int s = int(i / _gridSize) % 60;
//        ImGui::Text("%i:%02i", m, s);
//    }
//}
//
//// -----
//
//float VESeqTimeline::minHeight()
//{
//    return 28 + 32 * !fold();
//}
//
//void VESeqTimeline::_drawContents()
//{
//    height = 75;
//    SeqDataView::_drawContents();
//
//    bool tempo = false;
//    if (dataValueObject)
//        if (dataValueObject->property<_tbool>("Tempo Timebase")->value())
//            tempo = true;
//
//    if (tempo)
//        _drawTempoTimeline();
//    else
//        _drawSecondsTimeline();
//
//    // subviews
//    _drawInletViews(dataValueObject);
//
//    /*
//     if (this->dataObject->inletObjectAt(1))
//     _drawInletViews(this->dataObject->inletObjectAt(1));
//     */
//}

