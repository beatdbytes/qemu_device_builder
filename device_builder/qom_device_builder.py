import os
from jinja2 import FileSystemLoader, Environment

from qemu_device_builder.qemu_device_model.device_model import DeviceModel
from qemu_device_builder.qemu_device_model.register_map_model import BLOCK


class QemuDeviceBuilder:
    def __init__(self, block: BLOCK, arch:str, output:str):
        self.Block = block
        self.Output = output
        template_folder = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates')
        file_loader = FileSystemLoader(template_folder)
        self.env = Environment(loader=file_loader, trim_blocks=True, lstrip_blocks=True)
        self.model = DeviceModel(block=block,architecture=arch)


    def gen_h(self):
        template = self.env.get_template('device.h')
        output = template.render(DataModel=self.model)
        device_name = f'{self.model.Block.Name.lower()}.h'
        self.create_file(data=output, output_folder=self.Output, file=device_name)

    def gen_c(self):
        template = self.env.get_template('device.c')
        output = template.render(DataModel=self.model)
        device_name = f'{self.model.Block.Name.lower()}.c'
        self.create_file(data=output, output_folder=self.Output, file=device_name)

    @staticmethod
    def create_file(data, output_folder, file):
        file_name = os.path.join(output_folder, file)
        f = open(file_name, "w")
        f.write(data)
        f.write('\n')
        f.close()



