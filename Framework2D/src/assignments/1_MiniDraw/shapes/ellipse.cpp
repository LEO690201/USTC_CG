#include "ellipse.h"

#include <imgui.h>
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace USTC_CG
{
void Ellipse::draw(const Config& config) const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const float rx = std::abs(end_x_ - center_x_);
    const float ry = std::abs(end_y_ - center_y_);

    const int segments = 50;
    std::vector<ImVec2> points(segments);

    for (int i = 0; i < segments; ++i)
    {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float lx = center_x_ + rx * std::cos(theta);
        float ly = center_y_ + ry * std::sin(theta);

        float wx, wy;
        local_to_world(lx, ly, &wx, &wy);
        points[i] = ImVec2(config.bias[0] + wx, config.bias[1] + wy);
    }

    if (enable_fill_)
    {
        draw_list->AddConvexPolyFilled(
            points.data(),
            segments,
            IM_COL32(fill_color_[0], fill_color_[1], fill_color_[2], fill_color_[3]));
    }

    draw_list->AddPolyline(
        points.data(),
        segments,
        IM_COL32(line_color_[0], line_color_[1], line_color_[2], line_color_[3]),
        ImDrawFlags_Closed,
        line_thickness_);

    if (is_selected_)
    {
        draw_list->AddPolyline(
            points.data(),
            segments,
            IM_COL32(255, 255, 0, 255),
            ImDrawFlags_Closed,
            line_thickness_ + 2.0f);
    }
}

bool Ellipse::is_inside(float x, float y) const
{
    float lx, ly;
    world_to_local(x, y, &lx, &ly);

    float rx = std::abs(end_x_ - center_x_);
    float ry = std::abs(end_y_ - center_y_);

    if (rx < 1e-4f || ry < 1e-4f) return false;

    float dx = lx - center_x_;
    float dy = ly - center_y_;
    float value = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);

    if (enable_fill_)
    {
        return value <= 1.05f;
    }
    else
    {
        return value >= 0.85f && value <= 1.15f; 
    }
}


void Ellipse::update(float x, float y)
{
    // 这里是更新椭圆的状态，通常是在用户交互时调用，
    // 例如在鼠标拖动时更新椭圆的半径或中心坐标
    end_x_ = x;
    end_y_ = y;
}
}