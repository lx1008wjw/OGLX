/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxReadBinaryFonts.c
 DESCRIPTION       : oglxReadBinaryFonts provides an example of binary fonts reader.
 COPYRIGHT (C)     : 2016 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

#include "oglxReadBinaryFonts.h"
#include "sgl.h"
#include "glf.h"

/******************************************************************************
 **                           Keywords used in binary font format
 *****************************************************************************/

#define KW_BEG_STRUCT 0x6C
#define KW_END_STRUCT 0x6D
#define KW_BEG_ARRAY  0x6E
#define KW_END_ARRAY  0x6F

#define KW_TYP_UINT8 0x90
#define KW_TYP_UINT16 0x92
#define KW_TYP_UINT32 0x94
#define KW_TYP_FLOAT 0x98

#define KW_TYP_FONT_TABLE 0xA0
#define KW_TYP_FONT_TABLE_HEADER 0xA1
#define KW_TYP_FONT 0xA2
#define KW_TYP_MAP_PROPERTY 0xA3
#define KW_TYP_CHARACTER 0xA4
#define KW_TYP_CHARACTER_STROKE 0xA5
#define KW_TYP_U_V_COUPLE 0xA6
#define KW_TYP_CHARACTER_BITMAP1 0xB1
#define KW_TYP_CHARACTER_BITMAP2 0xB2
#define KW_TYP_CHARACTER_BITMAP3 0xB3
#define KW_TYP_CHARACTER_BITMAP4 0xB4
#define KW_TYP_CHARACTER_BITMAP5 0xB5
#define KW_TYP_CHARACTER_BITMAP6 0xB6
#define KW_TYP_CHARACTER_BITMAP7 0xB7
#define KW_TYP_CHARACTER_BITMAP8 0xB8
#define KW_TYP_CHARACTER_BITMAP9 0xB9


/******************************************************************************
 **                           Basic reading
 *****************************************************************************/

static oglxReadByteFunc global_read_byte = NULL;

static SGLuint8 oglxReadUint8()
{
    return (SGLuint8) global_read_byte();
}

static SGLuint16 oglxReadUint16()
{
    SGLuint16 u1 = oglxReadUint8();
    SGLuint16 u2 = oglxReadUint8();
    return ((u1 << 8) | u2);
}

static SGLuint32 oglxReadUint32()
{
    SGLuint32 u1 = oglxReadUint16();
    SGLuint32 u2 = oglxReadUint16();
    return ((u1 << 16) | u2);
}

static SGLfloat oglxReadFloat()
{
    union {
        SGLfloat fl;
        SGLuint32 in;
    } u;

    u.in = oglxReadUint32();
    return u.fl;
}

static void oglxSkip(SGLuint32 nb_bytes)
{
    while (nb_bytes > 0U) {
        (void) global_read_byte();
        nb_bytes--;
    }
}

/******************************************************************************
 **                           Blocks reading
 *****************************************************************************/

static SGLbool oglxReadBeginStruct(SGLuint8 kind)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_BEG_STRUCT) || (kw2 != kind))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadEndStruct(SGLuint8 kind)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_END_STRUCT) || (kw2 != kind))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadBeginArray(SGLuint8 kind, SGLuint32 nb)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_BEG_ARRAY) || (kw2 != kind))
        return SGL_FALSE;
    return (oglxReadUint32() == nb);
}

static SGLbool oglxReadBeginAnyArray(SGLuint8 * kind, SGLuint32 nb)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if (kw1 != KW_BEG_ARRAY)
        return SGL_FALSE;
    *kind = kw2;
    return (oglxReadUint32() == nb);
}

static SGLbool oglxReadBeginPartialArray(SGLuint8 kind, SGLuint32 nbmax, SGLuint32 * nb)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_BEG_ARRAY) || (kw2 != kind))
        return SGL_FALSE;
    *nb = oglxReadUint32();
    return (*nb <= nbmax);
}

static SGLbool oglxReadEndArray(SGLuint8 kind)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_END_ARRAY) || (kw2 != kind))
        return SGL_FALSE;
    return SGL_TRUE;
}

/******************************************************************************
 **                           GLF structures reading
 *****************************************************************************/

