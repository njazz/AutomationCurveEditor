#include "AutomationCurve.hpp"

namespace AutomationCurve {
namespace JSONCodec {
     void ToJSONString(const ACurve& ct, std::string& s);
     bool FromJSONString(ACurve& ct, const std::string& s);
};
}
