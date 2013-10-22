/*
 * Copyright (c) 2002-2013 BalaBit IT Ltd, Budapest, Hungary
 * Copyright (c) 1998-2013 Viktor Juhász
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "generate-agent.h"
#include "generate.h"
#include "misc.h"
#include "apphook.h"

static const gchar *agent_registry_config = "@version: 5.0\n@module eventlog\n@module agent-config type(REGISTRY) name(ac)\noptions{ threaded(yes); };\nac()\n";
static const gchar *agent_xml_config_format = "@version: 5.0\n@module eventlog\n@module agent-config type(XML) file(\"%s\") name(ac)\noptions { threaded(yes); }; \nac()\n";

gint
generate_agent_main(gint argc, gchar **argv)
{
  gchar *filename;
  gchar *config_string;
  PersistTool *self;

  if (!generate_agent_output_dir)
    {
      fprintf(stderr,"Required option (output-dir) is missing\n");
      return 1;
    }

  if (!g_file_test(generate_agent_output_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))
    {
      fprintf(stderr, "Directory doesn't exist: %s\n", generate_agent_output_dir);
      return 1;
    }
  filename = g_build_path(G_DIR_SEPARATOR_S, generate_agent_output_dir, DEFAULT_PERSIST_FILE, NULL);
  if (g_file_test(filename, G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS) && !force_generate_agent)
    {
      fprintf(stderr, "Persist file exists; filename = %s\n",filename);
      return 1;
    }
  else if (g_file_test(filename, G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS))
    {
      if (unlink(filename) != 0)
        {
          fprintf(stderr, "Can't delete existing persist file; file = %s, error = %s\n", filename, strerror(errno));
          return 1;
        }
    }

  if (xml_config_file && !g_file_test(xml_config_file, G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS))
    {
      fprintf(stderr, "Given config file doesn't exists; filename = %s\n", xml_config_file);
      return 1;
    }

  if (xml_config_file)
    {
      gchar *correct_path_name = escape_windows_path(xml_config_file);
      config_string = g_strdup_printf(agent_xml_config_format,correct_path_name);
      g_free(correct_path_name);
    }
  else
    {
      config_string = g_strdup(agent_registry_config);
    }

  app_startup();
  generate_persist_file = TRUE;
  self = persist_tool_new(filename, persist_mode_edit);
  if (cfg_load_config(self->cfg, config_string, FALSE, NULL))
    {
      self->cfg->state = self->state;
      self->state = NULL;
      cfg_generate_persist_file(self->cfg);
      fprintf(stderr,"New persist file generated: %s\n", filename);
      persist_state_commit(self->cfg->state);
    }
  persist_tool_free(self);
  app_shutdown();

  g_free(config_string);
  g_free(filename);
  return 0;
}
