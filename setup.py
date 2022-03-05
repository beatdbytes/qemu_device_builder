__copyright__ = 'Copyright (C) beatdbytes.com(2022)'

from pathlib import Path
from setuptools import find_packages, setup

NAME = 'qemu_device_builder'
VERSION = '0.0.1'
DESCRIPTION = 'build qemu devices based on qemu object model'
REQUIRES_PYTHON = '>=3.7.7'
REQUIRED = [
    'beautifulsoup4',
    'bs4',
    'click',
    'colorama',
    'Jinja2',
    'lxml',
    'MarkupSafe',
    'soupsieve'
]
URL = 'www.beatdbytes.com'

##############################################################################
# Update only the section above
##############################################################################

here = Path(__file__).parent

# Import the README and use it as the long-description.
with here.joinpath('README.md').open(encoding='utf-8') as f:
    long_description = '\n' + f.read()

# Import the LICENSE and use it as the package license.
with here.joinpath('LICENSE').open(encoding='utf-8') as f:
    _license = '\n' + f.read()

setup(
    name=NAME,
    version=VERSION,
    description=DESCRIPTION,
    long_description=long_description,
    long_description_content_type='text/markdown',
    author='beatdbyte',
    author_email='beatdbyte@gmail.com',
    url=URL,
    python_requires=REQUIRES_PYTHON,
    packages=find_packages(exclude=('tests', 'docs', 'utilities')),
    entry_points={},
    install_requires=REQUIRED,
    include_package_data=True,
    package_data={'': ['templates/*.*']},
    license=_license,
    classifiers=[
        # Full list: https://pypi.org/classifiers
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9'
    ]
)
