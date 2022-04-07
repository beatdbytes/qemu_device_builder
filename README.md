#QemuDeviceBuilder
This python package helps in creating the qemu devices, boards and cpus based on qemu object model.
##Creating Qemu Device 
The script expects an XML description of the Device Register map. An example XML file is available  `Device.xml`
To create the QEMU Device driver implementation use the below command 

`python -m qemu_device_builder --output_folder <Path\To\Output folder> device-builder --device_name <BNAM field value as in XML file>> ----arch_name <architecture e.g. arm> ----register_map <Path\To\Device.xml>` 

for example if the `Device.xml` file is used the command will be as below

`python -m qemu_device_builder --output_folder ./output  device-builder --device_name DEVICE ----arch_name arm ----register_map ./Device.xml`
