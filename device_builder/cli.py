import os
from pathlib import Path

import click

from device_builder.qom_device_builder import QemuDeviceBuilder
from register_map_parser.parser import Parser


@click.command()
@click.option(
    '--device_name',
    help='name of the qemu device',
    type=str,
    prompt=True
)
@click.option(
    '--arch_name',
    help='name of the architecture',
    type=str,
    prompt=True
)
@click.option(
    '--register_map',
    help='Name of the register map XML file',
    type=click.Path(exists=True, file_okay=True, dir_okay=False),
    prompt=True

)
@click.pass_context
def device_builder(ctx, device_name, arch_name, register_map):
    p = Parser(Path(register_map))
    blocks = p.get_block_with_name(device_name)
    for block in blocks:
        output = ctx.obj['OUTPUT']
        if not os.path.exists(output):
            os.makedirs(output)
        qom = QemuDeviceBuilder(block=block, arch=arch_name, output=output)
        qom.gen_c()
        qom.gen_h()

