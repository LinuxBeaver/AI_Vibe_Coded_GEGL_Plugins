#include "config.h"
#include <glib/gi18n-lib.h>
#include <math.h>
#include <gegl.h>
#include <gegl-plugin.h>

#ifdef GEGL_PROPERTIES

enum_start (gegl_grok2_num_colors)
  enum_value (GEGL_GROK2_NUM_COLORS_2, "two",   N_("2 Colors"))
  enum_value (GEGL_GROK2_NUM_COLORS_3, "three", N_("3 Colors"))
  enum_value (GEGL_GROK2_NUM_COLORS_4, "four",  N_("4 Colors"))
  enum_value (GEGL_GROK2_NUM_COLORS_5, "five",  N_("5 Colors"))
  enum_value (GEGL_GROK2_NUM_COLORS_6, "six",   N_("6 Colors"))
  enum_value (GEGL_GROK2_NUM_COLORS_7, "seven", N_("7 Colors"))
  enum_value (GEGL_GROK2_NUM_COLORS_8, "eight", N_("8 Colors"))
enum_end (GeglGrok2NumColors)

property_enum (num_colors, _("Number of Colors"),
               GeglGrok2NumColors, gegl_grok2_num_colors,
               GEGL_GROK2_NUM_COLORS_5)
    description (_("Number of colors to use in the spiral arms"))

property_color (color1, _("Spiral Color 1"), "#ff6179")
    description (_("First color of the spiral arms"))

property_color (color2, _("Spiral Color 2"), "#ffffff")
    description (_("Second color of the spiral arms"))

property_color (color3, _("Spiral Color 3"), "#c22485")
    description (_("Third color of the spiral arms"))
    ui_meta ("visible", "num_colors {three,four,five,six,seven,eight}")

property_color (color4, _("Spiral Color 4"), "#94009b")
    description (_("Fourth color of the spiral arms"))
    ui_meta ("visible", "num_colors {four,five,six,seven,eight}")

property_color (color5, _("Spiral Color 5"), "#f99dff")
    description (_("Fifth color of the spiral arms"))
    ui_meta ("visible", "num_colors {five,six,seven,eight}")

property_color (color6, _("Spiral Color 6"), "#c65fff")
    description (_("Sixth color of the spiral arms"))
    ui_meta ("visible", "num_colors {six,seven,eight}")

property_color (color7, _("Spiral Color 7"), "#ffda9e")
    description (_("Seventh color of the spiral arms"))
    ui_meta ("visible", "num_colors {seven,eight}")

property_color (color8, _("Spiral Color 8"), "#80ffe6")
    description (_("Eighth color of the spiral arms"))
    ui_meta ("visible", "num_colors {eight}")

property_color (bg_color, _("Background Color"), "black")
    description (_("Color of the background that will only be visible if the arm thickness is low"))
    ui_meta     ("role", "output-extent")

property_int (arms, _("Number of Arms"), 4)
    description (_("Number of spiral arms"))
    value_range (1, 10)
    ui_range (1, 8)

property_double (twist, _("Twist (starburst to spiral)"), 0.2)
    description (_("At 0 its a starburst and as you go up it will transition into a starburst and become tighter"))
    value_range (0.0, 1.0)
    ui_range (0.1, 0.5)

property_double (thickness, _("Arm Thickness"), 15.0)
    description (_("Thickness of the spiral arms"))
    value_range (0.5, 15.0)
    ui_range (2.5, 15.0)
    ui_meta     ("role", "output-extent")

property_double (x, _("X"), 0.5)
    description (_("X position of the spiral center (relative to image width)"))
    value_range (0.0, 1.0)

property_double (y, _("Y"), 0.5)
    description (_("Y position of the spiral center (relative to image height)"))
    value_range (0.0, 1.0)

property_boolean (ccw, _("Counter-Clockwise"), FALSE)
    description (_("Draw spiral counter-clockwise"))

#else

#define GEGL_OP_POINT_FILTER
#define GEGL_OP_NAME     candyspiral
#define GEGL_OP_C_SOURCE candyspiral.c

#include "gegl-op.h"

static void
prepare (GeglOperation *operation)
{
  gegl_operation_set_format (operation, "input", babl_format ("RGBA float"));
  gegl_operation_set_format (operation, "output", babl_format ("RGBA float"));
}

