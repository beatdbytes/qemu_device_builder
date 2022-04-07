from cli import tool_main
from device_builder.cli import device_builder

commands = {
    'device_builder': device_builder
}


def _load_commands():
    for command in commands.values():
        tool_main.add_command(command)


if __name__ == '__main__':
    _load_commands()
    tool_main()