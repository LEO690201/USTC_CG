#include "rect.h"

#include <cmath>
#include <vector>
#include <imgui.h>

namespace USTC_CG
{
// Draw the rectangle using ImGui
void Rect::draw(const Config& config) const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Vertices of the rectangle
    float lx[4] = { start_point_x_, end_point_x_, end_point_x_, start_point_x_ };
    float ly[4] = { start_point_y_, start_point_y_, end_point_y_, end_point_y_ };

    std::vector<ImVec2> points(4);
    for (int i = 0; i < 4; ++i)
    {
        float wx, wy;
        local_to_world(lx[i], ly[i], &wx, &wy);
        points[i] = ImVec2(config.bias[0] + wx, config.bias[1] + wy);
    }

    if (enable_fill_)
    {
        draw_list->AddConvexPolyFilled(
            points.data(),
            4,
            IM_COL32(fill_color_[0], fill_color_[1], fill_color_[2], fill_color_[3]));
    }

    // Always draw outline if we have it
    draw_list->AddPolyline(
        points.data(),
        4,
        IM_COL32(line_color_[0], line_color_[1], line_color_[2], line_color_[3]),
        ImDrawFlags_Closed,
        line_thickness_);

    if (is_selected_)
    {
        // Highlight selection
        draw_list->AddPolyline(
            points.data(),
            4,
            IM_COL32(255, 255, 0, 255), // Yellow for selection
            ImDrawFlags_Closed,
            line_thickness_ + 2.0f);
    }
}

bool Rect::is_inside(float x, float y) const
{
    float lx, ly;
    world_to_local(x, y, &lx, &ly);

    float min_x = std::min(start_point_x_, end_point_x_);
    float max_x = std::max(start_point_x_, end_point_x_);
    float min_y = std::min(start_point_y_, end_point_y_);
    float max_y = std::max(start_point_y_, end_point_y_);

    // Check with a small tolerance for lines
    float tol = 5.0f;
    if (enable_fill_)
    {
        return lx >= min_x - tol && lx <= max_x + tol &&
               ly >= min_y - tol && ly <= max_y + tol;
    }
    else
    {
        // Outline only
        bool on_x = lx >= min_x - tol && lx <= max_x + tol;
        bool on_y = ly >= min_y - tol && ly <= max_y + tol;
        bool near_left_right = std::abs(lx - min_x) <= tol || std::abs(lx - max_x) <= tol;
        bool near_top_bottom = std::abs(ly - min_y) <= tol || std::abs(ly - max_y) <= tol;
        return (on_x && near_top_bottom) || (on_y && near_left_right);
    }
}

void Rect::update(float x, float y)
{
    end_point_x_ = x;
    end_point_y_ = y;
}

}  // namespace USTC_CG
