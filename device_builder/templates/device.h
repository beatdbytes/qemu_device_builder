/*
 * {{ DataModel.Block.Name }} Module in {{ DataModel.Arch }}
 *
 * Copyright @ 2021 {{ DataModel.current_usr  }} Infineon Technologies A.G.
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
#define {{ DataModel.Block.Name }}_H
#include "hw/clock.h"
#include "hw/irq.h"
#include "hw/sysbus.h"
#include "qemu/timer.h"
typedef struct {
    SysBusDevice parent;
    MemoryRegion iomem;
    {% for sfr in  DataModel.Block.SfrList %}
    uint32_t {{ sfr.Name }}; //{{ sfr.Desc }}
    {% endfor %}
} {{ DataModel.Block.Name }}State;

{% for sfr in  DataModel.Block.SfrList %}
//Bitfields in register {{sfr.Name }}
typedef union{
    struct{
{% for field in sfr.BitFields%}
        uint32_t {{ field.Name }}:{{ field.Width }};//Start: {{field.Start}} End: {{ field.End }}
{% endfor %}
    };
    uint32_t U;
}{{sfr.Name }}_Bitfields;

{% endfor %}

#define TYPE_{{ DataModel.Block.Name.upper() }} "{{ DataModel.Block.Name }}"
#define {{ DataModel.Block.Name.upper() }}(obj) \
    OBJECT_CHECK({{ DataModel.Block.Name }}State, (obj), TYPE_{{ DataModel.Block.Name.upper() }})

#endif //{{ DataModel.Block.Name }}_H