static gboolean
process (GeglOperation       *operation,
         void               *in_buf,
         void               *out_buf,
         glong               n_pixels,
         const GeglRectangle *roi,
         gint                level)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  gfloat *out_pixel = (gfloat *) out_buf;

  gfloat c1[4], c2[4], c3[4], c4[4], c5[4], c6[4], c7[4], c8[4], bg[4];
  gegl_color_get_pixel (o->color1, babl_format ("RGBA float"), c1);
  gegl_color_get_pixel (o->color2, babl_format ("RGBA float"), c2);
  gegl_color_get_pixel (o->color3, babl_format ("RGBA float"), c3);
  gegl_color_get_pixel (o->color4, babl_format ("RGBA float"), c4);
  gegl_color_get_pixel (o->color5, babl_format ("RGBA float"), c5);
  gegl_color_get_pixel (o->color6, babl_format ("RGBA float"), c6);
  gegl_color_get_pixel (o->color7, babl_format ("RGBA float"), c7);
  gegl_color_get_pixel (o->color8, babl_format ("RGBA float"), c8);
  gegl_color_get_pixel (o->bg_color, babl_format ("RGBA float"), bg);

  // Get full canvas dimensions
  GeglRectangle *canvas = gegl_operation_source_get_bounding_box (operation, "input");
  gfloat canvas_width = canvas ? canvas->width : roi->width;
  gfloat canvas_height = canvas ? canvas->height : roi->height;

  // Center relative to full canvas
  gfloat cx = o->x * canvas_width;
  gfloat cy = o->y * canvas_height;
  gfloat max_radius = sqrt (canvas_width * canvas_width + canvas_height * canvas_height) / 2.0;
  gfloat base_arm_width = G_PI / o->arms; // Base width per arm
  gfloat arm_width = base_arm_width * o->thickness; // Scale with thickness
  gint num_colors;
  switch (o->num_colors)
  {
    case GEGL_GROK2_NUM_COLORS_2: num_colors = 2; break;
    case GEGL_GROK2_NUM_COLORS_3: num_colors = 3; break;
    case GEGL_GROK2_NUM_COLORS_4: num_colors = 4; break;
    case GEGL_GROK2_NUM_COLORS_5: num_colors = 5; break;
    case GEGL_GROK2_NUM_COLORS_6: num_colors = 6; break;
    case GEGL_GROK2_NUM_COLORS_7: num_colors = 7; break;
    case GEGL_GROK2_NUM_COLORS_8: num_colors = 8; break;
    default: num_colors = 5; // Fallback
  }
  gfloat color_segment_width = base_arm_width / num_colors; // Width per color segment

  for (glong i = 0; i < n_pixels; i++)
  {
    // Compute global coordinates
    gint x = (i % roi->width) + roi->x;
    gint y = (i / roi->width) + roi->y;

    gfloat dx = x - cx;
    gfloat dy = y - cy;
    gfloat dist = sqrt (dx * dx + dy * dy);
    gfloat norm_dist = dist / max_radius;
    gfloat angle = atan2 (dy, dx);
    if (angle < 0)
      angle += 2.0 * G_PI;

    // Archimedean spiral: theta = k * r
    gfloat spiral_angle = norm_dist * o->twist * 2.0 * G_PI;
    if (o->ccw)
      spiral_angle = -spiral_angle;

    gfloat total_angle = angle + spiral_angle;
    if (total_angle < 0)
      total_angle += 2.0 * G_PI;

    // Normalize angle to current arm
    gfloat arm_angle = fmod (total_angle, 2.0 * G_PI / o->arms);
    if (arm_angle < 0)
      arm_angle += 2.0 * G_PI / o->arms;

    // Calculate color segment within arm
    gfloat color_angle = fmod (total_angle, 2.0 * G_PI / o->arms);
    if (color_angle < 0)
      color_angle += 2.0 * G_PI / o->arms;
    gint color_index = (gint) (color_angle / color_segment_width) % num_colors;
    gfloat *color;
    switch (color_index)
    {
      case 0: color = c1; break;
      case 1: color = c2; break;
      case 2: color = c3; break;
      case 3: color = c4; break;
      case 4: color = c5; break;
      case 5: color = c6; break;
      case 6: color = c7; break;
      case 7: color = c8; break;
      default: color = bg; // Fallback
    }

    // Hard-edged rendering
    if (arm_angle <= arm_width)
    {
      out_pixel[0] = color[0];
      out_pixel[1] = color[1];
      out_pixel[2] = color[2];
      out_pixel[3] = 1.0;
    }
    else
    {
      out_pixel[0] = bg[0];
      out_pixel[1] = bg[1];
      out_pixel[2] = bg[2];
      out_pixel[3] = bg[3];
    }

    out_pixel += 4;
  }

  return TRUE;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
  GeglOperationPointFilterClass *point_filter_class = GEGL_OPERATION_POINT_FILTER_CLASS (klass);

  operation_class->prepare = prepare;
  point_filter_class->process = process;

  gegl_operation_class_set_keys (operation_class,
    "name", "ai/lb:spiral",
    "title", _("Candy Spiral Starburst"),
    "reference-hash", "groks_candy_spiral",
    "description", _("Generates a multicolor Archimedean spiral with up to eight customizable colors, also when twist is at 0 it becomes a starburst effect"),
    "gimp:menu-path", "<Image>/Filters/AI GEGL",
    "gimp:menu-label", _("Candy Spiral..."),
    NULL);
}

#endif
