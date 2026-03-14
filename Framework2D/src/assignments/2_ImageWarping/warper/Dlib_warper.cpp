#include "Dlib_warper.h"
#include <iostream>

#ifdef USE_DLIB
#include <dlib/dnn.h>
#include <dlib/matrix.h>

using namespace dlib;

// A simple MLP for predicting x and y coordinates
using net_type = loss_mean_squared<
    fc<1,
    relu<fc<16,
    relu<fc<16,
    input<matrix<float, 2, 1>>
    >>>>>>;

#endif

namespace USTC_CG
{
void DlibWarper::init(const std::vector<float>& src_x, const std::vector<float>& src_y,
                      const std::vector<float>& dst_x, const std::vector<float>& dst_y)
{
    p_x = src_x;
    p_y = src_y;
    q_x = dst_x;
    q_y = dst_y;

#ifdef USE_DLIB
    if (q_x.empty()) return;

    // We train the model to map from Destination (q) back to Source (p)
    std::vector<matrix<float, 2, 1>> samples;
    std::vector<float> targets_x;
    std::vector<float> targets_y;

    for (size_t i = 0; i < q_x.size(); ++i)
    {
        matrix<float, 2, 1> pt;
        pt = q_x[i], q_y[i];
        samples.push_back(pt);
        targets_x.push_back(p_x[i]);
        targets_y.push_back(p_y[i]);
    }

    net_type* net_x = new net_type();
    net_type* net_y = new net_type();

    dnn_trainer<net_type> trainer_x(*net_x);
    trainer_x.set_learning_rate(0.1);
    trainer_x.set_min_learning_rate(1e-5);
    trainer_x.set_synchronization_file("dlib_sync_x", std::chrono::seconds(20));

    dnn_trainer<net_type> trainer_y(*net_y);
    trainer_y.set_learning_rate(0.1);
    trainer_y.set_min_learning_rate(1e-5);
    trainer_y.set_synchronization_file("dlib_sync_y", std::chrono::seconds(20));

    // Simple train
    for (int iter = 0; iter < 500; ++iter) {
        trainer_x.train_one_step(samples, targets_x);
        trainer_y.train_one_step(samples, targets_y);
    }
    
    // Store pointers
    model_ptr_x = net_x;
    model_ptr_y = net_y;
#else
    std::cout << "[Warning] Project compiled without Dlib Support! Dlib Warping will act as identity mapping." << std::endl;
#endif
}

void DlibWarper::warp(float x, float y, float& out_x, float& out_y)
{
    if (p_x.empty())
    {
        out_x = x;
        out_y = y;
        return;
    }

#ifdef USE_DLIB
    net_type* net_x = static_cast<net_type*>(model_ptr_x);
    net_type* net_y = static_cast<net_type*>(model_ptr_y);

    if (!net_x || !net_y) {
        out_x = x;
        out_y = y;
        return;
    }

    matrix<float, 2, 1> input_pt;
    input_pt = x, y;

    // Use test mode (no grad)
    auto res_x = (*net_x)(input_pt);
    auto res_y = (*net_y)(input_pt);

    out_x = res_x;
    out_y = res_y;
#else
    out_x = x;
    out_y = y;
#endif
}

}  // namespace USTC_CG
