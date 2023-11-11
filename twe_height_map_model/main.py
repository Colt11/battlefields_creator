from trainer.trainer import Trainer
from trainer.training_map_dataset import TrainingMapDataset
from creator.creator import Creator
from vae.vae_model import VaeModel
from vae.vae_height_map_data import VaeHeightMapData
from vae.vae_heigh_map_creator_dataset import VaeHeighMapCreatorDataset
import torch
from optparse import OptionParser


def get_separated_args(option, value, parser_instance):
    setattr(parser_instance.values, option.dest, value.split(','))


parser = OptionParser()
parser.add_option("--train", action="store_true", dest="train", default=False)
parser.add_option("--input-path", dest="input_path", type="string")
parser.add_option("--postfixes", dest="postfixes", type="string", action='callback', callback=get_separated_args)
parser.add_option("--batch-size", dest="batch_size", type="int")
parser.add_option("--lr", dest="lr", type="float")
parser.add_option("--epochs", dest="epochs", type="int")

parser.add_option("--create", action="store_true", dest="create", default=False)
parser.add_option("--in-image-path", dest="in_image_path", type="string")
parser.add_option("--create-weights-path", dest="create_weights_path", type="string")
parser.add_option("--out-image-path", dest="out_image_path", type="string")

parser.add_option("--export", action="store_true", dest="export", default=False)
parser.add_option("--export-weights-path", dest="export_weights_path", type="string")
parser.add_option("--out-model-path", dest="out_model_path", type="string")

(options, args) = parser.parse_args()


def height_map_model_training():
    model = VaeModel(True)
    trainer = Trainer(model, TrainingMapDataset(VaeHeightMapData, 1025, options.input_path, options.postfixes), options.lr)
    trainer.training(options.epochs, options.batch_size)


def height_map_model_creation():
    creator = Creator(VaeModel(False), VaeHeighMapCreatorDataset(options.in_image_path, 1025), options.create_weights_path, options.out_image_path)
    creator.create()


def height_map_model_export():
    model = VaeModel(False)
    model.load_state_dict(torch.load(options.export_weights_path))
    model_scripted = torch.jit.script(model)
    model_scripted.save(options.out_model_path)


if __name__ == '__main__':
    if options.train:
        height_map_model_training()

    if options.create:
        height_map_model_creation()

    if options.export:
        height_map_model_export()



