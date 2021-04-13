/*******************************************************************************
 * Size: 32 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#include <lvgl.h>

#ifndef FREESANSOBLIQUE32
#define FREESANSOBLIQUE32 1
#endif

#if FREESANSOBLIQUE32

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002B "+" */
    0x0, 0xe0, 0x0, 0xc0, 0x0, 0xc0, 0x0, 0xc0,
    0x0, 0xc0, 0x1, 0xc0, 0x1, 0x80, 0xff, 0xff,
    0xff, 0xff, 0x3, 0x80, 0x3, 0x80, 0x3, 0x0,
    0x3, 0x0, 0x3, 0x0, 0x7, 0x0, 0x7, 0x0,

    /* U+002C "," */
    0x7b, 0xdc, 0x23, 0x11, 0x98,

    /* U+002D "-" */
    0xff, 0xff,

    /* U+002E "." */
    0xfe, 0xe0,

    /* U+0030 "0" */
    0x0, 0xf8, 0x3, 0xfe, 0x7, 0xe, 0xe, 0x7,
    0x1c, 0x7, 0x18, 0x7, 0x38, 0x7, 0x38, 0x7,
    0x70, 0x7, 0x70, 0x7, 0x70, 0x7, 0x70, 0x6,
    0x60, 0xe, 0xe0, 0xe, 0xe0, 0xe, 0xe0, 0x1c,
    0xe0, 0x1c, 0xe0, 0x18, 0xe0, 0x38, 0x70, 0x70,
    0x7f, 0xe0, 0x3f, 0xc0, 0x4, 0x0,

    /* U+0031 "1" */
    0x1, 0x80, 0xc0, 0xe0, 0xe1, 0xf7, 0xfb, 0xfc,
    0xc, 0xe, 0x7, 0x3, 0x81, 0xc0, 0xc0, 0xe0,
    0x70, 0x38, 0x1c, 0x1c, 0xe, 0x7, 0x3, 0x81,
    0x81, 0xc0,

    /* U+0032 "2" */
    0x0, 0x7f, 0x0, 0x3f, 0xf0, 0xf, 0xf, 0x3,
    0x80, 0xf0, 0x70, 0xe, 0x1c, 0x1, 0xc3, 0x80,
    0x38, 0x0, 0xe, 0x0, 0x1, 0xc0, 0x0, 0x70,
    0x0, 0x3c, 0x0, 0x1f, 0x0, 0xf, 0x80, 0x7,
    0xc0, 0x1, 0xe0, 0x0, 0xf0, 0x0, 0x1c, 0x0,
    0x7, 0x0, 0x1, 0xc0, 0x0, 0x3f, 0xff, 0x87,
    0xff, 0xe1, 0xff, 0xfc, 0x0,

    /* U+0033 "3" */
    0x0, 0xfc, 0x1, 0xff, 0x81, 0xe1, 0xe1, 0xc0,
    0x70, 0xc0, 0x38, 0xe0, 0x1c, 0x70, 0xe, 0x0,
    0xe, 0x0, 0xe, 0x0, 0x7e, 0x0, 0x7f, 0x0,
    0x3, 0xc0, 0x0, 0xf0, 0x0, 0x38, 0x0, 0x1d,
    0xc0, 0xe, 0xe0, 0x6, 0x70, 0x7, 0x38, 0x7,
    0xe, 0x7, 0x87, 0xff, 0x81, 0xff, 0x0, 0x8,
    0x0,

    /* U+0034 "4" */
    0x0, 0x3, 0x0, 0x7, 0x0, 0xf, 0x0, 0x1e,
    0x0, 0x3e, 0x0, 0x7e, 0x0, 0xee, 0x1, 0xce,
    0x3, 0x8c, 0x7, 0x1c, 0xe, 0x1c, 0x1c, 0x1c,
    0x38, 0x18, 0x70, 0x38, 0x70, 0x38, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x70, 0x0, 0x70, 0x0, 0x70,
    0x0, 0x70, 0x0, 0x70, 0x0, 0x60,

    /* U+0035 "5" */
    0x3, 0xff, 0xc0, 0xff, 0xf0, 0x7f, 0xfc, 0x1c,
    0x0, 0x6, 0x0, 0x3, 0x80, 0x0, 0xe0, 0x0,
    0x30, 0x0, 0x1d, 0xf8, 0x7, 0xff, 0x81, 0xe1,
    0xe0, 0xe0, 0x1c, 0x0, 0x7, 0x0, 0x1, 0xc0,
    0x0, 0x70, 0x0, 0x1c, 0x0, 0x7, 0x38, 0x1,
    0x8e, 0x0, 0xe3, 0x80, 0x70, 0x70, 0x38, 0x1f,
    0xfc, 0x3, 0xfe, 0x0, 0x8, 0x0,

    /* U+0036 "6" */
    0x0, 0xfc, 0x1, 0xff, 0x1, 0xc3, 0xc1, 0xc0,
    0xe1, 0xc0, 0x38, 0xc0, 0x0, 0xe0, 0x0, 0x60,
    0x0, 0x71, 0xf0, 0x3b, 0xfe, 0x1f, 0x7, 0xf,
    0x1, 0xcf, 0x0, 0xe7, 0x0, 0x73, 0x80, 0x39,
    0xc0, 0x1c, 0xe0, 0xc, 0x70, 0xe, 0x38, 0x6,
    0xe, 0x7, 0x7, 0xff, 0x1, 0xfe, 0x0, 0x18,
    0x0,

    /* U+0037 "7" */
    0x3f, 0xff, 0x9f, 0xff, 0xcf, 0xff, 0xe0, 0x0,
    0xe0, 0x0, 0x60, 0x0, 0x60, 0x0, 0x60, 0x0,
    0x60, 0x0, 0x70, 0x0, 0x70, 0x0, 0x70, 0x0,
    0x30, 0x0, 0x38, 0x0, 0x38, 0x0, 0x38, 0x0,
    0x1c, 0x0, 0x1c, 0x0, 0xe, 0x0, 0xe, 0x0,
    0x7, 0x0, 0x7, 0x0, 0x3, 0x80, 0x0,

    /* U+0038 "8" */
    0x0, 0xfc, 0x1, 0xff, 0x81, 0xe1, 0xe1, 0xc0,
    0x70, 0xe0, 0x18, 0x60, 0x1c, 0x70, 0xe, 0x38,
    0xf, 0xe, 0xf, 0x3, 0xff, 0x3, 0xff, 0x3,
    0xc3, 0xc3, 0x80, 0xe3, 0x80, 0x39, 0xc0, 0x1c,
    0xc0, 0xe, 0xe0, 0x7, 0x70, 0x7, 0x1c, 0x3,
    0x8e, 0x3, 0x83, 0xff, 0x80, 0xff, 0x0, 0xc,
    0x0,

    /* U+0039 "9" */
    0x1, 0xf8, 0x7, 0xfc, 0xe, 0x1e, 0x1c, 0x7,
    0x38, 0x7, 0x30, 0x7, 0x70, 0x7, 0x70, 0x7,
    0x70, 0x7, 0x70, 0xf, 0x70, 0x1f, 0x38, 0x3e,
    0x3f, 0xfe, 0xf, 0xce, 0x0, 0xe, 0x0, 0x1c,
    0x0, 0x1c, 0xe0, 0x38, 0xe0, 0x38, 0xe0, 0x70,
    0x7f, 0xe0, 0x3f, 0x80, 0x4, 0x0,

    /* U+0048 "H" */
    0xf, 0x0, 0x1c, 0x1c, 0x0, 0x38, 0x38, 0x0,
    0x70, 0x70, 0x0, 0xe0, 0xe0, 0x1, 0xc3, 0x80,
    0x7, 0x7, 0x0, 0xe, 0xe, 0x0, 0x1c, 0x1c,
    0x0, 0x38, 0x38, 0x0, 0x70, 0xff, 0xff, 0xc1,
    0xff, 0xff, 0x83, 0x80, 0x7, 0x7, 0x0, 0xe,
    0x1c, 0x0, 0x38, 0x38, 0x0, 0x70, 0x70, 0x0,
    0xe0, 0xe0, 0x1, 0xc1, 0xc0, 0x3, 0x87, 0x0,
    0xe, 0xe, 0x0, 0x1c, 0x1c, 0x0, 0x38, 0x38,
    0x0, 0x70, 0x0,

    /* U+004B "K" */
    0xe, 0x0, 0x1c, 0x1c, 0x0, 0xf0, 0x38, 0x3,
    0xc0, 0x70, 0xf, 0x0, 0xe0, 0x3c, 0x3, 0x80,
    0xe0, 0x7, 0x7, 0x80, 0xe, 0x1e, 0x0, 0x1c,
    0x78, 0x0, 0x71, 0xe0, 0x0, 0xef, 0xc0, 0x1,
    0xff, 0x80, 0x3, 0xf3, 0x80, 0x7, 0x87, 0x0,
    0x1e, 0x7, 0x0, 0x38, 0xe, 0x0, 0x70, 0xe,
    0x0, 0xe0, 0x1c, 0x3, 0xc0, 0x3c, 0x7, 0x0,
    0x38, 0xe, 0x0, 0x78, 0x1c, 0x0, 0x70, 0x38,
    0x0, 0xf0, 0x0,

    /* U+004D "M" */
    0x7, 0xc0, 0x1, 0xe0, 0xf8, 0x0, 0x7c, 0x1f,
    0x0, 0xf, 0x83, 0xe0, 0x3, 0xf0, 0xfc, 0x0,
    0x7c, 0x1d, 0x80, 0x1f, 0x83, 0xb0, 0x7, 0x70,
    0x76, 0x0, 0xee, 0xc, 0xe0, 0x39, 0x83, 0x9c,
    0x7, 0x70, 0x73, 0x81, 0xce, 0xe, 0x70, 0x31,
    0xc1, 0xce, 0xe, 0x38, 0x70, 0xc1, 0x86, 0xe,
    0x18, 0x71, 0xc1, 0xc3, 0x1c, 0x38, 0x38, 0x73,
    0x87, 0x6, 0xe, 0xe0, 0xe1, 0xc1, 0xd8, 0x38,
    0x38, 0x3f, 0x7, 0x7, 0x3, 0xc0, 0xe0, 0xe0,
    0x78, 0x1c, 0x18, 0xe, 0x3, 0x0,

    /* U+005A "Z" */
    0x3, 0xff, 0xfc, 0xf, 0xff, 0xf8, 0x0, 0x0,
    0xf0, 0x0, 0x3, 0xc0, 0x0, 0xf, 0x0, 0x0,
    0x3c, 0x0, 0x0, 0xf0, 0x0, 0x3, 0xc0, 0x0,
    0xf, 0x0, 0x0, 0x3c, 0x0, 0x0, 0xf0, 0x0,
    0x3, 0xc0, 0x0, 0xf, 0x80, 0x0, 0x3e, 0x0,
    0x0, 0xf8, 0x0, 0x3, 0xe0, 0x0, 0xf, 0x80,
    0x0, 0x3e, 0x0, 0x0, 0xf8, 0x0, 0x3, 0xe0,
    0x0, 0xf, 0x80, 0x0, 0x1f, 0xff, 0xf8, 0x3f,
    0xff, 0xf0, 0x0,

    /* U+0068 "h" */
    0x7, 0x0, 0x7, 0x0, 0x6, 0x0, 0xe, 0x0,
    0xe, 0x0, 0xe, 0x0, 0xc, 0x7c, 0xd, 0xff,
    0x1f, 0x8f, 0x1e, 0x7, 0x1c, 0x3, 0x1c, 0x7,
    0x38, 0x7, 0x38, 0x7, 0x38, 0x7, 0x38, 0x6,
    0x30, 0xe, 0x70, 0xe, 0x70, 0xe, 0x70, 0xc,
    0x60, 0xc, 0x60, 0x1c, 0xe0, 0x1c,

    /* U+006B "k" */
    0x6, 0x0, 0x7, 0x0, 0x3, 0x80, 0x1, 0xc0,
    0x0, 0xe0, 0x0, 0x60, 0x0, 0x70, 0x1c, 0x38,
    0x3c, 0x1c, 0x3c, 0xc, 0x38, 0x6, 0x38, 0x7,
    0x38, 0x3, 0xfc, 0x1, 0xfe, 0x0, 0xf3, 0x80,
    0xf1, 0xc0, 0x70, 0xe0, 0x38, 0x38, 0x1c, 0x1c,
    0xc, 0x7, 0xe, 0x3, 0x87, 0x1, 0xc3, 0x80,
    0x70,

    /* U+006D "m" */
    0xc, 0x7c, 0x3f, 0x6, 0xff, 0x3f, 0xc7, 0xc3,
    0xf0, 0xe3, 0xc0, 0xf0, 0x71, 0xc0, 0x70, 0x38,
    0xc0, 0x70, 0x1c, 0xe0, 0x38, 0xe, 0x70, 0x1c,
    0x7, 0x38, 0xe, 0x3, 0x1c, 0x6, 0x3, 0x8c,
    0x7, 0x1, 0xce, 0x3, 0x80, 0xe7, 0x1, 0xc0,
    0x73, 0x80, 0xe0, 0x31, 0x80, 0x60, 0x38, 0xc0,
    0x70, 0x1c, 0xe0, 0x38, 0xe, 0x0,

    /* U+007A "z" */
    0xf, 0xff, 0x87, 0xff, 0xc0, 0x1, 0xc0, 0x1,
    0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1,
    0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1,
    0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1,
    0xff, 0xf8, 0xff, 0xfc, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 299, .box_w = 16, .box_h = 16, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 32, .adv_w = 142, .box_w = 5, .box_h = 8, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 37, .adv_w = 170, .box_w = 8, .box_h = 2, .ofs_x = 3, .ofs_y = 8},
    {.bitmap_index = 39, .adv_w = 142, .box_w = 4, .box_h = 3, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 41, .adv_w = 285, .box_w = 16, .box_h = 23, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 87, .adv_w = 285, .box_w = 9, .box_h = 23, .ofs_x = 7, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 285, .box_w = 19, .box_h = 22, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 285, .box_w = 17, .box_h = 23, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 215, .adv_w = 285, .box_w = 16, .box_h = 23, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 261, .adv_w = 285, .box_w = 18, .box_h = 24, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 315, .adv_w = 285, .box_w = 17, .box_h = 23, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 364, .adv_w = 285, .box_w = 17, .box_h = 22, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 285, .box_w = 17, .box_h = 23, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 460, .adv_w = 285, .box_w = 16, .box_h = 23, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 506, .adv_w = 370, .box_w = 23, .box_h = 23, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 342, .box_w = 23, .box_h = 23, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 426, .box_w = 27, .box_h = 23, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 718, .adv_w = 313, .box_w = 23, .box_h = 23, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 785, .adv_w = 285, .box_w = 16, .box_h = 23, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 256, .box_w = 17, .box_h = 23, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 426, .box_w = 25, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 934, .adv_w = 256, .box_w = 17, .box_h = 17, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x1, 0x2, 0x3, 0x5, 0x6, 0x7, 0x8,
    0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0x1d, 0x20,
    0x22, 0x2f, 0x3d, 0x40, 0x42, 0x4f
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 43, .range_length = 80, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 22, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    3, 18,
    3, 21,
    3, 22,
    16, 21,
    16, 22,
    18, 21,
    18, 22,
    19, 21,
    19, 22,
    20, 21,
    20, 22,
    21, 21,
    21, 22,
    22, 21,
    22, 22
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -31, -20, -20, -20, -20, -20, -20, -13,
    -13, -10, -10, -13, -13, -15, -15
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 15,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t FreeSansOblique32 = {
#else
lv_font_t FreeSansOblique32 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 28,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -6,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FREESANSOBLIQUE32*/

