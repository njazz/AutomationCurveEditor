#include "AutomationCurve.hpp"

namespace AutomationCurve {
  struct JSONCodec {

    static void ToJSONString(const ACurve& ct, std::string& s);
    static bool FromJSONString(ACurve& ct, const std::string& s);

};

}
