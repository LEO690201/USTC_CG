import torch
from forward_noising import (
    get_index_from_list,
    sqrt_one_minus_alphas_cumprod,
    betas,
    posterior_variance,
    sqrt_recip_alphas,
    forward_diffusion_sample,
)
import matplotlib.pyplot as plt
from dataloader import show_tensor_image
from unet import SimpleUnet
import numpy as np
import cv2 as cv


# TODO: 你需要在这个函数中实现单步去噪过程
@torch.no_grad()
def sample_timestep(model, x, t):
    t_index = t[0].item()
    betas_t = get_index_from_list(betas, t, x.shape)
    sqrt_one_minus_alphas_cumprod_t = get_index_from_list(
        sqrt_one_minus_alphas_cumprod, t, x.shape
    )
    sqrt_recip_alphas_t = get_index_from_list(sqrt_recip_alphas, t, x.shape)

    model_mean = sqrt_recip_alphas_t * (
        x - betas_t * model(x, t) / sqrt_one_minus_alphas_cumprod_t
    )
    
    if t_index == 0:
        return model_mean
    else:
        posterior_variance_t = get_index_from_list(posterior_variance, t, x.shape)
        noise = torch.randn_like(x)
        return model_mean + torch.sqrt(posterior_variance_t) * noise

# TODO: 你需要在这个函数中完成对纯高斯噪声的去噪，并输出对应的去噪图片
# 你需要调用上面的sample_timestep函数，以实现单步去噪
@torch.no_grad()
def sample_plot_image(model, device, img_size, T):
    # generate base noise
    x = torch.randn((1, 3, img_size, img_size), device=device)
    
    # iterate backwards
    for i in range(T - 1, -1, -1):
        t = torch.full((1,), i, device=device, dtype=torch.long)
        x = sample_timestep(model, x, t)
        
    x = (x.clamp(-1, 1) + 1) / 2
    x = (x * 255).type(torch.uint8)
    
    # display/save
    import os
    current_dir = os.path.dirname(os.path.abspath(__file__))
    save_path = os.path.join(current_dir, "ddpm_sample.png")
    
    plt.imshow(x[0].permute(1, 2, 0).cpu().numpy())
    plt.savefig(save_path)
    # plt.show() # Optional, comment out if running on server without GUI
    print(f"Saved sample image to {save_path}")
    return x

# TODO: 你需要在这个函数中完成模型以及其他相关资源的加载，并调用sample_plot_image进行去噪，以生成图片
def test_image_generation():
    device = "cuda" if torch.cuda.is_available() else "cpu"
    model = SimpleUnet()
    model.to(device)
    # Check if a trained model weight exists, load it
    import os
    current_dir = os.path.dirname(os.path.abspath(__file__))
    best_model_path = os.path.join(current_dir, "ddpm_mse_best.pth")
    model_path = os.path.join(current_dir, "ddpm_mse_epochs_5000.pth")
    try:
        if os.path.exists(best_model_path):
            model.load_state_dict(torch.load(best_model_path, map_location=device))
            print(f"Loaded trained model {best_model_path}")
        else:
            model.load_state_dict(torch.load(model_path, map_location=device))
            print(f"Loaded trained model {model_path}")
    except FileNotFoundError:
        print("Trained model not found, running with random weights for test")
    
    model.eval() # Extremely important for BatchNorm
    
    img_size = 256 # Match the dataloader's image size!
    T = 300       # from training configuration
    sample_plot_image(model, device, img_size, T)

# TODO：你需要在这个函数中实现图像的补充
# Follows: RePaint: Inpainting using Denoising Diffusion Probabilistic Models
@torch.no_grad()
def inpaint(model, device, img, mask, t_max=50):
    return img

# TODO: 你需要在这个函数中完成模型以及其他相关资源的加载，并调用inpaint进行图像补全，以生成图片
def test_image_inpainting():
    pass
    

if __name__ == "__main__":
    test_image_generation()
    test_image_inpainting()