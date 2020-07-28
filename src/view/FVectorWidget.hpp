#include "imgui.h"
#include "FVector.hpp"
#include "ACurve.hpp"

namespace AutomationCurve {
  ImWidgetBars(const std::string& name, const ImVec2& size, FVector& vec, const ImVec2& viewRange);
  ImWidgetSlices(const std::string& name, const ImVec2& size, FVector& vec, const ImVec2& viewRange);
  
  // draw curve values. replace time or equal
  ImWidgetBars(const std::string& name, const ImVec2& size, FVector& vec, const ImVec2& viewRange);
  ImWidgetSlices(const std::string& name, const ImVec2& size, FVector& vec, const ImVec2& viewRange);
};
