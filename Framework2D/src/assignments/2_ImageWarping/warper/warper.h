#pragma once

#include <vector>

namespace USTC_CG
{
class Warper
{
   public:
    virtual ~Warper() = default;

    virtual void init(const std::vector<float>& src_x, const std::vector<float>& src_y,
                      const std::vector<float>& dst_x, const std::vector<float>& dst_y) = 0;

    virtual void warp(float x, float y, float& out_x, float& out_y) = 0;
};
}  // namespace USTC_CG
