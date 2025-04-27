/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2025 Beaver modifying mostly Grok's work. Deep Seek helped a little oto

Recreate plugin using this GEGL syntax
ai/lb:color-cubes
id=1 color-to-alpha color=black  bevel depth=35 elevation=136 azimuth=110 dst-over aux=[ color value=black ] crop aux=[ ref=1 ] unsharp-mask scale=0.3

 */

#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES


property_double (cube_size, _("Cube Size"), 30.0)
    description (_("Size of each Tetris cube in pixels"))
    value_range (10.0, 200.0)
    ui_range (20.0, 200.0)

property_double (spacing, _("Grid Spacing"), 1.2)
    description (_("Spacing between Tetris cubes as a multiple of cube size"))
    value_range (1.0, 2.0)
    ui_range (1.0, 1.5)

property_double (rotation, _("Rotation"), 0.0)
    description (_("Rotation angle of Tetris cubes in degrees"))
    value_range (0.0, 360.0)
    ui_range (0.0, 360.0)


property_seed    (seed, _("Color Seed"), rand)
    description (_("Seed for randomizing Tetris cube colors"))


property_boolean (enable_bevel, _("Enable Bevel"), FALSE)
  description    (_("Enable/Disable a bevel over the tetris cubes"))

property_double (darken_bevel, _("Darken Bevel"), 1.0)
    description (_("Darken the bevel if you find it to bright"))
    value_range (1.0, 4.0)
    ui_range (1.0, 4.0)
  ui_meta     ("sensitive", " enable_bevel")

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     colorcubes2
#define GEGL_OP_C_SOURCE colorcubes2.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *tetris;
 GeglNode *light;
 GeglNode *bevelgraph;
 GeglNode *output;
}State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

#define g1 \
"   id=1 color-to-alpha color=black  bevel depth=35 elevation=136 azimuth=110 dst-over aux=[ color value=black ] crop aux=[ ref=1 ] unsharp-mask scale=0.3  "\


  state->bevelgraph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", g1,
                                  NULL);
  state->tetris    = gegl_node_new_child (gegl,
                                  "operation", "ai/lb:color-cubes-core",
                                  NULL);
  state->light    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gamma",
                                  NULL);

  gegl_operation_meta_redirect (operation, "cube_size",    state->tetris, "cube_size");
  gegl_operation_meta_redirect (operation, "rotation",    state->tetris, "rotation");
  gegl_operation_meta_redirect (operation, "spacing",    state->tetris, "spacing");
  gegl_operation_meta_redirect (operation, "seed",    state->tetris, "seed");
  gegl_operation_meta_redirect (operation, "darken_bevel",    state->light, "value");



}

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  if (o->enable_bevel)
  {
 gegl_node_link_many (state->input, state->tetris, state->bevelgraph, state->light, state->output, NULL);
  }
  else
  {
 gegl_node_link_many (state->input, state->tetris,  state->output, NULL);
  }
}



static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "ai/lb:color-cubes",
    "title",       _("Tetris inspired color cubes"),
    "reference-hash", "coloredcubesgrokanddeepseek",
    "description", _("Render tetris inspired color cubes"),
    "gimp:menu-path", "<Image>/Filters/AI GEGL",
    "gimp:menu-label", _("Tetris Color Cubes..."),
    NULL);
}

#endif
