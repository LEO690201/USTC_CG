#include "target_image_widget.h"
#include <cmath>
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>

namespace USTC_CG
{
using uchar = unsigned char;

struct TargetImageWidget::PoissonSolver {
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
    std::vector<std::pair<int, int>> interior_pixels;
    std::vector<int> mask_index;
    int mask_width = 0;
    int mask_height = 0;
    bool is_ready = false;

    bool is_valid(const std::shared_ptr<Image>& mask) {
        if (!mask) return false;
        if (mask->width() != mask_width || mask->height() != mask_height) return false;
        
        int interior_col_counts = 0;
        for (int y = 0; y < mask_height; ++y) {
            for (int x = 0; x < mask_width; ++x) {
                bool is_mask = mask->get_pixel(x, y)[0] > 0;
                bool was_mask = mask_index[y * mask_width + x] >= 0;
                if (is_mask != was_mask) return false;
                if (is_mask) interior_col_counts++;
            }
        }
        if (interior_col_counts != interior_pixels.size()) return false;
        return true;
    }

    void build(const std::shared_ptr<Image>& mask) {
        mask_width = mask->width();
        mask_height = mask->height();
        mask_index.assign(mask_width * mask_height, -1);
        interior_pixels.clear();
        is_ready = false;

        for (int y = 0; y < mask_height; ++y) {
            for (int x = 0; x < mask_width; ++x) {
                if (mask->get_pixel(x, y)[0] > 0) {
                    mask_index[y * mask_width + x] = interior_pixels.size();
                    interior_pixels.push_back({x, y});
                }
            }
        }

        int N = interior_pixels.size();
        if (N == 0) return;
        
        Eigen::SparseMatrix<double> A(N, N);
        std::vector<Eigen::Triplet<double>> triplets;
        
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int i = 0; i < N; ++i) {
            int x = interior_pixels[i].first;
            int y = interior_pixels[i].second;

            triplets.push_back(Eigen::Triplet<double>(i, i, 4.0));
            
            for (int dir = 0; dir < 4; ++dir) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                
                if (nx >= 0 && nx < mask_width && ny >= 0 && ny < mask_height) {
                    int neighbor_idx = mask_index[ny * mask_width + nx];
                    if (neighbor_idx >= 0) {
                        triplets.push_back(Eigen::Triplet<double>(i, neighbor_idx, -1.0));
                    }
                }
            }
        }
        
        A.setFromTriplets(triplets.begin(), triplets.end());
        solver.compute(A);
        if(solver.info() == Eigen::Success) {
            is_ready = true;
        } else {
            std::cerr << "Poisson solver factorization failed!" << std::endl;
        }
    }
};

TargetImageWidget::~TargetImageWidget() = default;

TargetImageWidget::TargetImageWidget(
    const std::string& label,
    const std::string& filename)
    : ImageWidget(label, filename)
{
    if (data_)
        back_up_ = std::make_shared<Image>(*data_);
}

void TargetImageWidget::draw()
{
    // Draw the image
    ImageWidget::draw();
    // Invisible button for interactions
    ImGui::SetCursorScreenPos(position_);
    ImGui::InvisibleButton(
        label_.c_str(),
        ImVec2(
            static_cast<float>(image_width_),
            static_cast<float>(image_height_)),
        ImGuiButtonFlags_MouseButtonLeft);
    bool is_hovered_ = ImGui::IsItemHovered();
    // When the mouse is clicked or moving, we would adapt clone function to
    // copy the selected region to the target.

    if (is_hovered_ && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        mouse_click_event();
    }
    mouse_move_event();
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        mouse_release_event();
    }
}

void TargetImageWidget::set_source(std::shared_ptr<SourceImageWidget> source)
{
    source_image_ = source;
}

void TargetImageWidget::set_realtime(bool flag)
{
    flag_realtime_updating = flag;
}

void TargetImageWidget::restore()
{
    *data_ = *back_up_;
    update();
}

void TargetImageWidget::set_paste()
{
    clone_type_ = kPaste;
}

void TargetImageWidget::set_seamless()
{
    clone_type_ = kSeamless;
}