static SGLbool oglxReadFontDescription(glf_type_font * p)
{
    SGLuint32 i, nb;
    /* Begin struct glf_type_font */
    if (!oglxReadBeginStruct(KW_TYP_FONT))
        return SGL_FALSE;
    /* Font name */
    if (!oglxReadBeginPartialArray(KW_TYP_UINT8, GLF_FONT_NAME_LENGTH, &nb))
        return SGL_FALSE;
    for (i = 0U; i < nb; i++) {
        p->font_name[i] = (char) oglxReadUint8();
    }
    if (!oglxReadEndArray(KW_TYP_UINT8))
        return SGL_FALSE;
    /* Char size */
    p->us_char_size = oglxReadUint16();
    /* Number of chars */
    p->us_char_number = oglxReadUint16();
    /* Font type */
    p->us_font_kind = oglxReadUint16();
    /* Line widths availability */
    if (!oglxReadBeginPartialArray(KW_TYP_UINT8, GLF_LINE_WIDTH_NUMBER, &nb))
        return SGL_FALSE;
    for (i = 0U; i < nb; i++) {
        p->tb_available_widths[i] = (char) oglxReadUint8();
    }
    if (!oglxReadEndArray(KW_TYP_UINT8))
        return SGL_FALSE;
    /* Line widths delta */
    if (!oglxReadBeginPartialArray(KW_TYP_UINT8, GLF_LINE_WIDTH_NUMBER, &nb))
        return SGL_FALSE;
    for (i = 0U; i < nb; i++) {
        p->tub_delta_widths[i] = (char) oglxReadUint8();
    }
    if (!oglxReadEndArray(KW_TYP_UINT8))
        return SGL_FALSE;
    /* Halo line widths delta */
    if (!oglxReadBeginPartialArray(KW_TYP_UINT8, GLF_LINE_WIDTH_NUMBER, &nb))
        return SGL_FALSE;
    for (i = 0U; i < nb; i++) {
        p->tub_delta_halo_widths[i] = (char) oglxReadUint8();
    }
    if (!oglxReadEndArray(KW_TYP_UINT8))
        return SGL_FALSE;
    /* Padding for 64-bit alignment */
    if (!oglxReadBeginPartialArray(KW_TYP_UINT8, 4U, &nb))
        return SGL_FALSE;
    for (i = 0U; i < nb; i++) {
        p->_pad1_[i] = (char) oglxReadUint8();
    }
    if (!oglxReadEndArray(KW_TYP_UINT8))
        return SGL_FALSE;
    /* Underline */
    p->f_underline_y = oglxReadFloat();
    /* Stroke */
    p->f_stroke_y = oglxReadFloat();
    /* Maximum char width */
    p->f_max_char_width = oglxReadFloat();
    /* Maximum char height */
    p->f_max_char_height = oglxReadFloat();
    /* Maximum horizontal advance */
    p->f_max_h_advance = oglxReadFloat();
    /* Maximum vertical advance */
    p->f_max_v_advance = oglxReadFloat();
    /* Offset to chars description */
    p->ui_offset_characters = oglxReadUint32(); /* Need to be recomputed later, depend of architecture */
    /* Offset to vectorial descriptions */
    p->ui_offset_strokes = oglxReadUint32();    /* Need to be recomputed later, depend of architecture */
    /* Delta to chars description */
    if (!oglxReadBeginPartialArray(KW_TYP_UINT16, GLF_MAX_NUMBER_OF_CHAR, &nb))
        return SGL_FALSE;
    for (i = 0U; i < nb; i++) {
        p->tus_delta_chars[i] = oglxReadUint16();
    }
    if (!oglxReadEndArray(KW_TYP_UINT16))
        return SGL_FALSE;
    /* End struct glf_type_font */
    if (!oglxReadEndStruct(KW_TYP_FONT))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadMapProperty(glf_type_map_property * p)
{
    /* Begin struct glf_type_map_property */
    if (!oglxReadBeginStruct(KW_TYP_MAP_PROPERTY))
        return SGL_FALSE;
    /* Skip pointer to texture data (ptub_map_data) */
    oglxSkip(8U);
    /* Map width */
    p->ui_map_width = oglxReadUint32();
    /* Map height */
    p->ui_map_height = oglxReadUint32();
    /* Map size */
    p->ui_map_size = oglxReadUint32();
    /* End struct glf_type_map_property */
    if (!oglxReadEndStruct(KW_TYP_MAP_PROPERTY))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadCharacter(glf_type_character * p)
{
    /* Begin struct glf_type_character */
    if (!oglxReadBeginStruct(KW_TYP_CHARACTER))
        return SGL_FALSE;
    /* Codepoint */
    p->ui_codepoint = oglxReadUint32();
    /* Character size */
    p->f_width = oglxReadFloat();
    p->f_height = oglxReadFloat();
    /* Distance to next character when writing horizontally */
    p->f_horizontal_advance = oglxReadFloat();
    /* Distance to next character when writing vertically */
    p->f_vertical_advance = oglxReadFloat();
    /* End struct glf_type_character */
    if (!oglxReadEndStruct(KW_TYP_CHARACTER))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadCharacterStroke(glf_type_character_stroke * p)
{
    /* Begin struct glf_type_character_stroke */
    if (!oglxReadBeginStruct(KW_TYP_CHARACTER_STROKE))
        return SGL_FALSE;
    /* Character */
    if (!oglxReadCharacter(&(p->tr_type_character)))
        return SGL_FALSE;
    /* Offset to vertices description */
    p->ui_offset_to_description = oglxReadUint32();
    /* End struct glf_type_character_stroke */
    if (!oglxReadEndStruct(KW_TYP_CHARACTER_STROKE))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadUVCouple(glf_type_u_v_couple * p)
{
    /* Begin struct glf_type_u_v_couple */
    if (!oglxReadBeginStruct(KW_TYP_U_V_COUPLE))
        return SGL_FALSE;
    /* Bitmap bounding box */
    p->s_x = (SGLint16) oglxReadUint16();
    p->s_y = (SGLint16) oglxReadUint16();
    p->us_width = oglxReadUint16();
    p->us_height = oglxReadUint16();
    /* Texture map index */
    p->ui_map_index = oglxReadUint32();
    /* Texture coordinates */
    p->f_u1 = oglxReadFloat();
    p->f_v1 = oglxReadFloat();
    p->f_u2 = oglxReadFloat();
    p->f_v2 = oglxReadFloat();
    /* End struct glf_type_u_v_couple */
    if (!oglxReadEndStruct(KW_TYP_U_V_COUPLE))
        return SGL_FALSE;
    return SGL_TRUE;
}

static SGLbool oglxReadCharacterBitmap(glf_type_character_bitmap9 * p, SGLuint8 kind, SGLuint8 bmpsize)
{
    SGLuint32 i;
    /* Begin struct glf_type_character_bitmap2 */
    if (!oglxReadBeginStruct(kind))
        return SGL_FALSE;
    /* Character */
    oglxReadCharacter(&(p->tr_type_character));
    /* UV mapping array */
    if (!oglxReadBeginArray(KW_TYP_U_V_COUPLE, bmpsize))
        return SGL_FALSE;
    for (i = 0U; i < bmpsize; i++) {
        if (!oglxReadUVCouple(&(p->tr_u_v_mapping[i])))
            return SGL_FALSE;
    }
    if (!oglxReadEndArray(KW_TYP_U_V_COUPLE))
        return SGL_FALSE;
    /* End struct glf_type_character_bitmap2 */
    if (!oglxReadEndStruct(kind))
        return SGL_FALSE;
    return SGL_TRUE;
}

static char *oglxReadStrokeDescription(char *p)
{
    SGLuint32 nb_lines, i;
    SGLushort nb_vertex, j;
    SGLushort line_type;
    SGLfloat point;
    SGLuint32 nb_points;

    /* Read number of lines */
    nb_lines = oglxReadUint32();
    *(SGLuint32 *) p = nb_lines;
    p += sizeof(SGLuint32);

    /* Read each line */
    for (i = 0; i < nb_lines; i++) {
        /* Read line type */
        line_type = oglxReadUint16();
        *(SGLuint16 *) p = line_type;
        p += sizeof(SGLuint16);

        /* Read number of vertex */
        nb_vertex = oglxReadUint16();
        *(SGLuint16 *) p = nb_vertex;
        p += sizeof(SGLuint16);

        /* Read vertex array */
        nb_points = nb_vertex * 2;

        if (!oglxReadBeginArray(KW_TYP_FLOAT, nb_points))
            return p;
        for (j = 0U; j < nb_points; j++) {
            point = oglxReadFloat();
            *(SGLfloat *) p = point;
            p += sizeof(SGLfloat);
        }
        if (!oglxReadEndArray(KW_TYP_FLOAT))
            return p;
    }

    return p;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxReadBinaryFonts
  DESCRIPTION:
    Function provides an example of binary fonts reader.
  PARAMETERS:
    f: file containing binary fonts block
  RETURN:
    0 in case of success, otherwise an error code
---------------------------------------------------------------------+*/
int oglxReadBinaryFonts(oglxReadByteFunc pf_read_byte, oglxMallocFunc pf_malloc, oglxFreeFunc pf_free)
{
    SGLuint32 ui_fonts_size, ui_version, ui_fonts_number, ui_textures_number;
    SGLuint32 i, j, nb;
    char *fonts_data, *pointer;
    glf_type_font *font_descriptions;
    glf_type_map_property *map_properties;
    glf_type_font_table_header *tr_header;

    /* Record global function pointer */
    global_read_byte = pf_read_byte;

    /* Begin struct glf_type_font_table */
    if (!oglxReadBeginStruct(KW_TYP_FONT_TABLE))
        return KW_TYP_FONT_TABLE;

    /* Begin struct glf_type_font_table_header */
    if (!oglxReadBeginStruct(KW_TYP_FONT_TABLE_HEADER))
        return KW_TYP_FONT_TABLE_HEADER;
    /* Read data */
    ui_fonts_size = oglxReadUint32();
    ui_version = oglxReadUint32();
    ui_fonts_number = oglxReadUint32();
    ui_textures_number = oglxReadUint32();
    /* End struct glf_type_font_table_header */
    if (!oglxReadEndStruct(KW_TYP_FONT_TABLE_HEADER))
        return KW_TYP_FONT_TABLE_HEADER;

    /* Allocate memory for structure */
    fonts_data = pf_malloc(ui_fonts_size);
    if (fonts_data == NULL)
        return 10;
    pointer = fonts_data;

    /* Fill glf_type_font_table_header */
    tr_header = (glf_type_font_table_header *) pointer;
    tr_header->ui_fonts_size = ui_fonts_size;   /* Can be larger than actual, depend of architecture */
    tr_header->ui_version = ui_version;
    tr_header->ui_fonts_number = ui_fonts_number;
    tr_header->ui_textures_number = ui_textures_number;
    pointer += sizeof(glf_type_font_table_header);

    /* Read fonts descriptions */
    font_descriptions = (glf_type_font *) pointer;
    if (!oglxReadBeginArray(KW_TYP_FONT, ui_fonts_number))
        return KW_TYP_FONT;
    for (i = 0U; i < ui_fonts_number; i++) {
        if (!oglxReadFontDescription((glf_type_font *) pointer))
            return KW_TYP_FONT;
        pointer += sizeof(glf_type_font);
    }
    if (!oglxReadEndArray(KW_TYP_FONT))
        return KW_TYP_FONT;

    /* Read maps descriptions */
    map_properties = (glf_type_map_property *) pointer;
    if (!oglxReadBeginArray(KW_TYP_MAP_PROPERTY, ui_textures_number))
        return KW_TYP_MAP_PROPERTY;
    for (i = 0U; i < ui_textures_number; i++) {
        if (!oglxReadMapProperty((glf_type_map_property *) pointer))
            return KW_TYP_MAP_PROPERTY;
        pointer += sizeof(glf_type_map_property);
    }
    if (!oglxReadEndArray(KW_TYP_MAP_PROPERTY))
        return KW_TYP_MAP_PROPERTY;

    /* Read characters descriptions */
    for (i = 0U; i < ui_fonts_number; i++) {
        if ((font_descriptions[i].us_font_kind == GLF_FONT_TYPE_STROKE) || (font_descriptions[i].us_font_kind == GLF_FONT_TYPE_BITMAP)) {

            SGLuint32 char_number = font_descriptions[i].us_char_number + 1U;
            SGLuint8 kind = 0U;
            if (!oglxReadBeginAnyArray(&kind, char_number))
                return KW_TYP_CHARACTER;
            /* Fix-up offsets depending of architecture  */
            font_descriptions[i].ui_offset_characters = (SGLulong) (pointer - (char *) (&font_descriptions[i]));
            /* Read characters depending of type */
            switch (kind) {
            case KW_TYP_CHARACTER_STROKE:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_stroke);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterStroke((glf_type_character_stroke *) pointer))
                        return KW_TYP_CHARACTER_STROKE;
                    pointer += sizeof(glf_type_character_stroke);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP1:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap1);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 1U))
                        return KW_TYP_CHARACTER_BITMAP1;
                    pointer += sizeof(glf_type_character_bitmap1);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP2:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap2);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 2U))
                        return KW_TYP_CHARACTER_BITMAP2;
                    pointer += sizeof(glf_type_character_bitmap2);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP3:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap3);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 3U))
                        return KW_TYP_CHARACTER_BITMAP3;
                    pointer += sizeof(glf_type_character_bitmap3);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP4:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap4);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 4U))
                        return KW_TYP_CHARACTER_BITMAP4;
                    pointer += sizeof(glf_type_character_bitmap4);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP5:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap5);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 5U))
                        return KW_TYP_CHARACTER_BITMAP5;
                    pointer += sizeof(glf_type_character_bitmap5);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP6:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap6);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 6U))
                        return KW_TYP_CHARACTER_BITMAP6;
                    pointer += sizeof(glf_type_character_bitmap6);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP7:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap7);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 7U))
                        return KW_TYP_CHARACTER_BITMAP7;
                    pointer += sizeof(glf_type_character_bitmap7);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP8:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap8);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 8U))
                        return KW_TYP_CHARACTER_BITMAP8;
                    pointer += sizeof(glf_type_character_bitmap8);
                }
                break;
            case KW_TYP_CHARACTER_BITMAP9:
                font_descriptions[i].us_char_size = (SGLushort) sizeof(glf_type_character_bitmap9);
                for (j = 0U; j < char_number; j++) {
                    if (!oglxReadCharacterBitmap((glf_type_character_bitmap9 *) pointer, kind, 9U))
                        return KW_TYP_CHARACTER_BITMAP9;
                    pointer += sizeof(glf_type_character_bitmap9);
                }
                break;
            default:
                return KW_TYP_CHARACTER;
            }
            if (!oglxReadEndArray(kind))
                return kind;
        }
    }

    /* Fix-up offsets depending of architecture  */
    for (i = 0U; i < ui_fonts_number; i++) {
        if (font_descriptions[i].us_font_kind == GLF_FONT_TYPE_STROKE) {
            font_descriptions[i].ui_offset_strokes = (SGLulong) (pointer - (char *) (&font_descriptions[i]));
        }
    }

    /* Read stroke descriptions */
    for (i = 0U; i < ui_fonts_number; i++) {
        if (font_descriptions[i].us_font_kind == GLF_FONT_TYPE_STROKE) {
            SGLuint32 char_number = font_descriptions[i].us_char_number + 1U;

            /* Begin struct */
            if (!oglxReadBeginStruct(0))
                return KW_TYP_CHARACTER_STROKE;

            /* Read stroke description for each char */
            for (j = 0U; j < char_number; j++) {
                pointer = oglxReadStrokeDescription(pointer);
            }

            /* End struct */
            if (!oglxReadEndStruct(0))
                return KW_TYP_CHARACTER_STROKE;
        }
    }

    /* End struct glf_type_font_table */
    if (!oglxReadEndStruct(KW_TYP_FONT_TABLE))
        return KW_TYP_FONT_TABLE;

    /* Adjust font size, dependent of architecture */
    tr_header->ui_fonts_size = (SGLulong) (pointer - fonts_data);

    /* Read textures */
    for (i = 0U; i < ui_textures_number; i++) {
        SGLuint32 map_size = map_properties[i].ui_map_size;
        /* Allocate memory for data */
        SGLuint8 *map = pf_malloc(map_size);
        if (map == NULL)
            return 10;
        map_properties[i].ptub_map_data = map;

        if (!oglxReadBeginPartialArray(KW_TYP_UINT8, map_size, &nb))
            return KW_TYP_MAP_PROPERTY;
        for (j = 0U; j < nb; j++) {
            *map++ = oglxReadUint8();
        }
        if (!oglxReadEndArray(KW_TYP_UINT8))
            return KW_TYP_MAP_PROPERTY;
    }

    /* Call OGLX sgluLoadFonts */
    sgluLoadFonts(fonts_data);

    /* Free texture maps, now useless */
    for (i = 0U; i < ui_textures_number; i++) {
        pf_free(map_properties[i].ptub_map_data);
        map_properties[i].ptub_map_data = NULL;
    }

    return 0;
}

/* End of File ***************************************************************/
