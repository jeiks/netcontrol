function checkAll(flag)
{
    $("div#frm_setSquidGroups INPUT[@id=gruposSquid][type='checkbox']").attr('checked', (flag == 1) ? true : false);
}

