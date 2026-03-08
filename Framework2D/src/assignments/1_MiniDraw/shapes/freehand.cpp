#include "freehand.h"

#include <cmath>
#include <vector>
#include <imgui.h>

namespace USTC_CG
{

Freehand::Freehand(float x, float y) : x_list_({ x }), y_list_({ y }) 
// 构造函数，初始化自由手形状的起始点坐标。将传入的x和y坐标分别添加到x_list_和y_list_中，作为自由手形状的第一个控制点。
{
}

void Freehand::draw(const Config& config) const // 用于绘制自由手形状的函数
{
    if (x_list_.size() < 2)  // 如果点的数量少于2个，就无法形成线段，因此直接返回，不进行绘制。
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();  // 获取当前窗口的绘制列表，这个列表用于存储所有要绘制的图形元素。
    
    std::vector<ImVec2> points(x_list_.size());
    for (size_t i = 0; i < x_list_.size(); ++i)
    {
        float wx, wy;
        local_to_world(x_list_[i], y_list_[i], &wx, &wy);
        points[i] = ImVec2(config.bias[0] + wx, config.bias[1] + wy);
    }
    
    draw_list->AddPolyline(
        points.data(),
        points.size(),
        IM_COL32(line_color_[0], line_color_[1], line_color_[2], line_color_[3]),
        ImDrawFlags_None,
        line_thickness_);

    if (is_selected_)
    {
        draw_list->AddPolyline(
            points.data(),
            points.size(),
            IM_COL32(255, 255, 0, 255),
            ImDrawFlags_None,
            line_thickness_ + 2.0f);
    }
}

bool Freehand::is_inside(float x, float y) const
{
    float lx, ly;
    world_to_local(x, y, &lx, &ly);

    for (size_t i = 0; i + 1 < x_list_.size(); ++i)
    {
        float x1 = x_list_[i], y1 = y_list_[i];
        float x2 = x_list_[i+1], y2 = y_list_[i+1];

        float dx = x2 - x1;
        float dy = y2 - y1;
        float len_sq = dx * dx + dy * dy;
        
        if (len_sq < 1e-4f) continue;
        
        float dot = (lx - x1) * dx + (ly - y1) * dy;
        float t = std::max(0.0f, std::min(1.0f, dot / len_sq));
        
        float proj_x = x1 + t * dx;
        float proj_y = y1 + t * dy;
        
        float dist_sq = (lx - proj_x) * (lx - proj_x) + (ly - proj_y) * (ly - proj_y);
        if (dist_sq <= 25.0f) {
            return true;
        }
    }
    return false;
}

// For freehand, each update call appends a new point to the polyline.
void Freehand::update(float x, float y)  // 用于更新自由手形状的函数，每次调用都会向多段线添加一个新的点。通过调用add_control_point函数，将新的点坐标(x, y)添加到x_list_和y_list_中，形成新的控制点，从而使得自由手形状能够随着鼠标拖动而动态变化。
{
    add_control_point(x, y);
}

void Freehand::add_control_point(float x, float y)  
// 用于添加控制点的函数，将新的点坐标(x, y)分别添加到x_list_和y_list_中，形成新的控制点。这些控制点将用于绘制自由手形状的多段线，使得形状能够随着用户的输入动态变化。
{
    x_list_.push_back(x);
    y_list_.push_back(y);
}

}  // namespace USTC_CG
