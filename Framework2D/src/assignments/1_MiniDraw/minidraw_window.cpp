#include "minidraw_window.h"

#include <iostream>

namespace USTC_CG
{
MiniDraw::MiniDraw(const std::string& window_name) : Window(window_name)
{
    p_canvas_ = std::make_shared<Canvas>("Widget.Canvas");
}

MiniDraw::~MiniDraw()
{
}

void MiniDraw::draw()
{
    draw_canvas();
}

void MiniDraw::draw_canvas()
{
    // Set a full screen canvas view
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    if (ImGui::Begin(
            "Canvas",
            &flag_show_canvas_view_,
            ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoBackground))
    {
        // Buttons for shape types
        if (ImGui::Button("Select"))
        {
            std::cout << "Set mode to Select" << std::endl;
            p_canvas_->set_select();
        }
        ImGui::SameLine();
        if (ImGui::Button("Line"))
        {
            std::cout << "Set shape to Line" << std::endl;
            p_canvas_->set_line();
        }
        ImGui::SameLine();
        if (ImGui::Button("Rect"))
        {
            std::cout << "Set shape to Rect" << std::endl;
            p_canvas_->set_rect();  
            //  p_canvas_ 是一个智能指针，指向 Canvas 类的实例。通过调用 p_canvas_->set_rect()，
            // 我们可以设置当前绘制的形状类型为矩形（Rect）。这个函数会改变 Canvas 类中的 shape_type_ 成员变量，
            // 使得在后续的绘制过程中，Canvas 会根据 shape_type_ 的值来决定绘制哪种形状。
        }

        // HW1_TODO: More primitives
        //    - Ellipse
        //    - Polygon
        //    - Freehand(optional)
        ImGui::SameLine(); // 使得下一个按钮与前一个按钮在同一行显示
        if (ImGui::Button("Ellipse"))
        {
            std::cout << "Set shape to Ellipse" << std::endl;
            p_canvas_->set_ellipse(); 
        }

        ImGui::SameLine();
        if (ImGui::Button("Polygon"))  // 这里的polygon是多边形
        {
            std::cout << "Set shape to Polygon" << std::endl;
            p_canvas_->set_polygon();
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Freehand"))
        {
            std::cout << "Set shape to Freehand" << std::endl;
            p_canvas_->set_freehand();
        }

        ImGui::SameLine();
        if (ImGui::Button("Undo"))
        {
            std::cout << "Undo last shape" << std::endl;
            p_canvas_->undo();
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear"))
        {
            std::cout << "Clear canvas" << std::endl;
            p_canvas_->clear_shape_list();
        }

        // --- Current Styles UI ---
        ImGui::Separator();
        ImGui::Text("Global Shape Styles:");
        ImGui::SliderFloat("Line Thickness", &p_canvas_->current_line_thickness_, 1.0f, 20.0f);
        ImGui::ColorEdit4("Line Color", p_canvas_->current_line_color_);
        ImGui::Checkbox("Enable Fill", &p_canvas_->current_enable_fill_);
        if (p_canvas_->current_enable_fill_) {
            ImGui::SameLine();
            ImGui::ColorEdit4("Fill Color", p_canvas_->current_fill_color_);
        }
        ImGui::Separator();

        // --- Selected Shape Transform UI ---
        if (p_canvas_->selected_shape_) {
            ImGui::Text("Selected Shape Transformations:");
            auto shape = p_canvas_->selected_shape_;

            float dx = 0.0f, dy = 0.0f;
            if (ImGui::DragFloat("Translate X", &dx, 1.0f)) {
                shape->translate(dx, 0.0f);
            }
            if (ImGui::DragFloat("Translate Y", &dy, 1.0f)) {
                shape->translate(0.0f, dy);
            }
            
            float scale = shape->scale_;
            if (ImGui::DragFloat("Scale", &scale, 0.01f, 0.1f, 10.0f)) {
                shape->scale(scale);
            }
            
            float rotation = shape->rotation_ * 180.0f / 3.1415926535f; // Rad to Deg
            if (ImGui::DragFloat("Rotate (deg)", &rotation, 1.0f)) {
                shape->rotation_ = rotation * 3.1415926535f / 180.0f;
            }
            ImGui::Separator();
        }

        // Canvas component
        ImGui::Text(
            "Line/Rect/Ellipse: click to start, click to finish. "
            "Polygon: left-click to add vertex, right-click to finish. "
            "Freehand: hold left button and drag.");
        // Set the canvas to fill the rest of the window
        const auto& canvas_min = ImGui::GetCursorScreenPos();
        const auto& canvas_size = ImGui::GetContentRegionAvail();
        p_canvas_->set_attributes(canvas_min, canvas_size);
        p_canvas_->draw();
    }
    ImGui::End();
}
}  // namespace USTC_CG