from forward_noising import forward_diffusion_sample
from unet import SimpleUnet
from dataloader import load_transformed_dataset
import torch.nn.functional as F
import torch
from torch.optim import Adam
import logging
import os

logging.basicConfig(level=logging.INFO)

# TODO: 完成训练过程的Loss计算
# 加噪过程需要补充forward_diffusion_sample中内容，并调用
def get_loss(model, x_0, t, device):
    x_noisy, noise = forward_diffusion_sample(x_0, t, device)
    noise_pred = model(x_noisy.to(device), t.to(device))
    return F.mse_loss(noise.to(device), noise_pred)


if __name__ == "__main__":
    model = SimpleUnet()
    T = 300
    BATCH_SIZE = 1
    epochs = 5000
    TARGET_LOSS = 0.015

    dataloader = load_transformed_dataset(batch_size=BATCH_SIZE)

    device = "cuda" if torch.cuda.is_available() else "cpu"
    # device = "cpu"
    logging.info(f"Using device: {device}")
    model.to(device)
    model.train()
    optimizer = Adam(model.parameters(), lr=1e-4)

    current_dir = os.path.dirname(os.path.abspath(__file__))
    best_model_path = os.path.join(current_dir, "ddpm_mse_best.pth")
    last_model_path = os.path.join(current_dir, f"ddpm_mse_epochs_{epochs}.pth")
    best_loss = float("inf")
    stop_training = False

    for epoch in range(epochs):
        epoch_loss_sum = 0.0
        epoch_steps = 0
        for batch_idx, (batch, _) in enumerate(dataloader):
            optimizer.zero_grad()

            # TODO: 完成对时间步的采样、Loss计算以及反向传播
            t = torch.randint(0, T, (BATCH_SIZE,), device=device).long()
            loss = get_loss(model, batch, t, device)
            loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
            optimizer.step()

            loss_value = loss.item()
            epoch_loss_sum += loss_value
            epoch_steps += 1

            if batch_idx % 50 == 0:
                logging.info(f"Epoch {epoch} | Batch index {batch_idx:03d} Loss: {loss_value}")

        if epoch_steps > 0:
            epoch_mean_loss = epoch_loss_sum / epoch_steps
            if epoch_mean_loss < best_loss:
                best_loss = epoch_mean_loss
                torch.save(model.state_dict(), best_model_path)
            logging.info(f"Epoch {epoch} | Mean Loss: {epoch_mean_loss:.6f} | Best Loss: {best_loss:.6f}")

            if epoch_mean_loss <= TARGET_LOSS:
                logging.info(f"Early stop: epoch mean loss {epoch_mean_loss:.6f} <= target {TARGET_LOSS}")
                stop_training = True

        if stop_training:
            break

    torch.save(model.state_dict(), last_model_path)
    logging.info(f"Saved best model to: {best_model_path}")
    logging.info(f"Saved last model to: {last_model_path}")
