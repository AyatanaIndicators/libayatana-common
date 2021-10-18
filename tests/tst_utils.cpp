/*
 * Copyright 2021 Marius Gripsgard <marius@ubports.com>
 * Copyright 2021 Robert Tari <robert@tari.in>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <memory>

extern "C" {
    #include "utils.h"
}

class XdgCurrentDesktopUtilsTest : public ::testing::Test
{
public:
   XdgCurrentDesktopUtilsTest() {}

   void SetUp() {
       unsetenv("XDG_CURRENT_DESKTOP");
       unsetenv("DESKTOP_SESSION");
   }

   void TearDown() {
       unsetenv("XDG_CURRENT_DESKTOP");
       unsetenv("DESKTOP_SESSION");
   }
};

TEST_F(XdgCurrentDesktopUtilsTest, isLomiri)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_lomiri());
    setenv("DESKTOP_SESSION", "ubuntu-touch", 1);
    EXPECT_TRUE(ayatana_common_utils_is_lomiri());
    setenv("XDG_CURRENT_DESKTOP", "Lomiri", 1);
    EXPECT_TRUE(ayatana_common_utils_is_lomiri());
}

TEST_F(XdgCurrentDesktopUtilsTest, isGnome)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_gnome());
    setenv("DESKTOP_SESSION", "gnome", 1);
    EXPECT_TRUE(ayatana_common_utils_is_gnome());
    setenv("XDG_CURRENT_DESKTOP", "GNOME", 1);
    EXPECT_TRUE(ayatana_common_utils_is_gnome());
}
TEST_F(XdgCurrentDesktopUtilsTest, isUnity)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_unity());
    setenv("DESKTOP_SESSION", "gnome-fallback", 1);
    EXPECT_FALSE(ayatana_common_utils_is_unity());
    setenv("XDG_CURRENT_DESKTOP", "Unity", 1);
    EXPECT_TRUE(ayatana_common_utils_is_unity());
}
TEST_F(XdgCurrentDesktopUtilsTest, isMate)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_mate());
    setenv("DESKTOP_SESSION", "mate", 1);
    EXPECT_TRUE(ayatana_common_utils_is_mate());
    setenv("XDG_CURRENT_DESKTOP", "MATE", 1);
    EXPECT_TRUE(ayatana_common_utils_is_mate());
}
TEST_F(XdgCurrentDesktopUtilsTest, isXfce)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_xfce());
    setenv("DESKTOP_SESSION", "xfce", 1);
    EXPECT_TRUE(ayatana_common_utils_is_xfce());
    setenv("XDG_CURRENT_DESKTOP", "XFCE", 1);
    EXPECT_TRUE(ayatana_common_utils_is_xfce());
}
TEST_F(XdgCurrentDesktopUtilsTest, isPantheon)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_pantheon());
    setenv("XDG_CURRENT_DESKTOP", "PANTHEON", 1);
    EXPECT_TRUE(ayatana_common_utils_is_pantheon());
}
TEST_F(XdgCurrentDesktopUtilsTest, isBudgie)
{
    unsetenv("XDG_CURRENT_DESKTOP");
    unsetenv("DESKTOP_SESSION");
    EXPECT_FALSE(ayatana_common_utils_is_budgie());
    setenv("DESKTOP_SESSION", "budgie-desktop", 1);
    EXPECT_TRUE(ayatana_common_utils_is_budgie());
    setenv("XDG_CURRENT_DESKTOP", "Budgie:GNOME", 1);
    EXPECT_TRUE(ayatana_common_utils_is_budgie());
}

class StringFunctionsTest : public ::testing::Test
{
public:

    StringFunctionsTest()
    {
    }

    void SetUp()
    {
        g_setenv("GSETTINGS_SCHEMA_DIR", SCHEMA_DIR, TRUE);
        g_setenv("GSETTINGS_BACKEND", "memory", TRUE);
    }

    void TearDown()
    {
    }
};

TEST_F(StringFunctionsTest, ellipsize)
{
    GSettings *pSettings = g_settings_new("org.ayatana.common");
    gchar *sTest = g_strdup("123456789012345678901234567890123456789012345öüóőúéáűšđß");
    g_settings_set_uint(pSettings, "max-menu-text-length", 0);
    ayatana_common_utils_ellipsize(sTest);

    EXPECT_STREQ(sTest, "123456789012345678901234567890123456789012345öüóőúéáűšđß");

    g_settings_set_uint(pSettings, "max-menu-text-length", 100);
    ayatana_common_utils_ellipsize(sTest);

    EXPECT_STREQ(sTest, "123456789012345678901234567890123456789012345öüóőúéáűšđß");

    g_settings_set_uint(pSettings, "max-menu-text-length", 50);
    ayatana_common_utils_ellipsize(sTest);

    EXPECT_STREQ(sTest, "123456789012345678901234567890123456789012345öüóőú...");

    g_free(sTest);
    g_object_unref(pSettings);
}
