#pragma once

#include <cmath>

namespace USTC_CG
{
class Shape
{
   public:
    // Draw Settings
    struct Config
    {
        // Offset to convert canvas position to screen position
        float bias[2] = { 0.f, 0.f };
        // Line color in RGBA format
        unsigned char line_color[4] = { 255, 0, 0, 255 };
        float line_thickness = 2.0f;
    };

    // Shape specific drawing styles
    float line_thickness_ = 2.0f;
    unsigned char line_color_[4] = {255, 0, 0, 255};
    unsigned char fill_color_[4] = {0, 255, 0, 100};
    bool enable_fill_ = false;

    // Selection logic
    bool is_selected_ = false;

    // Transformation logic
    float translation_x_ = 0.0f;
    float translation_y_ = 0.0f;
    float rotation_ = 0.0f; // stored in radians
    float scale_ = 1.0f;

   public:
    virtual ~Shape() = default;

    /**
     * Hit testing: checks if a point is logically inside or along the stroke of the shape.
     */
    virtual bool is_inside(float /*x*/, float /*y*/) const
    {
        return false; // Default: not inside
    }

    /**
     * Apply transformations.
     */
    void translate(float dx, float dy)
    {
        translation_x_ += dx;
        translation_y_ += dy;
    }

    void scale(float s)
    {
        scale_ = s;
    }

    void rotate(float angle_rad)
    {
        rotation_ += angle_rad;
    }

    // Convert from world to local coordinates (inverse transform)
    void world_to_local(float wx, float wy, float* lx, float* ly) const
    {
        // Translate
        float tx = wx - translation_x_;
        float ty = wy - translation_y_;
        // Rotate
        float current_rotation = -rotation_;
        float cos_r = std::cos(current_rotation);
        float sin_r = std::sin(current_rotation);
        float rx = tx * cos_r - ty * sin_r;
        float ry = tx * sin_r + ty * cos_r;
        // Scale
        *lx = rx / scale_;
        *ly = ry / scale_;
    }

    // Convert from local to world coordinates (forward transform)
    void local_to_world(float lx, float ly, float* wx, float* wy) const
    {
        // Scale
        float sx = lx * scale_;
        float sy = ly * scale_;
        // Rotate
        float current_rotation = rotation_;
        float cos_r = std::cos(current_rotation);
        float sin_r = std::sin(current_rotation);
        float rx = sx * cos_r - sy * sin_r;
        float ry = sx * sin_r + sy * cos_r;
        // Translate
        *wx = rx + translation_x_;
        *wy = ry + translation_y_;
    }

    /**
     * Draws the shape on the screen.
     * This is a pure virtual function that must be implemented by all derived
     * classes.
     *
     * @param config The configuration settings for drawing, including line
     * color, thickness, and bias.
     *               - line_color defines the color of the shape's outline.
     *               - line_thickness determines how thick the outline will be.
     *               - bias is used to adjust the shape's position on the
     * screen.
     */
    virtual void draw(const Config& config) const = 0;
    /**
     * Updates the state of the shape.
     * This function allows for dynamic modification of the shape, in response
     * to user interactions like dragging.
     *
     * @param x, y Dragging point. e.g. end point of a line.
     */
    virtual void update(float x, float y) = 0;
    /**
     * Adds a control point to the shape.
     * This function is used to add control points to the shape, which can be
     * used to modify the shape's appearance.
     *
     * @param x, y Control point to be added. e.g. vertex of a polygon.
     */
    virtual void add_control_point(float /*x*/, float /*y*/) {}
};
}  // namespace USTC_CG