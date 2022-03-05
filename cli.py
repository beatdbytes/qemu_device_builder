from pathlib import Path
import click


@click.group(short_help='Generating the Qemu source code for system emulation', chain=True)
@click.option(
    '--output_folder',
    help='Output folder where the generated code is saved',
    type=click.Path(exists=True, file_okay=False, dir_okay=True),
    default=Path.cwd(),
    prompt=True

)
@click.pass_context
def tool_main(ctx, output_folder):
    ctx.ensure_object(dict)
    ctx.obj['OUTPUT'] = output_folder