void TargetImageWidget::set_mix_gradient()
{
    clone_type_ = kMixGradient;
}

void TargetImageWidget::clone()
{
    // The implementation of different types of cloning
    // HW3_TODO: 
    // 1. In this function, you should at least implement the "seamless"
    // cloning labeled by `clone_type_ ==kSeamless`.
    //
    // 2. It is required to improve the efficiency of your seamless cloning to
    // achieve real-time editing. (Use decomposition of sparse matrix before
    // solve the linear system). The real-time updating (update when the mouse
    // is moving) is only available when the checkerboard is selected. 
    if (data_ == nullptr || source_image_ == nullptr ||
        source_image_->get_region_mask() == nullptr)
        return;
    // The selected region in the source image, this would be a binary mask.
    // The **size** of the mask should be the same as the source image.
    // The **value** of the mask should be 0 or 255: 0 for the background and
    // 255 for the selected region.
    std::shared_ptr<Image> mask = source_image_->get_region_mask();

    switch (clone_type_)
    {
        case USTC_CG::TargetImageWidget::kDefault: break;
        case USTC_CG::TargetImageWidget::kPaste:
        {
            restore();

            for (int x = 0; x < mask->width(); ++x)
            {
                for (int y = 0; y < mask->height(); ++y)
                {
                    int tar_x =
                        static_cast<int>(mouse_position_.x) + x -
                        static_cast<int>(source_image_->get_position().x);
                    int tar_y =
                        static_cast<int>(mouse_position_.y) + y -
                        static_cast<int>(source_image_->get_position().y);
                    if (0 <= tar_x && tar_x < image_width_ && 0 <= tar_y &&
                        tar_y < image_height_ && mask->get_pixel(x, y)[0] > 0)
                    {
                        data_->set_pixel(
                            tar_x,
                            tar_y,
                            source_image_->get_data()->get_pixel(x, y));
                    }
                }
            }
            break;
        }
        case USTC_CG::TargetImageWidget::kSeamless:
        case USTC_CG::TargetImageWidget::kMixGradient:
        {
            restore();
            if (!poisson_solver_ || !poisson_solver_->is_valid(mask)) {
                if (!poisson_solver_) {
                    poisson_solver_ = std::make_unique<PoissonSolver>();
                }
                poisson_solver_->build(mask);
            }
            
            if (!poisson_solver_->is_ready) break;
            
            int N = poisson_solver_->interior_pixels.size();
            Eigen::MatrixXd b(N, 3);
            b.setZero();
            
            int dx[] = {0, 0, -1, 1};
            int dy[] = {-1, 1, 0, 0};
            
            auto src_data = source_image_->get_data();
            int c_x = static_cast<int>(mouse_position_.x) - static_cast<int>(source_image_->get_position().x);
            int c_y = static_cast<int>(mouse_position_.y) - static_cast<int>(source_image_->get_position().y);
            
            for (int i = 0; i < N; ++i) {
                int x = poisson_solver_->interior_pixels[i].first;
                int y = poisson_solver_->interior_pixels[i].second;
                
                int tar_x = c_x + x;
                int tar_y = c_y + y;
                
                auto src_color = src_data->get_pixel(x, y);
                
                for (int dir = 0; dir < 4; ++dir) {
                    int nx = x + dx[dir];
                    int ny = y + dy[dir];
                    
                    std::vector<unsigned char> src_neighbor_color(3, 0);
                    if (nx >= 0 && nx < mask->width() && ny >= 0 && ny < mask->height()) {
                        src_neighbor_color = src_data->get_pixel(nx, ny);
                    } else {
                        src_neighbor_color = src_color;
                    }
                    
                    int s_diff_r = src_color[0] - src_neighbor_color[0];
                    int s_diff_g = src_color[1] - src_neighbor_color[1];
                    int s_diff_b = src_color[2] - src_neighbor_color[2];

                    int v_diff_r = s_diff_r;
                    int v_diff_g = s_diff_g;
                    int v_diff_b = s_diff_b;

                    if (clone_type_ == kMixGradient) {
                        int tar_nx = c_x + nx;
                        int tar_ny = c_y + ny;
                        tar_nx = std::clamp(tar_nx, 0, image_width_ - 1);
                        tar_ny = std::clamp(tar_ny, 0, image_height_ - 1);
                        
                        int t_x_clamped = std::clamp(c_x + x, 0, image_width_ - 1);
                        int t_y_clamped = std::clamp(c_y + y, 0, image_height_ - 1);

                        auto tar_center_color = back_up_->get_pixel(t_x_clamped, t_y_clamped);
                        auto tar_neighbor_color = back_up_->get_pixel(tar_nx, tar_ny);

                        int t_diff_r = tar_center_color[0] - tar_neighbor_color[0];
                        int t_diff_g = tar_center_color[1] - tar_neighbor_color[1];
                        int t_diff_b = tar_center_color[2] - tar_neighbor_color[2];

                        v_diff_r = std::abs(s_diff_r) > std::abs(t_diff_r) ? s_diff_r : t_diff_r;
                        v_diff_g = std::abs(s_diff_g) > std::abs(t_diff_g) ? s_diff_g : t_diff_g;
                        v_diff_b = std::abs(s_diff_b) > std::abs(t_diff_b) ? s_diff_b : t_diff_b;
                    }
                    
                    b(i, 0) += v_diff_r;
                    b(i, 1) += v_diff_g;
                    b(i, 2) += v_diff_b;
                    
                    bool neighbor_in_omega = false;
                    if (nx >= 0 && nx < mask->width() && ny >= 0 && ny < mask->height()) {
                        if (poisson_solver_->mask_index[ny * mask->width() + nx] >= 0) {
                            neighbor_in_omega = true;
                        }
                    }
                    
                    if (!neighbor_in_omega) {
                        int tar_nx = c_x + nx;
                        int tar_ny = c_y + ny;
                        tar_nx = std::clamp(tar_nx, 0, image_width_ - 1);
                        tar_ny = std::clamp(tar_ny, 0, image_height_ - 1);
                        auto tar_neighbor_color = back_up_->get_pixel(tar_nx, tar_ny);
                        
                        b(i, 0) += tar_neighbor_color[0];
                        b(i, 1) += tar_neighbor_color[1];
                        b(i, 2) += tar_neighbor_color[2];
                    }
                }
            }
            
            Eigen::MatrixXd x_res = poisson_solver_->solver.solve(b);
            
            for (int i = 0; i < N; ++i) {
                int x = poisson_solver_->interior_pixels[i].first;
                int y = poisson_solver_->interior_pixels[i].second;
                
                int tar_x = c_x + x;
                int tar_y = c_y + y;
                
                if (tar_x >= 0 && tar_x < image_width_ && tar_y >= 0 && tar_y < image_height_) {
                    std::vector<unsigned char> current_color(3);
                    current_color[0] = static_cast<unsigned char>(std::clamp(std::round(x_res(i, 0)), 0.0, 255.0));
                    current_color[1] = static_cast<unsigned char>(std::clamp(std::round(x_res(i, 1)), 0.0, 255.0));
                    current_color[2] = static_cast<unsigned char>(std::clamp(std::round(x_res(i, 2)), 0.0, 255.0));
                    data_->set_pixel(tar_x, tar_y, current_color);
                }
            }
            break;
        }
        default: break;
    }

    update();
}

void TargetImageWidget::mouse_click_event()
{
    edit_status_ = true;
    mouse_position_ = mouse_pos_in_canvas();
    clone();
}

void TargetImageWidget::mouse_move_event()
{
    if (edit_status_)
    {
        mouse_position_ = mouse_pos_in_canvas();
        if (flag_realtime_updating)
            clone();
    }
}

void TargetImageWidget::mouse_release_event()
{
    if (edit_status_)
    {
        edit_status_ = false;
    }
}

ImVec2 TargetImageWidget::mouse_pos_in_canvas() const
{
    ImGuiIO& io = ImGui::GetIO();
    return ImVec2(io.MousePos.x - position_.x, io.MousePos.y - position_.y);
}
}  // namespace USTC_CG