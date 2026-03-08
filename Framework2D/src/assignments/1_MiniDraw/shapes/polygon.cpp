#include "polygon.h"

#include <cmath>
#include <vector>
#include <imgui.h>

namespace USTC_CG
{

Polygon::Polygon() : x_list_(), y_list_() {}  // 默认构造函数，初始化空的顶点列表

Polygon::Polygon(std::vector<float> x_list, std::vector<float> y_list)
    : x_list_(std::move(x_list)), y_list_(std::move(y_list))
{

}  // 构造函数，接受两个向量作为参数，分别表示多边形的x坐标和y坐标，并将它们移动到成员变量中

void Polygon::draw(const Config& config) const  
// 绘制多边形的函数
{
    if (x_list_.size() < 2 || y_list_.size() < 2 || x_list_.size() != y_list_.size())
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    std::vector<ImVec2> points(x_list_.size());
    for (size_t i = 0; i < x_list_.size(); ++i)
    {
        float wx, wy;
        local_to_world(x_list_[i], y_list_[i], &wx, &wy);
        points[i] = ImVec2(config.bias[0] + wx, config.bias[1] + wy);
    }

    if (enable_fill_ && points.size() >= 3)
    {
        draw_list->AddConvexPolyFilled(
            points.data(),
            points.size(),
            IM_COL32(fill_color_[0], fill_color_[1], fill_color_[2], fill_color_[3]));
    }

    draw_list->AddPolyline(
        points.data(),
        points.size(),
        IM_COL32(line_color_[0], line_color_[1], line_color_[2], line_color_[3]),
        ImDrawFlags_Closed,
        line_thickness_);

    if (is_selected_)
    {
        draw_list->AddPolyline(
            points.data(),
            points.size(),
            IM_COL32(255, 255, 0, 255),
            ImDrawFlags_Closed,
            line_thickness_ + 2.0f);
    }
}

bool Polygon::is_inside(float x, float y) const
{
    if (x_list_.size() < 3) return false;

    float lx, ly;
    world_to_local(x, y, &lx, &ly);

    bool inside = false;
    for (size_t i = 0, j = x_list_.size() - 1; i < x_list_.size(); j = i++)
    {
        float xi = x_list_[i], yi = y_list_[i];
        float xj = x_list_[j], yj = y_list_[j];

        bool intersect = ((yi > ly) != (yj > ly)) &&
                         (lx < (xj - xi) * (ly - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
    }

    // For simplicity, always return true if inside regardless of fill state,
    // or we could check distance to edges if enable_fill_ is false. 
    // Usually even un-filled shapes are selectable by clicking inside or edge.
    return inside;
}

void Polygon::update(float x, float y)  
// 更新多边形的函数，通常在用户交互时调用，例如在鼠标拖动时更新最后一个顶点的坐标
{
    if (!x_list_.empty() && !y_list_.empty())  // 检查顶点列表是否为空，如果不为空，则更新最后一个顶点的坐标为新的x和y值
    {
        x_list_.back() = x;
        y_list_.back() = y;
    }
}

void Polygon::add_control_point(float x, float y)
// 添加控制点的函数，用于在多边形中添加新的顶点，这些顶点可以用来修改多边形的形状
{
    x_list_.push_back(x);
    y_list_.push_back(y);
}

}  // namespace USTC_CG