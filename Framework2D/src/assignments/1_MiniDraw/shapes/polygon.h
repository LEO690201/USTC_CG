#pragma once

#include "shape.h"
#include <vector>

namespace USTC_CG
{
class Polygon : public Shape
{
   public:
    Polygon();
    Polygon(std::vector<float> x_list, 
            std::vector<float> y_list);
    virtual ~Polygon() = default;

    void draw(const Config& config) const override;
    void update(float x, float y) override;
    void add_control_point(float x, float y) override;

    bool is_inside(float x, float y) const override;

   private:
    std::vector<float> x_list_, y_list_;
};
}// namespace USTC_CG