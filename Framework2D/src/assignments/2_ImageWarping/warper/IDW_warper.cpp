#include "IDW_warper.h"
#include <cmath>

namespace USTC_CG
{

void IDWWarper::init(const std::vector<float>& src_x, const std::vector<float>& src_y,
                     const std::vector<float>& dst_x, const std::vector<float>& dst_y)
{
    p_x = src_x;
    p_y = src_y;
    q_x = dst_x;
    q_y = dst_y;
}

void IDWWarper::warp(float x, float y, float& out_x, float& out_y)
{
    if (p_x.empty())
    {
        out_x = x;
        out_y = y;
        return;
    }

    float weight_sum = 0.0f;
    float disp_x = 0.0f;
    float disp_y = 0.0f;
    const float mu = 2.0f;

    for (size_t i = 0; i < q_x.size(); ++i)
    {
        float dx = x - q_x[i];
        float dy = y - q_y[i];
        float dist_sq = dx * dx + dy * dy;

        if (dist_sq < 1e-6f)
        {
            out_x = p_x[i];
            out_y = p_y[i];
            return;
        }

        float w = 1.0f / std::pow(dist_sq, mu / 2.0f);
        weight_sum += w;
        disp_x += w * (p_x[i] - q_x[i]);
        disp_y += w * (p_y[i] - q_y[i]);
    }

    disp_x /= weight_sum;
    disp_y /= weight_sum;

    out_x = x + disp_x;
    out_y = y + disp_y;
}

}  // namespace USTC_CG
