#pragma once

#include <memory>

#include "common/window.h"
#include "canvas_widget.h"

namespace USTC_CG
{
class MiniDraw : public Window  // 这里这个class xx ： public Window 代表 MiniDraw 继承了 Window 类，
// 意思是 MiniDraw 是 Window 的一个子类，MiniDraw 可以使用 Window 类的成员函数和属性，并且可以重写 Window 类中的虚函数来实现自己的功能。
{
   public:
    explicit MiniDraw(const std::string& window_name);
    ~MiniDraw();

    void draw();

   private:
    void draw_canvas();

    std::shared_ptr<Canvas> p_canvas_ = nullptr;

    bool flag_show_canvas_view_ = true;
};
}  // namespace USTC_CG