#include "canvas_widget.h"

#include <cmath>
#include <iostream>

#include "imgui.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/ellipse.h"
#include "shapes/polygon.h"
#include "shapes/freehand.h"

namespace USTC_CG
{
void Canvas::draw()
{
    draw_background();
    if (is_hovered_ && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        mouse_click_event();
    if (is_hovered_ && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    // 这是用于处理鼠标右键点击事件的条件语句。它首先检查鼠标是否悬停在画布上（is_hovered_），然后使用ImGui::IsMouseClicked函数检查是否有鼠标右键点击事件发生。如果满足这两个条件，就会调用mouse_right_click_event()函数来处理这个事件，通常用于完成多边形的绘制。
        mouse_right_click_event();
    mouse_move_event();
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        mouse_release_event();

    draw_shapes();
}

void Canvas::set_attributes(const ImVec2& min, const ImVec2& size)
{
    canvas_min_ = min;
    canvas_size_ = size;
    canvas_minimal_size_ = size;
    canvas_max_ =
        ImVec2(canvas_min_.x + canvas_size_.x, canvas_min_.y + canvas_size_.y);
}

void Canvas::show_background(bool flag)
{
    show_background_ = flag;
}

void Canvas::set_default()
{
    draw_status_ = false;
    shape_type_ = kDefault;
}

void Canvas::set_select()
{
    draw_status_ = false;
    shape_type_ = kSelect;
}

void Canvas::set_line()
{
    draw_status_ = false;
    shape_type_ = kLine;
}

void Canvas::set_rect()
{
    draw_status_ = false;
    shape_type_ = kRect;
}

// HW1_TODO：实现更多图形类型
void Canvas::set_ellipse()
{
    draw_status_ = false;
    shape_type_ = kEllipse;
}

void Canvas::set_polygon()
{
    draw_status_ = false;
    shape_type_ = kPolygon;
    current_shape_.reset();  
    // 用于重置当前形状的智能指针，确保在切换到多边形绘制模式时没有残留的当前形状。这是为了避免在切换形状类型时出现不必要的内存占用或潜在的错误。通过调用reset()函数，current_shape_将被置空，准备好接受新的多边形形状对象。
}

void Canvas::set_freehand()
{
    draw_status_ = false;
    shape_type_ = kFreehand;
    current_shape_.reset();
}

void Canvas::clear_shape_list()
{
    shape_list_.clear();
}

void Canvas::undo()
{
    if (!shape_list_.empty())
        shape_list_.pop_back();
}

void Canvas::draw_background()
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (show_background_)
    {
        // 绘制背景矩形
        draw_list->AddRectFilled(canvas_min_, canvas_max_, background_color_);
        // 绘制背景边框
        draw_list->AddRect(canvas_min_, canvas_max_, border_color_);
    }
    /// 在画布上方放置一个不可见按钮，用于捕获鼠标交互。
    ImGui::SetCursorScreenPos(canvas_min_);
    ImGui::InvisibleButton(
        label_.c_str(), canvas_size_, ImGuiButtonFlags_MouseButtonLeft);
    // 记录当前不可见按钮的状态
    is_hovered_ = ImGui::IsItemHovered();
    is_active_ = ImGui::IsItemActive();
}

void Canvas::draw_shapes()
{
    Shape::Config s = { .bias = { canvas_min_.x, canvas_min_.y } };
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // ClipRect 可以隐藏矩形区域之外的绘制内容
    draw_list->PushClipRect(canvas_min_, canvas_max_, true);
    for (const auto& shape : shape_list_)
    {
        shape->draw(s);
    }
    if (draw_status_ && current_shape_)
    {
        current_shape_->draw(s);
    }
    draw_list->PopClipRect();
}

void Canvas::mouse_click_event()  // 用于处理鼠标点击事件的函数，根据当前选择的形状类型和鼠标位置，创建相应的形状对象并更新绘制状态。
{
    ImVec2 mouse = mouse_pos_in_canvas();  // 获取鼠标在画布中的相对位置，考虑了画布的偏移量。这是通过调用mouse_pos_in_canvas()函数实现的，该函数计算了鼠标在画布坐标系中的位置，以便后续的绘制操作能够正确地使用这个位置来创建和更新形状对象。

    if (shape_type_ == kSelect || shape_type_ == kDefault)
    {
        // Object selection logic
        if (selected_shape_) {
            selected_shape_->is_selected_ = false;
            selected_shape_ = nullptr;
        }

        // Iterate backwards to select the topmost shape
        for (auto it = shape_list_.rbegin(); it != shape_list_.rend(); ++it)
        {
            if ((*it)->is_inside(mouse.x, mouse.y))
            {
                selected_shape_ = *it;
                selected_shape_->is_selected_ = true;
                break;
            }
        }
        return;
    }

    // Function to apply current styles
    auto apply_styles = [&](std::shared_ptr<Shape> shape) {
        shape->line_thickness_ = current_line_thickness_;
        shape->line_color_[0] = static_cast<unsigned char>(current_line_color_[0] * 255.f);
        shape->line_color_[1] = static_cast<unsigned char>(current_line_color_[1] * 255.f);
        shape->line_color_[2] = static_cast<unsigned char>(current_line_color_[2] * 255.f);
        shape->line_color_[3] = static_cast<unsigned char>(current_line_color_[3] * 255.f);
        shape->fill_color_[0] = static_cast<unsigned char>(current_fill_color_[0] * 255.f);
        shape->fill_color_[1] = static_cast<unsigned char>(current_fill_color_[1] * 255.f);
        shape->fill_color_[2] = static_cast<unsigned char>(current_fill_color_[2] * 255.f);
        shape->fill_color_[3] = static_cast<unsigned char>(current_fill_color_[3] * 255.f);
        shape->enable_fill_ = current_enable_fill_;
    };

    // --- 多边形：多次点击添加顶点，右键点击完成 ---
    if (shape_type_ == kPolygon)
    {
        if (!draw_status_)
        {
            // 第一次点击：创建多边形，放置第一个顶点，
            // 并额外添加一个会跟随鼠标移动的“预览”点。
            
            draw_status_ = true;
            start_point_ = end_point_ = mouse;
            auto poly = std::make_shared<Polygon>();
            apply_styles(poly);
            poly->add_control_point(mouse.x, mouse.y);  // 已确认的顶点
            poly->add_control_point(mouse.x, mouse.y);  // 预览顶点
            current_shape_ = poly;
        }
        else
        {
            // 后续点击：将当前预览位置确认为一个新顶点，
            // 然后在同一位置添加一个新的预览点。
            current_shape_->add_control_point(mouse.x, mouse.y);
        }
        return;
    }

    // --- 自由绘制：按住鼠标左键并拖动进行绘制 ---
    if (shape_type_ == kFreehand)
    {
        if (!draw_status_)
        {
            draw_status_ = true;
            start_point_ = end_point_ = mouse;
            current_shape_ = std::make_shared<Freehand>(mouse.x, mouse.y);
            apply_styles(current_shape_);
        }
        return;
    }

    // --- 其他图形（直线 / 矩形 / 椭圆）：通过两次点击完成绘制 ---
    if (!draw_status_)
    {
        draw_status_ = true;
        start_point_ = end_point_ = mouse;  // 记录第一次点击的位置作为图形的起点。
        switch (shape_type_)
        {
            case kDefault: 
            case kSelect: break;
            case kLine:  // 创建一个新的Line对象，使用第一次点击的位置作为起点，当前鼠标位置作为终点。这个对象被赋值给current_shape_智能指针，以便在后续的绘制过程中进行更新和最终确认。
            {
                current_shape_ = std::make_shared<Line>(
                    start_point_.x,
                    start_point_.y,
                    end_point_.x,
                    end_point_.y);
                apply_styles(current_shape_);
                break;
            }
            case kRect:
            {
                current_shape_ = std::make_shared<Rect>(
                    start_point_.x,
                    start_point_.y,
                    end_point_.x,
                    end_point_.y);
                apply_styles(current_shape_);
                break;
            }
            case kEllipse:
            {
                current_shape_ = std::make_shared<Ellipse>(
                    start_point_.x,
                    start_point_.y,
                    end_point_.x,
                    end_point_.y);
                apply_styles(current_shape_);
                break;
            }
            default: break;
        }
    }
    else
    {
        // 第二次点击后完成该图形。
        draw_status_ = false;
        if (current_shape_)
        {
            shape_list_.push_back(current_shape_);
            current_shape_.reset();
        }
    }
}

void Canvas::mouse_move_event()
{
    if (!draw_status_ || !current_shape_)
        return;

    end_point_ = mouse_pos_in_canvas();

    if (shape_type_ == kFreehand)
    {
        // 对于自由绘制，仅在鼠标按键按住时追加点。
        if (is_active_)
            current_shape_->update(end_point_.x, end_point_.y);
    }
    else
    {
        // 对于多边形和其他图形，更新预览点（最后一个点）。
        current_shape_->update(end_point_.x, end_point_.y);
    }
}

void Canvas::mouse_release_event()
{
    // 当鼠标按键释放时，自由绘制结束。
    if (draw_status_ && shape_type_ == kFreehand)
    {
        draw_status_ = false;
        if (current_shape_)
        {
            shape_list_.push_back(current_shape_);
            current_shape_.reset();
        }
    }
}

// 右键点击用于结束当前正在绘制的多边形。
void Canvas::mouse_right_click_event()
{
    if (draw_status_ && shape_type_ == kPolygon && current_shape_)
    {
        draw_status_ = false;
        shape_list_.push_back(current_shape_);
        current_shape_.reset();
    }
}

ImVec2 Canvas::mouse_pos_in_canvas() const
{
    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 mouse_pos_in_canvas(
        io.MousePos.x - canvas_min_.x, io.MousePos.y - canvas_min_.y);
    return mouse_pos_in_canvas;
}
}  // namespace USTC_CG