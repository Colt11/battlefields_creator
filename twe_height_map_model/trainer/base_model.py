import torch.nn as nn


class BaseModel(nn.Module):
    def __init__(self, is_training):
        super(BaseModel, self).__init__()
        self.is_training = is_training
        pass


    def get_loss(self, labels, forward_out_params):
        return ()


    def print_loss_param(self, loss_param):
        return ""


    def get_name(self):
        return "BaseModel"


    def get_is_training(self):
        return self.is_training
