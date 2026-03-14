#pragma once

#include "warper.h"
#include <vector>

namespace USTC_CG
{
class DlibWarper : public Warper
{
   public:
    DlibWarper() = default;
    virtual ~DlibWarper() = default;

    void init(const std::vector<float>& src_x, const std::vector<float>& src_y,
              const std::vector<float>& dst_x, const std::vector<float>& dst_y) override;

    void warp(float x, float y, float& out_x, float& out_y) override;

   private:
    std::vector<float> p_x, p_y;
    std::vector<float> q_x, q_y;
    // Dlib models will be encapsulated internally to avoid dragging heavy headers externally
    void* model_ptr_x = nullptr;
    void* model_ptr_y = nullptr;
};
}  // namespace USTC_CG
