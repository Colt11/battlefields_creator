import torch
from torch.utils.data import DataLoader
import torch.optim as optim


class Trainer:
    def __init__(self, model, data_set, lr):
        self.model = model
        self.data_set = data_set
        self.lr = lr

    def training(self, num_epochs=100, batch_size=1):
        assert self.model.get_is_training()

        training_data_loader = DataLoader(self.data_set, batch_size=batch_size, shuffle=True)
        optimizer = optim.Adam(self.model.parameters(), lr=self.lr)

        self.model.to(torch.cuda.current_device())

        print("Start training model: ")
        print(self.model)

        for epoch in range(num_epochs):
            for i, data in enumerate(training_data_loader):
                images, labels = data
                forward_out_params = self.model(images.cuda())
                loss_param = self.model.get_loss(labels.cuda(), forward_out_params)
                loss = loss_param[0]

                optimizer.zero_grad()
                loss.backward()
                optimizer.step()
                if i % 1 == 0:
                    to_print = "Epoch[{}/{}] Data[{}/{}] ".format(epoch + 1, num_epochs, i * batch_size, len(training_data_loader.dataset))
                    print(to_print + self.model.print_loss_param(loss_param))

            torch.save(self.model.state_dict(),  self.model.get_name() + "_" + str(epoch) + ".ckpt")

        torch.save(self.model.state_dict(), self.model.get_name() + ".ckpt")
