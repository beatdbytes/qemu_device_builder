/*
 * {{ DataModel.Block.Name }} Module
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

#include "qemu/osdep.h"
#include "{{ DataModel.Block.Name.lower() }}.h"
#include "hw/qdev-clock.h"
#include "hw/qdev-properties.h"
#include "hw/registerfields.h"
#include "migration/vmstate.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "qemu/units.h"
#include "trace.h"

#define {{ DataModel.Block.Name.upper() }}_TRACE(fmt,...) printf("{{ DataModel.Arch.title() }}::{{ DataModel.Block.Name }}::%s:"fmt,__func__,##__VA_ARGS__)
#define {{ DataModel.Block.Name.upper() }}_ERROR(fmt,...) printf("{{ DataModel.Arch.title() }}::{{ DataModel.Block.Name }}::%s:"fmt,__func__,##__VA_ARGS__)

/* Register field definitions. */

//Reset the state of the device
static void {{ DataModel.Block.Name.lower() }}_reset({{ DataModel.Block.Name }}State *s)
{
    {% for sfr in  DataModel.Block.SfrList %}
    s->{{ sfr.Name }} = 0x{{sfr.RVal}};
    {% endfor %}
}

static uint64_t {{ DataModel.Block.Name.lower() }}_read(void *opaque, hwaddr offset, unsigned size)
{
    uint64_t value = 0;
    {{ DataModel.Block.Name }}State *s = opaque;

    switch (offset) {
    {% for sfr in  DataModel.Block.SfrList %}
       case {{ sfr.Addr }}:
            value = s->{{ sfr.Name }};
       break;
    {% endfor %}
    default:
         {{ DataModel.Block.Name.upper() }}_ERROR("invalid offset 0x%04" HWADDR_PRIx "\n", offset);
        break;
    }
    return value;
}
{% for sfr in  DataModel.Block.SfrList %}
{% if sfr.Writable %}
static inline void {{ sfr.Name }}_write({{ DataModel.Block.Name }}State  *s, hwaddr offset, uint64_t v,  unsigned size)
{
    {{sfr.Name }}_Bitfields req_value;
    req_value.U = (uint32_t)v;
    {{sfr.Name }}_Bitfields * current_value = ({{sfr.Name }}_Bitfields*)&s->{{sfr.Name }};
    {% for field in sfr.BitFields%}
    {% if 'w' in field.Access %}
    if(req_value.{{ field.Name}} != current_value->{{ field.Name }})
    {
        {{ DataModel.Block.Name.upper() }}_TRACE("%s", "updating {{ sfr.Name }}.{{ field.Name }}\n" );
        current_value->{{ field.Name }} = req_value.{{ field.Name}};
    }
    {% endif %}
    {% endfor %}
    //To Be Updated with Logic
}
{% endif %}
{% endfor %}
static void {{ DataModel.Block.Name.lower() }}_write(void *opaque, hwaddr offset, uint64_t v,  unsigned size)
{
   {{ DataModel.Block.Name }}State  *s = opaque;

    switch (offset) {
    {% for sfr in  DataModel.Block.SfrList %}
    {% if sfr.Writable %}
    case {{ sfr.Addr }}:
       {{ sfr.Name }}_write(s,offset,v,size);
       break;
     {% endif %}
    {% endfor %}
    default:
        {{ DataModel.Block.Name.upper() }}_ERROR("invalid offset 0x%04" HWADDR_PRIx "\n", offset);
        break;
    }
}

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

static void {{ DataModel.Block.Name.lower() }}_enter_reset(Object *obj, ResetType type)
{
    {{ DataModel.Block.Name }}State *s = {{ DataModel.Block.Name.upper() }}(obj);

    {{ DataModel.Block.Name.lower() }}_reset(s);
}

static void {{ DataModel.Block.Name.lower() }}_hold_reset(Object *obj)
{

}

static void {{ DataModel.Block.Name.lower() }}_init(Object *obj)
{
    {{ DataModel.Block.Name }}State  *s = {{ DataModel.Block.Name.upper() }}(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    memory_region_init_io(&s->iomem, obj, &{{ DataModel.Block.Name.lower() }}_ops, s,
                          TYPE_{{ DataModel.Block.Name.upper() }}, {{ DataModel.Block.Size}});
    sysbus_init_mmio(sbd, &s->iomem);
}

static const VMStateDescription vmstate_{{ DataModel.Block.Name.lower() }} = {
    .name = "{{ DataModel.Block.Name }}",
    .version_id = 0,
    .minimum_version_id = 0,
    .fields = (VMStateField[]) {
        {% for sfr in  DataModel.Block.SfrList %}
        VMSTATE_UINT32({{ sfr.Name }}, {{ DataModel.Block.Name }}State),
        {% endfor %}
        VMSTATE_END_OF_LIST(),
    },
};

static Property {{ DataModel.Block.Name.lower() }}_properties[] = {
     DEFINE_PROP_END_OF_LIST(),
};

static void {{ DataModel.Block.Name.lower() }}_class_init(ObjectClass *klass, void *data)
{
    ResettableClass *rc = RESETTABLE_CLASS(klass);
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->desc = "{{ DataModel.Block.Name }} Module";
    dc->vmsd = &vmstate_{{ DataModel.Block.Name.lower() }};
    rc->phases.enter = {{ DataModel.Block.Name.lower() }}_enter_reset;
    rc->phases.hold = {{ DataModel.Block.Name.lower() }}_hold_reset;
    device_class_set_props(dc, {{ DataModel.Block.Name.lower() }}_properties);
}

static const TypeInfo {{ DataModel.Block.Name.lower() }}_info = {
    .name               = TYPE_{{ DataModel.Block.Name.upper() }},
    .parent             = TYPE_SYS_BUS_DEVICE,
    .instance_size      = sizeof({{ DataModel.Block.Name }}State),
    .class_init         = {{ DataModel.Block.Name.lower() }}_class_init,
    .instance_init      = {{ DataModel.Block.Name.lower() }}_init,
};

static void {{ DataModel.Block.Name.lower() }}_register_types(void)
{
    type_register_static(&{{ DataModel.Block.Name.lower() }}_info);
}

type_init({{ DataModel.Block.Name.lower() }}_register_types);