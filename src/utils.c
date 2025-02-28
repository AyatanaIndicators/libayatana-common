/*
 * Copyright 2021 Marius Gripsgard <marius@ubports.com>
 * Copyright 2021-2023 Robert Tari <robert@tari.in>
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
#include <glib/gi18n-lib.h>
#include <string.h>

#ifdef LOMIRI_FEATURES_ENABLED
# include <lomiri-url-dispatcher.h>
#endif

static gboolean bI18nInit = FALSE;

static void initI18n ()
{
    if (!bI18nInit)
    {
        bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
        bI18nInit = TRUE;
    }
}

// TODO: make case insensitive
gboolean
is_xdg_current_desktop (const gchar* desktop, const gchar* session)
{
  const gchar *xdg_current_desktop;

  xdg_current_desktop = g_getenv ("XDG_CURRENT_DESKTOP");
  if (xdg_current_desktop != NULL) {
    gchar **desktop_names = g_strsplit (xdg_current_desktop, ":", 0);
    for (int i = 0; desktop_names[i]; ++i) {
      if (!g_strcmp0 (desktop_names[i], desktop)) {
        g_strfreev (desktop_names);
        return TRUE;
      }
    }
    g_strfreev (desktop_names);
  }

  if (session != NULL)
  {
    const gchar *desktop_session = g_getenv ("DESKTOP_SESSION");

    if (desktop_session != NULL && g_str_equal(desktop_session, session))
    {
        return TRUE;
    }

  }

  return FALSE;
}

gboolean
ayatana_common_utils_is_lomiri ()
{
  return is_xdg_current_desktop(DESKTOP_LOMIRI, SESSION_LOMIRI);
}

gboolean
ayatana_common_utils_is_ubuntutouch ()
{
  return is_xdg_current_desktop(DESKTOP_UBUNTUTOUCH, SESSION_UBUNTUTOUCH);
}

gboolean
ayatana_common_utils_is_gnome ()
{
  return is_xdg_current_desktop(DESKTOP_GNOME, SESSION_GNOME);
}

gboolean
ayatana_common_utils_is_unity ()
{
  return is_xdg_current_desktop(DESKTOP_UNITY, SESSION_UNITY);
}

gboolean
ayatana_common_utils_is_mate ()
{
  return is_xdg_current_desktop(DESKTOP_MATE, SESSION_MATE);
}

gboolean
ayatana_common_utils_is_xfce ()
{
  return is_xdg_current_desktop(DESKTOP_XFCE, SESSION_XFCE);
}

gboolean
ayatana_common_utils_is_pantheon ()
{
  return is_xdg_current_desktop(DESKTOP_PANTHEON, SESSION_PANTHEON);
}

gboolean
ayatana_common_utils_is_budgie ()
{
  return is_xdg_current_desktop(DESKTOP_BUDGIE, SESSION_BUDGIE);
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

  if (browser_path == NULL)
  {
    gchar **browser_names = g_strsplit ("x-www-browser,google-chrome,firefox,chromium", ",", 0);

    for (int i = 0; browser_names[i]; ++i) {
      char* tmp_browser_path = g_find_program_in_path (browser_names[i]);

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
  if (ayatana_common_utils_is_lomiri())
  {
#ifdef LOMIRI_FEATURES_ENABLED
    lomiri_url_dispatch_send(url, NULL, NULL);
    return TRUE;
#else
    g_warning("Built without url-dispatcher, not able to open URL '%s'", url);
#endif
  }

  char * browser = find_browser();

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
        #if GLIB_CHECK_VERSION(2, 70, 0)
          confirmed = g_spawn_check_wait_status (exit_status, &error);
        #else
          confirmed = g_spawn_check_exit_status (exit_status, &error);
        #endif
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

gboolean ayatana_common_utils_execute_command_warn (const gchar *sProgram, const gchar *sArgs)
{
    gboolean bHasProgram = ayatana_common_utils_have_program (sProgram);

    if (!bHasProgram)
    {
        initI18n ();
        gchar *sMessage = g_strdup_printf (_("The %s program is required for this action, but it was not found."), sProgram);
        ayatana_common_utils_zenity_warning ("dialog-warning", _("Warning"), sMessage);
        g_free (sMessage);

        return FALSE;
    }

    gchar *sCommand = g_strdup_printf ("%s %s", sProgram, sArgs);
    gboolean bSuccess = ayatana_common_utils_execute_command (sCommand);
    g_free (sCommand);

    return bSuccess;
}

gboolean ayatana_common_utils_zenity_question (const gchar *sIcon, const gchar *sTitle, const gchar *sText, const gchar *sOk, const gchar *sCancel)
{
    gchar *sZenity = g_find_program_in_path ("zenity");
    gchar *sCommand = g_strdup_printf ("%s --question --icon-name=\"%s\" --title=\"%s\" --text=\"%s\" --ok-label=\"%s\" --cancel-label=\"%s\" --no-wrap", sZenity, sIcon, sTitle, sText, sOk, sCancel);
    g_free (sZenity);
    int nStatus = -1;
    GError *pError = NULL;
    gboolean bConfirmed = FALSE;

    if (!g_spawn_command_line_sync (sCommand, NULL, NULL, &nStatus, &pError))
    {
        bConfirmed = TRUE;
    }
    else
    {
        #if GLIB_CHECK_VERSION(2, 70, 0)
            bConfirmed = g_spawn_check_wait_status (nStatus, &pError);
        #else
            bConfirmed = g_spawn_check_exit_status (nStatus, &pError);
        #endif
    }

    if (pError)
    {
        if (!g_error_matches (pError, G_IO_ERROR, G_IO_ERROR_CANCELLED))
        {
            g_warning ("%s %s: %s", G_STRLOC, G_STRFUNC, pError->message);
        }

        g_clear_error (&pError);
    }

    g_free (sCommand);

    return bConfirmed;
}
