#include "RBF_warper.h"
#include <cmath>
#include <iostream>

namespace USTC_CG
{

static float phi(float r)
{
    float d = 100.0f;
    return std::sqrt(r * r + d * d);
}

void RBFWarper::solve(std::vector<std::vector<float>>& A, std::vector<float>& b, std::vector<float>& x_out)
{
    int n = A.size();
    x_out.assign(n, 0.0f);
    // Gaussian elimination
    for (int i = 0; i < n; ++i)
    {
        // Find pivot
        int pivot = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (std::abs(A[j][i]) > std::abs(A[pivot][i]))
            {
                pivot = j;
            }
        }
        std::swap(A[i], A[pivot]);
        std::swap(b[i], b[pivot]);

        if (std::abs(A[i][i]) < 1e-6f)
        {
            // Singular matrix
            continue;
        }

        // Eliminate
        for (int j = i + 1; j < n; ++j)
        {
            float factor = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k)
            {
                A[j][k] -= factor * A[i][k];
            }
            b[j] -= factor * b[i];
        }
    }

    // Back substitution
    for (int i = n - 1; i >= 0; --i)
    {
        if (std::abs(A[i][i]) < 1e-6f) continue;
        float sum = b[i];
        for (int j = i + 1; j < n; ++j)
        {
            sum -= A[i][j] * x_out[j];
        }
        x_out[i] = sum / A[i][i];
    }
}

void RBFWarper::init(const std::vector<float>& src_x, const std::vector<float>& src_y,
                     const std::vector<float>& dst_x, const std::vector<float>& dst_y)
{
    p_x = src_x;
    p_y = src_y;
    q_x = dst_x;
    q_y = dst_y;

    int n = q_x.size();
    if (n == 0) return;

    int mat_size = n + 3;
    std::vector<std::vector<float>> A(mat_size, std::vector<float>(mat_size, 0.0f));
    std::vector<float> b_x(mat_size, 0.0f);
    std::vector<float> b_y(mat_size, 0.0f);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            float dx = q_x[i] - q_x[j];
            float dy = q_y[i] - q_y[j];
            A[i][j] = phi(std::sqrt(dx * dx + dy * dy));
        }
        A[i][n] = 1.0f;
        A[i][n + 1] = q_x[i];
        A[i][n + 2] = q_y[i];

        A[n][i] = 1.0f;
        A[n + 1][i] = q_x[i];
        A[n + 2][i] = q_y[i];

        // Solve for the displacement difference (p - q) rather than absolute
        // or just solve for p_x, p_y
        b_x[i] = p_x[i];
        b_y[i] = p_y[i];
    }

    std::vector<std::vector<float>> A_copy_y = A;

    std::vector<float> x_sol, y_sol;
    solve(A, b_x, x_sol);
    solve(A_copy_y, b_y, y_sol);

    alpha_x.resize(n);
    alpha_y.resize(n);
    for (int i = 0; i < n; ++i)
    {
        alpha_x[i] = x_sol[i];
        alpha_y[i] = y_sol[i];
    }
    a1_x = x_sol[n];
    a2_x = x_sol[n + 1];
    a3_x = x_sol[n + 2];

    a1_y = y_sol[n];
    a2_y = y_sol[n + 1];
    a3_y = y_sol[n + 2];
}

void RBFWarper::warp(float x, float y, float& out_x, float& out_y)
{
    if (p_x.empty())
    {
        out_x = x;
        out_y = y;
        return;
    }

    float rx = a1_x + a2_x * x + a3_x * y;
    float ry = a1_y + a2_y * x + a3_y * y;

    for (size_t i = 0; i < q_x.size(); ++i)
    {
        float dx = x - q_x[i];
        float dy = y - q_y[i];
        float dist = std::sqrt(dx * dx + dy * dy);
        float w = phi(dist);
        rx += alpha_x[i] * w;
        ry += alpha_y[i] * w;
    }

    out_x = rx;
    out_y = ry;
}

}  // namespace USTC_CG
