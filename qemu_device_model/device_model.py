from qemu_device_builder.qemu_device_model.register_map_model import BLOCK
import getpass


class DeviceModel:
    def __init__(self, block: BLOCK, architecture: str):
        self.Block = block
        self.Block.Size = str(((int(self.Block.End,16) - int(self.Block.Start,16))+1))
        self.Arch = architecture
        self.DeviceName = {self.Block.Name}
        self.Block.Name = f'{self.Arch.title()}_{self.Block.Name}'
        print(self.Block.Name)
        self.current_usr = getpass.getuser()
        self._update_sfr_addr()

    def _update_sfr_addr(self):
        for sfr in self.Block.SfrList:
            sfr.Addr = hex((int(sfr.Addr, 16) - int(self.Block.Start, 16)))

