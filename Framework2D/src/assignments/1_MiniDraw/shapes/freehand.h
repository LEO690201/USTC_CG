#pragma once

#include "shape.h"
#include <vector>

namespace USTC_CG
{
// Freehand (free drawing) shape: records all mouse-dragged points and draws
// them as a connected polyline. Points are added continuously during dragging.
class Freehand : public Shape
{
   public:
    Freehand() = default;
    Freehand(float x, float y);
    virtual ~Freehand() = default;

    // Draw the freehand polyline.
    void draw(const Config& config) const override;

    // Adds a new point to the polyline (called during mouse drag).
    void update(float x, float y) override;

    // Same as update – appends a point.
    void add_control_point(float x, float y) override;

    bool is_inside(float x, float y) const override;

   private:
    std::vector<float> x_list_, y_list_;
};
}  // namespace USTC_CG
