
#include "AutomationCurve.hpp"

#include <algorithm>

namespace AutomationCurve{

void CurveEditor::SelectPoint(const size_t idx)
{
    if (!curve) return;
    
    if (!IsSelected(idx))
        _selectionIdx.push_back(idx);
}
void CurveEditor::DeselectPoint(const size_t idx)
{
    if (!curve) return;
    
    if (std::find(_selectionIdx.begin(), _selectionIdx.end(), idx) == _selectionIdx.end())
        return;
    _selectionIdx.erase(std::remove(_selectionIdx.begin(), _selectionIdx.end(), idx));
}
void CurveEditor::ClearSelection()
{
    if (!curve) return;
    
    _selectionIdx.clear();
}

void CurveEditor::SelectAll()
{
    if (!curve) return;
    
    _selectionIdx.clear();
    for (int i = 0; i < curve->Size(); i++)
        _selectionIdx.push_back(i);
}

bool CurveEditor::IsSelected(const size_t idx)
{
    if (!curve) return false;
    
    return std::find(_selectionIdx.begin(), _selectionIdx.end(), idx) != _selectionIdx.end();
}

void CurveEditor::MoveSelectionValue(const float& v)
{
    if (!curve) return;
    
    for (const auto& idx : _selectionIdx)
        curve->SetValue(idx, curve->ValueAt(idx) + v);
}
void CurveEditor::MoveSelectionTime(const float& v)
{
    if (!curve) return;
    
    for (const auto& idx : _selectionIdx)
        curve->SetTime(idx, curve->TimeAt(idx) + v);
}

void CurveEditor::SetSelectionTransitions(const std::string& t)
{
    if (!curve) return;
    
    for (const auto& idx : _selectionIdx)
       curve-> SetTransitionToPoint(idx, t);
}

void CurveEditor::SetSelectionLocks(const LockEdit& l)
{
    if (!curve) return;
    
    for (const auto& idx : _selectionIdx)
       curve-> SetPointLock(idx, l);
}

void CurveEditor::DeleteSelection()
{
    if (!curve) return;
    
    // TODO: clean
    for (int i = (curve->Size() - 1); i >= 0; i--) {
        if (IsSelected(i))
           curve-> RemovePointAt(i);
    }

    ClearSelection();
}

}
