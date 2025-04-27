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
 * 2025 Grok with Beaver's help (GEGL Hawaiian flowers) Beaver wrote code to aid a Grok plugin)
 * 
 * Test this plugin without installing by pasting this syntax into GIMP's GEGL graph filter
 * 

ai/lb:hawaiian-flowers-core flower-size=200
opacity value=2.9
lb:threshold-alpha
median-blur radius=0 abyss-policy=none

end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (flower_size, _("Flower Size"), 145.0)
  description (_("Diameter of larger flowers in pixels"))
  value_range (20.0, 200.0)
  ui_range (40.0, 200.0)

property_double (flower_spacing, _("Flower Spacing"), 40.0)
  description (_("Spacing between flowers in pixels"))
  value_range (5.0, 100.0)
  ui_range (5.0, 100.0)

property_double (size_ratio, _("Small Flower Size Ratio"), 0.6)
  description (_("Ratio of small flower size to large flower size"))
  value_range (0.2, 1.0)
  ui_range (0.2, 1.0)

property_double (rotation_variation, _("Rotation Variation"), 20.0)
  description (_("Random rotation variation per flower in degrees"))
  value_range (0.0, 90.0)
  ui_range (0.0, 90.0)

property_double (petal_scale, _("Petal Roundness"), 2.0)
  description (_("Controls petal shape: higher values make a teardrop-shaped petal"))
  value_range (0.5, 2.0)
  ui_range (0.5, 2.0)

property_color (petal_color, _("Petal Color"), "#ff4040")
  description (_("Color of the flower petals (e.g., red for hibiscus)"))

property_color (center_color, _("Center Color"), "#ffff00")
  description (_("Color of the flower center"))

property_boolean (enable_background, _("Enable background"), TRUE)
  description    (_("Enable/Disable a color fill background"))

property_color (background_color, _("Background Color"), "#68efef")
  description (_("Color of background"))
  ui_meta     ("sensitive", " enable_background")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     hawaiian_flowers_2
#define GEGL_OP_C_SOURCE hawaiian_flowers_2.c

#include "gegl-op.h"

/*starred nodes go inside typedef struct */

typedef struct
{
 GeglNode *input;
 GeglNode *flowers;
 GeglNode *median;
 GeglNode *opacity;
 GeglNode *threshold;
 GeglNode *behind;
 GeglNode *crop;
 GeglNode *idref;
 GeglNode *color;


 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

/*new child node list is here, this is where starred nodes get defined

 state->newchildname = gegl_node_new_child (gegl, "operation", "lb:name", NULL);*/
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

  state->opacity  = gegl_node_new_child (gegl,
                                          "operation", "gegl:opacity", "value", 2.9,
                                          NULL);
  state->threshold  = gegl_node_new_child (gegl,
                                          "operation", "lb:threshold-alpha", 
                                          NULL);

  state->median  = gegl_node_new_child (gegl,
                                          "operation", "gegl:median-blur", "radius", 0, "abyss-policy", 0,
                                          NULL);

  state->flowers  = gegl_node_new_child (gegl,
                                          "operation", "ai/lb:hawaiian-flowers-core", 
                                          NULL);

  state->behind  = gegl_node_new_child (gegl,
                                          "operation", "gegl:dst-over", 
                                          NULL);

  state->idref  = gegl_node_new_child (gegl,
                                          "operation", "gegl:nop", 
                                          NULL);

  state->crop = gegl_node_new_child (gegl,
                                          "operation", "gegl:crop", 
                                          NULL);


  GeglColor *embeddedcolor = gegl_color_new ("#68efef");

  state->color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color", "value", embeddedcolor, 
                                  NULL);

  gegl_operation_meta_redirect (operation, "background_color",    state->color, "value");
  gegl_operation_meta_redirect (operation, "flower_size",    state->flowers, "flower_size");
  gegl_operation_meta_redirect (operation, "flower_spacing",    state->flowers, "flower_spacing");
  gegl_operation_meta_redirect (operation, "size_ratio",    state->flowers, "size_ratio");
  gegl_operation_meta_redirect (operation, "rotation_variation",    state->flowers, "rotation_variation");
  gegl_operation_meta_redirect (operation, "petal_scale",    state->flowers, "petal_scale");
  gegl_operation_meta_redirect (operation, "petal_color",    state->flowers, "petal_color");
  gegl_operation_meta_redirect (operation, "center_color",    state->flowers, "center_color");


}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  if (o->enable_background)
  {
 gegl_node_link_many (state->input, state->flowers, state->opacity, state->threshold, state->median, state->behind, state->idref,  state->crop,  state->output, NULL);
 gegl_node_connect (state->behind, "aux", state->color, "output");
 gegl_node_connect (state->crop, "aux", state->idref, "output");

  }
  else
  {
 gegl_node_link_many (state->input, state->flowers, state->opacity, state->threshold,  state->median,   state->output, NULL);
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
    "name",           "ai/lb:hawaiian-flowers",
    "title",          _("Hawaiian Flowers"),
    "reference-hash", "groksfirstpublicgeglplugin",
    "description",
    _("Renders a stylized Hawaiian flower pattern with teardrop-shaped petals in a staggered grid"),
    "gimp:menu-path", "<Image>/Filters/AI GEGL",
    "gimp:menu-label", _("Hawaiian Flowers..."),
    NULL);
}

#endif
