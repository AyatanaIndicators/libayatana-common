using AyatanaCommon;
using Environment;
using GLib;

static int main(string[] args)
{
    assert(args.length == 2);

    unset_variable("XDG_CURRENT_DESKTOP");

    assert(! utils_is_lomiri());
    set_variable("XDG_CURRENT_DESKTOP", "Lomiri", true);
    assert(utils_is_lomiri());

    assert(! utils_is_gnome());
    set_variable("XDG_CURRENT_DESKTOP", "GNOME", true);
    assert(utils_is_gnome());

    assert(! utils_is_unity());
    set_variable("XDG_CURRENT_DESKTOP", "Unity", true);
    assert(utils_is_unity());

    assert(! utils_is_mate());
    set_variable("XDG_CURRENT_DESKTOP", "MATE", true);
    assert(utils_is_mate());

    assert(! utils_is_xfce());
    set_variable("XDG_CURRENT_DESKTOP", "XFCE", true);
    assert(utils_is_xfce());

    assert(! utils_is_pantheon());
    set_variable("XDG_CURRENT_DESKTOP", "PANTHEON", true);
    assert(utils_is_pantheon());

    assert(! utils_is_budgie());
    set_variable("XDG_CURRENT_DESKTOP", "Budgie:GNOME", true);
    assert(utils_is_budgie());

    unset_variable("XDG_CURRENT_DESKTOP");
    set_variable("GSETTINGS_SCHEMA_DIR", args[1], true);
    set_variable("GSETTINGS_BACKEND", "memory", true);

    Settings pSettings = new Settings("org.ayatana.common");
    string sTest = "123456789012345678901234567890123456789012345öüóőúéáűšđß";
    pSettings.set_uint("max-menu-text-length", 0);
    utils_ellipsize(sTest);
    assert(sTest == "123456789012345678901234567890123456789012345öüóőúéáűšđß");

    pSettings.set_uint("max-menu-text-length", 100);
    utils_ellipsize(sTest);
    assert(sTest == "123456789012345678901234567890123456789012345öüóőúéáűšđß");

    pSettings.set_uint("max-menu-text-length", 50);
    utils_ellipsize(sTest);
    assert(sTest == "123456789012345678901234567890123456789012345öüóőú...");

    return 0;
}
