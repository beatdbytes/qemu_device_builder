/*
 * {{ DataModel.Block.Name }} Module
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
#include "qemu/osdep.h"
#include "hw/qdev-clock.h"
#include "hw/qdev-properties.h"
#include "hw/registerfields.h"
#include "migration/vmstate.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "qemu/units.h"

#include "hw/registerfields.h"
#include "hw/{{ DataModel.Arch }}/{{ DataModel.Block.Name.lower() }}.h"


#define {{ DataModel.Block.Name.upper() }}_TRACE(fmt,...) printf("{{ DataModel.Arch.title() }}::{{ DataModel.Block.Name }}::%s:"fmt,__func__,##__VA_ARGS__)
#define {{ DataModel.Block.Name.upper() }}_ERROR(fmt,...) printf("{{ DataModel.Arch.title() }}::{{ DataModel.Block.Name }}::%s:"fmt,__func__,##__VA_ARGS__)
OBJECT_DEFINE_TYPE({{ DataModel.Block.Name.title() }},{{ DataModel.Block.Name.lower() }},{{ DataModel.Block.Name.upper() }},SYS_BUS_DEVICE)

static void {{ DataModel.Block.Name.lower() }}_reset(DeviceState *dev);
static void {{ DataModel.Block.Name.lower() }}_realize(DeviceState *dev, Error **errp);
static void {{ DataModel.Block.Name.lower() }}_write(void* opaque, hwaddr offset, uint64_t value64, unsigned size);
static uint64_t {{ DataModel.Block.Name.lower() }}_read(void*opaque, hwaddr offset, unsigned size);

/* Register field definitions. */
{% for sfr in  DataModel.Block.SfrList %}
REG32({{ sfr.Name }},  {{ sfr.Addr }})
{% endfor %}

static const VMStateDescription {{ DataModel.Block.Name.lower() }}_vmstate = {
    .name = TYPE_{{ DataModel.Block.Name.upper() }},
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        {% for sfr in  DataModel.Block.SfrList %}
        VMSTATE_UINT32({{ sfr.Name }}.U, {{ DataModel.Block.Name.title()}}),
        {% endfor %}
        VMSTATE_END_OF_LIST(),
    },
};

static const struct MemoryRegionOps {{ DataModel.Block.Name.lower() }}_ops = {
    .read       = {{ DataModel.Block.Name.lower() }}_read,
    .write      = {{ DataModel.Block.Name.lower() }}_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid      = {
        .min_access_size        = 4,
        .max_access_size        = 4,
        .unaligned              = false,
    },
};
//Reset the state of the device
static void {{ DataModel.Block.Name.lower() }}_reset(DeviceState* dev)
{
    {{ DataModel.Block.Name.title() }} *s = {{ DataModel.Block.Name.upper() }}(dev);
    {% for sfr in  DataModel.Block.SfrList %}
    s->{{ sfr.Name }}.U = 0x{{sfr.RVal}};
    {% endfor %}
}

static uint64_t {{ DataModel.Block.Name.lower() }}_read(void *opaque, hwaddr offset, unsigned size)
{
    uint64_t value = 0;
    {{ DataModel.Block.Name.title() }} *s = {{ DataModel.Block.Name.upper() }}(opaque);

    switch (offset) {
    {% for sfr in  DataModel.Block.SfrList %}
       case A_{{ sfr.Name }}:
            {{ DataModel.Block.Name.upper() }}_TRACE("%s","Reading {{ sfr.Name }}\n");
            value = (uint64_t) s->{{ sfr.Name }}.U;
       break;
    {% endfor %}
    default:
         {{ DataModel.Block.Name.upper() }}_ERROR("invalid read at offset 0x%04" HWADDR_PRIx "\n", offset);
        break;
    }
    return value;
}
static void {{ DataModel.Block.Name.lower() }}_write(void *opaque, hwaddr offset, uint64_t v,  unsigned size)
{
   {{ DataModel.Block.Name.title() }}  *s = {{ DataModel.Block.Name.upper() }}(opaque);

    switch (offset) {
    {% for sfr in  DataModel.Block.SfrList %}
    {% if sfr.Writable %}
    case A_{{ sfr.Name }}:
       {{ DataModel.Block.Name.upper() }}_TRACE("%s","Writing {{ sfr.Name }}\n");
       s->{{ sfr.Name }}.U = (uint32_t)v;
       break;
     {% endif %}
    {% endfor %}
    default:
        {{ DataModel.Block.Name.upper() }}_ERROR("invalid write at offset 0x%04" HWADDR_PRIx "\n", offset);
        break;
    }
}
static void {{ DataModel.Block.Name.lower() }}_realize(DeviceState *dev, Error **errp){

}
static void {{ DataModel.Block.Name.lower() }}_finalize(Object* obj){

}
static void {{ DataModel.Block.Name.lower() }}_init(Object *obj){
    {{ DataModel.Block.Name.title() }}  *s = {{ DataModel.Block.Name.upper() }}(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    memory_region_init_io(&s->iomem, obj, &{{ DataModel.Block.Name.lower() }}_ops, s,
                          TYPE_{{ DataModel.Block.Name.upper() }}, {{ DataModel.Block.Size}});
    sysbus_init_mmio(sbd, &s->iomem);
}

static void {{ DataModel.Block.Name.lower() }}_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    dc->realize = {{ DataModel.Block.Name.lower() }}_realize;
    dc->reset = {{ DataModel.Block.Name.lower() }}_reset;
    dc->vmsd = &{{ DataModel.Block.Name.lower() }}_vmstate;
}