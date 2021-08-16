using AyatanaCommon;
using Environment;

static int main(string[] args)
{
    assert_true(args.length == 2);

    unset_variable("XDG_CURRENT_DESKTOP");

    assert_false(utils_is_lomiri());
    set_variable("XDG_CURRENT_DESKTOP", "Lomiri", true);
    assert_true(utils_is_lomiri());

    assert_false(utils_is_gnome());
    set_variable("XDG_CURRENT_DESKTOP", "GNOME", true);
    assert_true(utils_is_gnome());

    assert_false(utils_is_unity());
    set_variable("XDG_CURRENT_DESKTOP", "Unity", true);
    assert_true(utils_is_unity());

    assert_false(utils_is_mate());
    set_variable("XDG_CURRENT_DESKTOP", "MATE", true);
    assert_true(utils_is_mate());

    assert_false(utils_is_xfce());
    set_variable("XDG_CURRENT_DESKTOP", "XFCE", true);
    assert_true(utils_is_xfce());

    assert_false(utils_is_pantheon());
    set_variable("XDG_CURRENT_DESKTOP", "PANTHEON", true);
    assert_true(utils_is_pantheon());

    assert_false(utils_is_budgie());
    set_variable("XDG_CURRENT_DESKTOP", "Budgie:GNOME", true);
    assert_true(utils_is_budgie());

    unset_variable("XDG_CURRENT_DESKTOP");
    set_variable("GSETTINGS_SCHEMA_DIR", args[1], true);
    set_variable("GSETTINGS_BACKEND", "memory", true);

    Settings pSettings = new Settings("org.ayatana.common");
    string sTest = "123456789012345678901234567890123456789012345öüóőúéáűšđß";
    pSettings.set_uint("max-menu-text-length", 0);
    utils_ellipsize(sTest);
    assert_true(sTest == "123456789012345678901234567890123456789012345öüóőúéáűšđß");

    pSettings.set_uint("max-menu-text-length", 100);
    utils_ellipsize(sTest);
    assert_true(sTest == "123456789012345678901234567890123456789012345öüóőúéáűšđß");

    pSettings.set_uint("max-menu-text-length", 50);
    utils_ellipsize(sTest);
    assert_true(sTest == "123456789012345678901234567890123456789012345öüóőú...");

    return 0;
}
