

#include "AutomationCurve.hpp"

// "tween.hpp"
/* Tween, a lightweight easing library. zlib/libpng licensed.
 * - rlyeh.

 * Based on code by Robert Penner, GapJumper, Terry Schubring, Jesus Gollonet,
 * Tomas Cepeda, John Resig, lQuery team, Warren Moore. Thanks guys! :-)
 */

#include "math.h"

namespace AutomationCurve {

const double pi = 3.1415926535897932384626433832795;
const double pi2 = 3.1415926535897932384626433832795 / 2;

template <typename T>
T __tween_bounceout(const T& p) { return (p) < 4 / 11.0 ? (121 * (p) * (p)) / 16.0 : (p) < 8 / 11.0 ? (363 / 40.0 * (p) * (p)) - (99 / 10.0 * (p)) + 17 / 5.0 : (p) < 9 / 10.0 ? (4356 / 361.0 * (p) * (p)) - (35442 / 1805.0 * (p)) + 16061 / 1805.0 : (54 / 5.0 * (p) * (p)) - (513 / 25.0 * (p)) + 268 / 25.0; }

const std::map<std::string, EaseFunctor> EaseFunctorFactory::_map = {
    { "Hold", [](const float& t) -> float { return (0); } },
    { "Step", [](const float& t) -> float { return (t > .5); } },
    { "Linear", [](const float& t) -> float { return t; } },

    { "QuadIn", [](const float& t) -> float { return t * t; } },
    { "QuadOut", [](const float& t) -> float { return -(t * (t - 2)); } },
    { "QuadInOut", [](const float& t) -> float { return (t < 0.5) ? (2 * t * t) : ((-2 * t * t) + (4 * t) - 1); } },

    { "CubicIn", [](const float& t) -> float { return t * t * t; } },
    { "CubicOut", [](const float& t) -> float { return (t - 1) * (t - 1) * (t - 1) + 1; } },
    { "CubicInOut", [](const float& t) -> float { return (t < 0.5) ? (4 * t * t * t) : (.5 * ((2 * t) - 2) * ((2 * t) - 2) * ((2 * t) - 2) + 1); } },

    { "SineIn", [](const float& t) -> float { return sin((t - 1) * pi2) + 1; } },
    { "SineOut", [](const float& t) -> float { return sin(t * pi2); } },
    { "SineInOut", [](const float& t) -> float { return 0.5 * (1 - cos(t * pi)); } },

    { "ExpIn", [](const float& t) -> float { return (t == 0.0) ? t : pow(2, 10 * (t - 1)); } },
    { "ExpOut", [](const float& t) -> float { return (t == 1.0) ? t : 1 - pow(2, -10 * t); } },
    { "ExpInOut", [](const float& t) -> float {
         if (t == 0.0 || t == 1.0)
             return t;
         if (t < 0.5) {
             return 0.5 * pow(2, (20 * t) - 10);
         } else {
             return -0.5 * pow(2, (-20 * t) + 10) + 1;
         }
     } },

    { "CircIn", [](const float& t) -> float { return 1 - sqrt(1 - (t * t)); } }, { "CircOut", [](const float& t) -> float { return sqrt((2 - t) * t); } }, { "CircInOut", [](const float& t) -> float {
                                                                                                                                                                if (t < 0.5) {
                                                                                                                                                                    return 0.5 * (1 - sqrt(1 - 4 * (t * t)));
                                                                                                                                                                } else {
                                                                                                                                                                    return 0.5 * (sqrt(-((2 * t) - 3) * ((2 * t) - 1)) + 1);
                                                                                                                                                                }
                                                                                                                                                            } },

    { "ElasticIn", [](const float& t) -> float { return sin(13 * pi2 * t) * pow(2, 10 * (t - 1)); } }, { "ElasticOut", [](const float& t) -> float { return sin(-13 * pi2 * (t + 1)) * pow(2, -10 * t) + 1; } }, { "ElasticInOut", [](const float& t) -> float {
                                                                                                                                                                                                                      if (t < 0.5) {
                                                                                                                                                                                                                          return 0.5 * sin(13 * pi2 * (2 * t)) * pow(2, 10 * ((2 * t) - 1));
                                                                                                                                                                                                                      } else {
                                                                                                                                                                                                                          return 0.5 * (sin(-13 * pi2 * ((2 * t - 1) + 1)) * pow(2, -10 * (2 * t - 1)) + 2);
                                                                                                                                                                                                                      }
                                                                                                                                                                                                                  } },

    { "BackIn", [](const float& t) -> float {

         double s = 1.70158f;
         return t * t * ((s + 1) * t - s);

     } },
    { "BackOut", [](const float& t) -> float {
         // ???
         double s = 1.70158f;
         auto t2 = t - 1;
         return 1.f * (t2 * t2 * ((s + 1) * t2 + s) + 1);
     } },
    { "BackInOut", [](const float& t) -> float {
         // TODO: fix
         double s = 1.70158f * 1.525f;
         if (t < 0.5) {
             auto p2 = t * 2;
             return 0.5 * p2 * p2 * (p2 * s + p2 - s);
         } else {
             auto p = t * 2 - 2;
             return 0.5 * (2 + p * p * (p * s + p + s));
         }

     } },

    { "BounceIn", [](const float& t) -> float {
         return 1 - __tween_bounceout(1 - t);
     } },
    { "BounceOut", [](const float& t) -> float { return __tween_bounceout(t); } }, { "BounceInOut", [](const float& t) -> float {
                                                                                        if (t < 0.5) {
                                                                                            return 0.5 * (1 - __tween_bounceout(1 - t * 2));
                                                                                        } else {
                                                                                            return 0.5 * __tween_bounceout((t * 2 - 1)) + 0.5;
                                                                                        }
                                                                                    } },

};

EaseFunctor EaseFunctorFactory::Create(const std::string& name)
{
    // TODO:
    if (_map.find(name) == _map.end())
        return [](const float& t) { return t; };

    return _map.at(name);
}

std::vector<std::string> EaseFunctorFactory::Names()
{
    std::vector<std::string> ret;
    for (const auto& e : _map) {
        ret.push_back(e.first);
    }
    return ret;
}
}

// ------------------------------

//
//            // Modeled after the quartic x^4
//            case TYPE::QUARTIN: {
//                return p * p * p * p;
//            }
//
//            // Modeled after the quartic y = 1 - (x - 1)^4
//            case TYPE::QUARTOUT: {
//                double f = (p - 1);
//                return f * f * f * (1 - p) + 1;
//            }
//
//            // Modeled after the piecewise quartic
//            // y = (1/2)((2x)^4)        ; [0, 0.5)
//            // y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
//            case TYPE::QUARTINOUT:  {
//                if(p < 0.5) {
//                    return 8 * p * p * p * p;
//                }
//                else {
//                    double f = (p - 1);
//                    return -8 * f * f * f * f + 1;
//                }
//            }
//
//            // Modeled after the quintic y = x^5
//            case TYPE::QUINTIN:  {
//                return p * p * p * p * p;
//            }
//
//            // Modeled after the quintic y = (x - 1)^5 + 1
//            case TYPE::QUINTOUT:  {
//                double f = (p - 1);
//                return f * f * f * f * f + 1;
//            }
//
//            // Modeled after the piecewise quintic
//            // y = (1/2)((2x)^5)       ; [0, 0.5)
//            // y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
//            case TYPE::QUINTINOUT:  {
//                if(p < 0.5) {
//                    return 16 * p * p * p * p * p;
//                }
//                else {
//                    double f = ((2 * p) - 2);
//                    return  0.5 * f * f * f * f * f + 1;
//                }
//            }
//}

