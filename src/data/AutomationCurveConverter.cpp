
#include "AutomationCurve.hpp"

namespace AutomationCurve{


const std::vector<float> CurveConverter::AsFloatVector(const size_t res)
{
    if (!curve) return {};
    
    std::vector<float> ret;

    for (size_t i = 0; i < res; i++) {
        ret.push_back(curve->ValueAtFraction(float(i) / float(res)));
    }
    return ret;
} // resolution

const ACurve CurveConverter::AsLineApproximation(const size_t res)
{
    
    ACurve ret;
    
    if (!curve) return ret;
    
    ret.Clear();

    for (size_t i = 0; i < res; i++) {
        float fract = float(i) / float(res);
        ret.AddPoint(fract, curve->ValueAtFraction(fract));
    }
    return ret;
}

const std::vector<std::pair<float, float> > CurveConverter::AsPoints(const size_t res)
{
    std::vector<std::pair<float, float> > ret;
    if (!curve) return ret;
    
    for (size_t i = 0; i < res; i++) {
        std::pair<float, float> v;
        float fract = float(i) / float(res);
        v.first = fract;
        v.second = curve->ValueAtFraction(fract);
        ret.push_back(v);
    }
    return ret;
}

const ACurve CurveConverter::AsApproximation(const size_t numsteps)
{
    ACurve ret;
    
    if (!curve) return ret;
    
    if (curve->Size()==0) return *curve;
    
    ret.Clear();
    ret.AddPoint(0, curve->ValueAt(0));
    
    float t_start = 0;
    for (int i = 1; i < curve->Size(); i++) {
        float t_end = curve->TimeAt(i);
        for (int j = 0; j < numsteps; j++) {
            float fract = float(j+1) / float(numsteps);
            float tv = (1 - fract) * t_start + fract * t_end;
            auto v1 = curve->ValueAtFraction(tv);
            ret.AddPoint(tv, v1);
        }
        t_start = t_end;
    }

    return ret;
}

// ---
// TODO: threaded converter

}
