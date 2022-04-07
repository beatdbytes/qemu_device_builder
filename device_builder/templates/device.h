/*
 * {{ DataModel.Block.Name }} Module in {{ DataModel.Arch }}
 *
 * Copyright @ 2021 {{ DataModel.current_usr  }} beatdbytes.com.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */


#ifndef {{ DataModel.Block.Name.upper() }}_H
#define {{ DataModel.Block.Name.upper() }}_H
#include "hw/clock.h"
#include "hw/irq.h"
#include "hw/sysbus.h"
#include "qemu/timer.h"
#include "hw/ptimer.h"
#include "{{ DataModel.Block.Name.lower() }}_types.h"
#include "qom/object.h"
#define TYPE_{{ DataModel.Block.Name.upper() }} "{{ DataModel.Block.Name.lower() }}"
OBJECT_DECLARE_TYPE({{ DataModel.Block.Name.title() }},{{ DataModel.Block.Name.title() }}Class,{{ DataModel.Block.Name.upper() }})
 struct {{ DataModel.Block.Name.title() }}{
    SysBusDevice parent;
    MemoryRegion iomem;
    {% for sfr in  DataModel.Block.SfrList %}
    {{sfr.Name }}_Bitfields {{ sfr.Name }}; //{{ sfr.Desc }}
    {% endfor %}
};

struct {{ DataModel.Block.Name.title() }}Class{
    SysBusDeviceClass parent_class;
};
#endif //{{ DataModel.Block.Name }}_H