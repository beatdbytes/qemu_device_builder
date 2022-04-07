#ifndef {{ DataModel.Block.Name.upper() }}_TYPES_H
#define {{ DataModel.Block.Name.upper() }}TYPES_H

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

#endif //{{ DataModel.Block.Name.upper() }}TYPES_H