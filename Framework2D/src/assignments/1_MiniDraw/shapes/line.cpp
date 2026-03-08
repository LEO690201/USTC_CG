#include "line.h"

#include <cmath>
#include <imgui.h>

namespace USTC_CG
{
// Draw the line using ImGui
void Line::draw(const Config& config) const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float wx1, wy1, wx2, wy2;
    local_to_world(start_point_x_, start_point_y_, &wx1, &wy1);
    local_to_world(end_point_x_, end_point_y_, &wx2, &wy2);

    ImVec2 p1(config.bias[0] + wx1, config.bias[1] + wy1);
    ImVec2 p2(config.bias[0] + wx2, config.bias[1] + wy2);

    draw_list->AddLine(
        p1,
        p2,
        IM_COL32(
            line_color_[0],
            line_color_[1],
            line_color_[2],
            line_color_[3]),
        line_thickness_);

    if (is_selected_)
    {
        draw_list->AddLine(
            p1,
            p2,
            IM_COL32(255, 255, 0, 255),
            line_thickness_ + 2.0f);
    }
}

bool Line::is_inside(float x, float y) const
{
    float lx, ly;
    world_to_local(x, y, &lx, &ly);

    float dx = end_point_x_ - start_point_x_;
    float dy = end_point_y_ - start_point_y_;
    float len_sq = dx * dx + dy * dy;

    if (len_sq < 1e-4f) return false;

    float dot = (lx - start_point_x_) * dx + (ly - start_point_y_) * dy;
    float t = std::max(0.0f, std::min(1.0f, dot / len_sq));

    float proj_x = start_point_x_ + t * dx;
    float proj_y = start_point_y_ + t * dy;

    float dist_sq = (lx - proj_x) * (lx - proj_x) + (ly - proj_y) * (ly - proj_y);
    return dist_sq <= 25.0f; // radius of 5
}

void Line::update(float x, float y)
{
    end_point_x_ = x;
    end_point_y_ = y;
}
}  // namespace USTC_CG