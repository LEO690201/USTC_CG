#pragma once  // 这里是防止头文件被多次包含

#include "shape.h"

namespace USTC_CG
{
class Ellipse : public Shape
{
   public:
    Ellipse() = default;

    // 与 Canvas 传参语义一致：起点 + 终点

    Ellipse(
        float center_x, 
        float center_y, 
        float end_x, 
        float end_y)
        : center_x_(center_x),
          center_y_(center_y),
          end_x_(end_x),
          end_y_(end_y)
    {
    }

    virtual ~Ellipse() = default; // 默认析构函数

    void draw(const Config& config) const override; // 重写Shape类的draw函数，负责绘制椭圆
    void update(float x, float y) override; // 重写Shape类的update函数，负责更新椭圆的状态

    bool is_inside(float x, float y) const override;

   private:
        float center_x_ = 0.0f, center_y_ = 0.0f;
        float end_x_ = 0.0f, end_y_ = 0.0f;
};
}

