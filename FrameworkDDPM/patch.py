import re
with open('/home/hgl/code/ustc/USTC_CG/FrameworkDDPM/training_model.py', 'r') as f:
    content = f.read()

content = content.replace('''            if loss_value < best_loss:
                best_loss = loss_value
                torch.save(model.state_dict(), best_model_path)

            if batch_idx % 50 == 0:
                logging.info(f"Epoch {epoch} | Batch index {batch_idx:03d} Loss: {loss_value}")

        if epoch_steps > 0:
            epoch_mean_loss = epoch_loss_sum / epoch_steps
            logging.info(f"Epoch {epoch} | Mean Loss: {epoch_mean_loss:.6f} | Best Loss: {best_loss:.6f}")''',
'''            if batch_idx % 50 == 0:
                logging.info(f"Epoch {epoch} | Batch index {batch_idx:03d} Loss: {loss_value}")

        if epoch_steps > 0:
            epoch_mean_loss = epoch_loss_sum / epoch_steps
            if epoch_mean_loss < best_loss:
                best_loss = epoch_mean_loss
                torch.save(model.state_dict(), best_model_path)
            logging.info(f"Epoch {epoch} | Mean Loss: {epoch_mean_loss:.6f} | Best Loss: {best_loss:.6f}")''')

with open('/home/hgl/code/ustc/USTC_CG/FrameworkDDPM/training_model.py', 'w') as f:
    f.write(content)
