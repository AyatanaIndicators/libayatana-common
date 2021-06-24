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

#include "utils.h"

#include <string.h>

#ifdef HAS_URLDISPATCHER
# include <lomiri-url-dispatcher.h>
#endif

// TODO: make case insensitive
gboolean
is_xdg_current_desktop (const gchar* desktop)
{
  const gchar *xdg_current_desktop;
  gchar **desktop_names;
  int i;

  xdg_current_desktop = g_getenv ("XDG_CURRENT_DESKTOP");
  if (xdg_current_desktop != NULL) {
    desktop_names = g_strsplit (xdg_current_desktop, ":", 0);
    for (i = 0; desktop_names[i]; ++i) {
      if (!g_strcmp0 (desktop_names[i], desktop)) {
        g_strfreev (desktop_names);
        return TRUE;
      }
    }
    g_strfreev (desktop_names);
  }
  return FALSE;
}

gboolean
ayatana_common_utils_is_lomiri ()
{
  // For legacy reasons keep the MIR_SOCKET hack
  return (g_getenv ("MIR_SOCKET") != NULL ||
          is_xdg_current_desktop(DESKTOP_LOMIRI));
}

gboolean
ayatana_common_utils_is_gnome ()
{
  return is_xdg_current_desktop(DESKTOP_GNOME);
}

gboolean
ayatana_common_utils_is_unity ()
{
  return is_xdg_current_desktop(DESKTOP_UNITY);
}

gboolean
ayatana_common_utils_is_mate ()
{
  return is_xdg_current_desktop(DESKTOP_MATE);
}

gboolean
ayatana_common_utils_is_xfce ()
{
  return is_xdg_current_desktop(DESKTOP_XFCE);
}

gboolean
ayatana_common_utils_is_pantheon ()
{
  return is_xdg_current_desktop(DESKTOP_PANTHEON);
}

gboolean
ayatana_common_utils_is_budgie ()
{
  return is_xdg_current_desktop(DESKTOP_BUDGIE);
}

gboolean
ayatana_common_utils_have_unity_program (const gchar *program)
{
  if (!ayatana_common_utils_is_unity())
    return FALSE;

  return ayatana_common_utils_have_program(program);
}

gboolean
ayatana_common_utils_have_gnome_program (const gchar *program)
{
  if (!ayatana_common_utils_is_gnome())
    return FALSE;

  return ayatana_common_utils_have_program(program);
}

gboolean
ayatana_common_utils_have_mate_program (const gchar *program)
{
  if (!ayatana_common_utils_is_mate())
    return FALSE;

  return ayatana_common_utils_have_program(program);
}

gboolean
ayatana_common_utils_have_xfce_program (const gchar *program)
{
  if (!ayatana_common_utils_is_xfce())
    return FALSE;

  return ayatana_common_utils_have_program(program);
}

gboolean
ayatana_common_utils_have_pantheon_program (const gchar *program)
{
  if (!ayatana_common_utils_is_pantheon())
    return FALSE;

  return ayatana_common_utils_have_program(program);
}

gboolean
ayatana_common_utils_have_budgie_program (const gchar *program)
{
  if (!ayatana_common_utils_is_budgie())
    return FALSE;

  return ayatana_common_utils_have_program(program);
}


// Bit of a hacky way? should use xdg open
char *
find_browser ()
{
  static char * browser_path = NULL;
  char* tmp_browser_path;
  gchar **browser_names;

  int i;

  if (browser_path == NULL)
  {
    browser_names = g_strsplit ("x-www-browser,google-chrome,firefox,chromium", ",", 0);

    for (i = 0; browser_names[i]; ++i) {
      tmp_browser_path = g_find_program_in_path (browser_names[i]);

      if (tmp_browser_path) {
        browser_path = g_strdup (tmp_browser_path);
        g_free (tmp_browser_path);
        g_strfreev (browser_names);
        break;
      }
    }
  }

  return browser_path;
}

gboolean
ayatana_common_utils_execute_command (const gchar * cmd)
{
  GError * err = NULL;

  g_debug ("Issuing command '%s'", cmd);

  if (!g_spawn_command_line_async (cmd, &err))
  {
    g_warning ("Unable to start %s: %s", cmd, err->message);
    g_error_free (err);
    return FALSE;
  }

  return TRUE;
}

gboolean
ayatana_common_utils_open_url (const gchar * url)
{
  char * browser = NULL;

  if (ayatana_common_utils_is_lomiri())
  {
#ifdef HAS_URLDISPATCHER
    lomiri_url_dispatch_send(url, NULL, NULL);
    return TRUE;
#else
    g_warning("Built without url-dispatcher, not able to open URL '%s'", url);
#endif
  }

  if (browser == NULL)
    browser = find_browser();

  if (browser != NULL)
    return ayatana_common_utils_execute_command(g_strdup_printf("%s '%s'", browser, url));
  else
    return FALSE;

}

gboolean
ayatana_common_utils_have_program (const gchar * program)
{
  gchar *path;
  gboolean have;

  path = g_find_program_in_path(program);
  have = path != NULL;
  g_free(path);

  return have;
}

gboolean
ayatana_common_utils_zenity_warning (const char * icon_name,
                const char * title,
                const char * text)
{
  char * command_line;
  int exit_status;
  GError * error;
  gboolean confirmed;
  char * zenity;

  confirmed = FALSE;
  zenity = g_find_program_in_path ("zenity");

  if (zenity)
    {
      command_line = g_strdup_printf ("%s"
                                      " --warning"
                                      " --icon-name=\"%s\""
                                      " --title=\"%s\""
                                      " --text=\"%s\""
                                      " --no-wrap",
                                      zenity,
                                      icon_name,
                                      title,
                                      text);

      /* Treat errors as user confirmation.
         Otherwise how will the user ever log out? */
      exit_status = -1;
      error = NULL;
      if (!g_spawn_command_line_sync (command_line, NULL, NULL, &exit_status, &error))
        {
          confirmed = TRUE;
        }
      else
        {
          confirmed = g_spawn_check_exit_status (exit_status, &error);
        }

      g_free (command_line);
    }
  g_free (zenity);
  return confirmed;
}

void ayatana_common_utils_ellipsize(char *sText)
{
    guint nMaxLetters = 50;
    glong nLetters = g_utf8_strlen(sText, -1);
    GSettingsSchemaSource *pSource = g_settings_schema_source_get_default();

    if (pSource != NULL)
    {
        GSettingsSchema *pSchema = g_settings_schema_source_lookup(pSource, "org.ayatana.common", FALSE);

        if (pSchema != NULL)
        {
            g_settings_schema_unref(pSchema);
            GSettings *pSettings = g_settings_new("org.ayatana.common");
            nMaxLetters = g_settings_get_uint(pSettings, "max-menu-text-length");
            g_object_unref(pSettings);
        }
    }

    if (nMaxLetters > 0 && nLetters > nMaxLetters + 4)
    {
        gchar *pLastChar = g_utf8_offset_to_pointer(sText, nMaxLetters);
        memcpy(pLastChar, "...\0", 4);
    }
}
