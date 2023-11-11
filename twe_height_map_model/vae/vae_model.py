from trainer.base_model import BaseModel
import torch
import torch.nn as nn


class Flatten(nn.Module):
    def forward(self, input):
        return input.view(input.size(0), -1)


class UnFlatten(nn.Module):
    def __init__(self, h_dim):
        super(UnFlatten, self).__init__()
        self.h_dim = h_dim


    def forward(self, input):
        return input.view(input.size(0), self.h_dim, 1, 1)


class VaeModel(BaseModel):
    def __init__(self, is_training):
        super(VaeModel, self).__init__(is_training)

        h_dim = 9216
        z_dim = 256

        self.encoder = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size=5, stride=2),
            nn.BatchNorm2d(16),
            nn.ReLU(),
            nn.MaxPool2d(2, 2),
            nn.Conv2d(16, 64, kernel_size=5, stride=2),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.MaxPool2d(2, 2),
            nn.Conv2d(64, 256, kernel_size=5, stride=2),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.MaxPool2d(2, 2),
            nn.Conv2d(256, 1024, kernel_size=5, stride=2),
            nn.BatchNorm2d(1024),
            nn.ReLU(),
            nn.MaxPool2d(2, 2),
            Flatten()
        )
        self.decoder = nn.Sequential(
            UnFlatten(h_dim),
            nn.ConvTranspose2d(h_dim, 1024, kernel_size=5, stride=3),
            nn.BatchNorm2d(1024),
            nn.ReLU(),
            nn.ConvTranspose2d(1024, 256, kernel_size=8, stride=5),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.ConvTranspose2d(256, 64, kernel_size=5, stride=4),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.ConvTranspose2d(64, 16, kernel_size=5, stride=3),
            nn.BatchNorm2d(16),
            nn.ReLU(),
            nn.ConvTranspose2d(16, 1, kernel_size=5, stride=3),
            nn.Sigmoid(),
        )
        self.fc1 = nn.Linear(h_dim, z_dim)
        self.fc2 = nn.Linear(h_dim, z_dim)
        self.fc3 = nn.Linear(z_dim, h_dim)


    def reparameterize(self, mu, log_var):
        std = log_var.mul(0.5).exp_()
        esp = torch.randn_like(mu).cuda()
        z = mu + std * esp
        return z


    def forward(self, x):
        h = self.encoder(x)
        mu = self.fc1(h)
        log_var = self.fc2(h)
        if self.get_is_training():
            z = self.reparameterize(mu, log_var)
            z = self.fc3(z)
        else:
            z = self.fc3(mu)
        predict = self.decoder(z)
        return predict, mu, log_var


    def get_loss(self, labels, forward_out_params):
        predict, mu, log_var = forward_out_params

        l1_loss = nn.functional.smooth_l1_loss(predict, labels, reduction='sum')

        kl = -0.5 * torch.sum(1 + log_var - mu.pow(2) - log_var.exp())
        loss = (kl + l1_loss) / labels.shape[0]
        return loss, l1_loss / labels.shape[0], kl / labels.shape[0]


    def print_loss_param(self, loss_param):
        return "Loss: {:.3f} L1Loss: {:.3f} KL: {:.3f} ".format(loss_param[0].data, loss_param[1].data, loss_param[2].data)


    def get_name(self):
        return "vae"
