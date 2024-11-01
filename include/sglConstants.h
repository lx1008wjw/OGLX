/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglConstants.h
 DESCRIPTION       : Definition of SGL constants
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- +*/

#ifndef _SGLCONSTANTS_H
#define _SGLCONSTANTS_H

/* NULL constant */
#define SGL_NULL ((void *)0)

/*+ DEFINITION OF CONSTANTS -------------------------------------------------------
 DESCRIPTION       : Definition of OGLX Constants
 LLR: [OGLX-DDD-Constants]
---------------------------------------------------------------------------- +*/


/*****************************************************************************
**  Min floating point value used for internal purposes
 *****************************************************************************/
#define SGL_MIN_F                           0.000001F
#define SGL_MIN_D                           0.000001


/*****************************************************************************
** Define the Maximum number of line widths and line stipple
*****************************************************************************/

#define SGL_NUMBER_OF_LINE_WIDTH                256UL
#define SGL_NUMBER_OF_LINE_STIPPLES             256UL


/*****************************************************************************
** Define the Maximum number of clipplanes
*****************************************************************************/

#define SGL_MAX_CLIP_PLANES                     6U

/*****************************************************************************
** Texture index when no texture is bound
*****************************************************************************/
#define NO_OGL_TEXTURE_BOUND				65535U

/*****************************************************************************
** Size max of textures
*****************************************************************************/

#define SGL_TEXTURE_MIN_WIDTH                    64UL
#define SGL_TEXTURE_MIN_HEIGHT                   64UL
#define SGL_TEXTURE_MAX_WIDTH                  1024UL
#define SGL_TEXTURE_MAX_HEIGHT                 1024UL

/*****************************************************************************
** Textures application mode
		- SGL_CLAMP for 1*1 texture application (rectangle bitmap)
		- SGL_REPEAT for texture with repetition
*****************************************************************************/
#define SGL_CLAMP	0U
#define SGL_REPEAT	1U

/*****************************************************************************
** Max number of vertices for a primitive
*****************************************************************************/

#define SGL_MAX_VERTEX_ARRAY_SIZE			    4096UL

/*****************************************************************************
** Maximum alpha value
*****************************************************************************/

#define SGL_MAX_ALPHA_8                        255U
#define SGL_ONE_DIV_MAX_ALPHA_8                0.003921569F

/*****************************************************************************
** Maximum string length
*****************************************************************************/

#define SGLU_MAX_STRING_LENGTH                  1024U

#define SGLU_MAX_TEXT_AREA_STRING_LENGTH                  4096U

/*****************************************************************************
 ** Max number of error codes
 *****************************************************************************/

#define SGL_ERROR_MAX						32UL

/*****************************************************************************
** Linewidth range
*****************************************************************************/

#define SGL_MIN_LINE_WIDTH					1U
#define SGL_MAX_LINE_WIDTH					8U

/*****************************************************************************
** Error stack management
*****************************************************************************/

#define SGL_NO_ERROR						0U
#define SGL_ERROR_STACK_UNDERFLOW			1U
#define SGL_ERROR_STACK_OVERFLOW			2U

/*****************************************************************************
** Matrix stack depth
*****************************************************************************/
#if !defined(SC101_DEV_ENV)
#define SGL_MAX_STACK									32U
#else
#define SGL_MAX_STACK									16U
#endif

/*****************************************************************************
** Number of scissors
*****************************************************************************/
#define SGL_MAX_SCISSORS								16U


/*****************************************************************************
** Maximum number of lines in a multiline text
*****************************************************************************/
#define SGL_MAX_STRING_LINES							256UL


/*****************************************************************************
** Maximum value used for some float computations
*****************************************************************************/
#define SGL_VALUE_MAX									1.0E5F

/*****************************************************************************
** Maximum number of text separators
*****************************************************************************/
#define SGL_MAX_SEPARATORS								8U

/*****************************************************************************
** States of sglBegin Modes
** remark : 0 value is not allowed
*****************************************************************************/

#define SGL_LINES                           9U
#define SGL_LINE_STRIP                      11U
#define SGL_LINE_LOOP                       3U
#define SGL_POLYGON                         5U
#define SGL_TRIANGLE_STRIP                  7U

#define SGL_MASK1							1U
#define SGL_MASK2							2U
#define SGL_MASK4							4U
#define SGL_MASK8							8U
#define SGL_MASK16							16U
#define SGL_MASK32							32U
#define SGL_MASK64							64U
#define SGL_MASK128							128U


/*****************************************************************************
** State of the stencil box
*****************************************************************************/

#define SGL_VISIBLE_INSIDE                 0U
#define SGL_VISIBLE_OUTSIDE                1U

/*****************************************************************************
** States of SGL_Bitmap fot the each type of bitmap
*****************************************************************************/

#define SGL_BITMAP_RGB_NOT_TRANSPARENT      5U
#define SGL_BITMAP_RGB_TRANSPARENT          6U
#define SGL_BITMAP_ALPHA					7U

/*****************************************************************************
** States of Enable and Disable and Clipping plane name type
*****************************************************************************/

/*****************************************************************************
** States of Enable and Disable and Clipping plane name type
*****************************************************************************/

#define SGL_NO_MASK                 0U
#define SGL_CLIP_PLANE0				256U
#define SGL_CLIP_PLANE1				512U
#define SGL_CLIP_PLANE2				1024U
#define SGL_CLIP_PLANE3				2048U
#define SGL_CLIP_PLANE4				4096U
#define SGL_CLIP_PLANE5				8192U
#define SGL_NO_CLIP_PLANE			0xFFU

#define SGL_UNUSED_MASK_BITS	0xFFFFC000UL

/*****************************************************************************
** Define the Maximum number of masks
*****************************************************************************/
#define SGL_MAX_MASKS_NUMBER                 8U


#ifndef OGLX_NO_GLCLIPPLANE
#define SGL_TYPE_CLIP_PLANE			0U
#endif
#define SGL_TYPE_CLIP_BOX			1U
#define SGL_TYPE_STENCIL			2U

#define SGL_POLYGON_SMOOTH          33U
#define SGL_LINE_HALOING            35U

/*****************************************************************************
** State texture mapping
*****************************************************************************/
#define SGL_TEXTURE_2D              56U

/*****************************************************************************
** State modulate for textures
*****************************************************************************/
#define SGL_MODULATE				57U

/*****************************************************************************
** State tessellation for filled polygon
*****************************************************************************/
#define SGL_TESSELLATION			58U

/*****************************************************************************
** State gradient mapping
*****************************************************************************/
#define SGL_GRADIENT				59U

/*****************************************************************************
** State Depth
*****************************************************************************/
#define SGL_HALO_SAME_LEVEL			60U
#define SGL_HALO_TRANSLATE			0.0001F

/*****************************************************************************
** Text adjustment property
*****************************************************************************/
#define SGL_TEXT_POS_ADJUSTMENT		61U


/*****************************************************************************
** Maximum Depth
*****************************************************************************/
#define SGL_MAX_DEPTH           255U

/*****************************************************************************
** States of sgluArcSector2f 
*****************************************************************************/

#define SGL_ARC_FILLED          1U
#define SGL_ARC_OUTLINE         2U
#define SGL_ARC_CIRCLE_LINE     3U

/*****************************************************************************
** States of sgluCrown2f
*****************************************************************************/

#define SGL_CROWN_FILLED    0U
#define SGL_CROWN_OUTLINE   1U

/*****************************************************************************
** States of sgluArcEllipse2f
*****************************************************************************/

#define SGL_ELLIPSE_FILLED    0U
#define SGL_ELLIPSE_OUTLINE   1U

/* attribute values */

/***********************/
/* Alignment constants */
/***********************/

/* Horizontal */
#define SGL_ALIGN_LEFT								0L
#define SGL_ALIGN_CENTER							1L
#define SGL_ALIGN_RIGHT								2L

/* Vertical */
#define SGL_ALIGN_BOTTOM							0L
#define SGL_ALIGN_MIDDLE							1L
#define SGL_ALIGN_TOP								2L

/*****************************************************************************
** Constants to select the line drawing mode
*****************************************************************************/

#define SGL_RAW_OPENGL_LINES							0U
#define SGL_SMOOTH_LINES								1U


/*****************************************************************************
** Constants defining escape sequences in complex text
*****************************************************************************/
#define SGL_K_NULL			0x0U
#define SGL_K_LF			0x0AU
#define SGL_K_CR			0x0DU
#define SGL_K_ESC			0x1BU

#define SGL_K_OUTLINE		0x40U
#define SGL_K_FORECOLOR		0x41U
#define SGL_K_BACKCOLOR		0x42U
#define SGL_K_FONT			0x43U

#define SGL_K_VIDEOINV_BEG	0x44U
#define SGL_K_VIDEOINV_END	0x45U
#define SGL_K_UNDERLINE_BEG	0x48U
#define SGL_K_UNDERLINE_END	0x49U
#define SGL_K_BOLD_BEG		0x4AU
#define SGL_K_BOLD_END		0x4BU
#define SGL_K_CROSSED_BEG	0x4CU
#define SGL_K_CROSSED_END	0x4DU
#define SGL_K_FRAMED_BEG	0x4EU
#define SGL_K_FRAMED_END	0x4FU
#define SGL_K_REPEAT_BEG	0x50U
#define SGL_K_REPEAT_END	0x51U

#define OGLX_TOP		0x01U
#define OGLX_BOTTOM		0x02U
#define OGLX_LEFT		0x04U
#define OGLX_RIGHT		0x08U
#define OGLX_UNDERLINE	0x10U
#define OGLX_CROSSED	0x20U



/*****************************************************************************
** Constants to display the sign of a float
*****************************************************************************/
#define SGL_SIGN_ALWAYS										0L
#define SGL_SIGN_NEVER										1L
#define SGL_SIGN_WHEN_NEGATIVE								2L

/****************************************************************************
** Line caps appearance
*****************************************************************************/
#define SGL_STRAIGHT_CAPS									0U
#define SGL_ROUNDED_CAPS									1U

/****************************************************************************
** Font metrics constants
*****************************************************************************/
#define SGL_CHAR_WIDTH										0U
#define SGL_CHAR_HEIGHT										1U
#define SGL_VERTICAL_ADVANCE								2U
#define SGL_HORIZONTAL_ADVANCE								3U
#define SGL_UNDERLINE										4U
#define SGL_STROKE											5U
#define SGL_MAX_CHAR_WIDTH									6U
#define SGL_MAX_CHAR_HEIGHT									7U
#define SGL_MAX_CHAR_H_ADVANCE								8U
#define SGL_MAX_CHAR_V_ADVANCE								9U

/****************************************************************************
** Charset Encoding Formats
*****************************************************************************/
#define SGL_FORMAT_ASCII									0U
#define SGL_FORMAT_UTF8										1U
#define SGL_FORMAT_GBK										2U


/****************************************************************************
** Constants for OpenGL settings reset through sglResetOpenGLSettings
*****************************************************************************/
#define SGL_DISABLE_BLENDING								0x1UL
#define SGL_DISABLE_TEXTURE_2D								0x2UL
#define SGL_DISABLE_SCISSOR_TEST							0x4UL
#define SGL_DISABLE_DEPTH_TEST								0x8UL
#define SGL_DISABLE_CULL_FACE								0x10UL
#define SGL_RESET_BLEND_FUNC								0x20UL
#define SGL_DISABLE_LIGHTING								0x40UL
#define SGL_ENABLE_LINE_SMOOTH								0x80UL
#define SGL_ENABLE_VERTEX_ARRAY								0x100UL
#define SGL_ENABLE_TEXTURE_ARRAY							0x200UL
#define SGL_RESET_TEXTURE_ENV								0x400UL
#define SGL_RESET_ALL_SETTINGS								0x7FFUL

/*****************************************************************************
** Path constants
*****************************************************************************/
#define SGL_OUTLINE											1U
#define SGL_FILL											2U

/* Size constants for structures in sgl_context_types.h */
#define OGLX_MAX_LINES 16

#define SGL_MAX_STATIC_TEXTURE 1024UL

/*****************************************************************************
** Errors list
*****************************************************************************/


/* SGL ERRORS */

#define SGL_ERROR_SGL_BEGIN								1U

#define SGL_ERROR_SGL_BINDTEXTURE						2U

#define SGL_ERROR_SGL_CLIPBOX							4U

#define SGL_ERROR_SGL_CLIPPLANE							5U

#define SGL_ERROR_SGL_ENABLE							6U

#define SGL_ERROR_SGL_DISABLE							7U

#define SGL_ERROR_SGLU_LOADFONTS						9U

#define SGL_ERROR_SGL_ORTHO								13U

#define SGL_ERROR_SGL_PUSHMATRIX						15U

#define SGL_ERROR_SGL_POPMATRIX							16U

#define SGL_ERROR_SGL_VERTEX2F							20U

#define SGL_ERROR_SGLU_ARCSECTOR2F						26U

#define SGL_ERROR_SGLU_CROWN2F							27U

#define SGL_ERROR_SGL_INDEXFONTI                     29U

#define SGL_ERROR_SGLU_WRITETEXT                        31U

#define SGL_ERROR_SGL_VIEWPORT							32U

#define SGL_ERROR_SGL_INDEXCOLORI					    49U

#define SGL_ERROR_SGL_INDEXLINESTIPPLE				    50U

#define SGL_ERROR_SGL_INDEXLINEWIDTHI				    51U

#define SGL_ERROR_SGL_TEXIMAGE2DUBV						52U

#define SGL_ERROR_SGL_COMPRESSEDTEXIMAGE2D				53U

#define SGL_ERROR_SGL_TRANSPARENCY8						55U

#define SGL_ERROR_SGL_COLORPOINTERF						57U

#define SGL_ERROR_SGL_LINESTIPPLEPOINTER				61U

#define SGL_ERROR_SGL_SETTEXTALIGNMENT					65U

#define SGL_ERROR_SGL_SETACTIVEMASKS					66U

#define SGL_ERROR_SGL_BEGINSCISSOR						67U

#define SGL_ERROR_SGL_ENDSCISSOR						68U

#define SGL_ERROR_SGL_SETWORDSSEPARATORS				69U

#define SGL_ERROR_SGL_SETLINESSEPARATORS				70U

#define SGL_ERROR_SGLU_WRITETEXTAREA					71U

#define SGL_ERROR_SGLU_GETTABLEINDEX					72U

#define SGL_ERROR_SGLU_LONGDIV							73U

#define SGL_ERROR_SGLU_FLOATDIV							74U

#define SGL_ERROR_SGLU_WRITEFLOAT						75U

#define SGL_ERROR_SGLU_DRAWBITMAP						76U

#define SGL_ERROR_SGLU_WRITERICHTEXT					77U

#define SGL_ERROR_SGL_FORMATTEXTURE						78U

#define SGL_ERROR_SGL_ALLOCATEMASKID					79U

#define SGL_ERROR_SGL_TEXVERTEX4F						80U

#define SGL_ERROR_SGL_RESET								83U

#define SGL_ERROR_SGL_SETCURRENTCONTEXT					84U

#define SGL_ERROR_SGLU_ARCSEGMENT						85U

#define SGL_ERROR_SGL_FREEMASKID						86U

#define SGL_ERROR_SGLU_WRITEUNICODE_TEXTAREA			87U

#define SGL_ERROR_SGL_GETFONTVALUE						88U

#define SGL_ERROR_SGLU_MODULOANGLE						89U

#define SGL_ERROR_SGL_GETARCSEGMENTBOUNDS				90U

#define SGL_ERROR_SGLU_ARCELLIPSE2F						91U

#define SGL_ERROR_SGL_SETCHARSETENCODING				92U

#define SGL_ERROR_SGL_PUSHMATRIXANDTRANSLATE			93U

#define SGL_ERROR_SGL_POPMATRIXANDTRANSLATE				94U

#define SGL_ERROR_SGL_INDEXFONTLINEWIDTHI				95U

#define SGL_ERROR_SGL_LINEWIDTHPOINTERF					96U

#define SGL_ERROR_SGLU_WRITEUNICODETEXT					97U

#define SGL_ERROR_SGL_CREATESTATICBITMAP				98U

#define SGL_ERROR_SGL_DRAWSTATICBITMAP					99U

#define SGL_ERROR_SGLU_WRITEWIDETEXT					100U

#define SGL_ERROR_SGLU_WRITEWIDETEXTAREA				101U

#define SGL_ERROR_SGL_INDEXHALOCOLORI					102U

#define SGL_ERROR_SGL_CONVERTTOINT32					104U

#define SGL_ERROR_SGL_CONVERTTOUINT32					105U

#define SGL_ERROR_SGL_CONVERTTOFLOAT					106U

#define SGL_ERROR_SGL_CONVERTTOINT8						107U

#define SGL_ERROR_SGL_CONVERTTOUINT8					108U

#define SGL_ERROR_SGL_CONVERTTOINT16					109U

#define SGL_ERROR_SGL_CONVERTTOUINT16					110U

#define SGL_ERROR_SGL_CONVERTTOUINT64					111U

#define SGL_ERROR_SGL_CONVERTTOINT64					112U

#define SGL_ERROR_SGL_BEGINPATH							113U

#define SGL_ERROR_SGL_GRADIENTTEXTURE					114U

#define SGL_ERROR_SGL_INDEXGRADIENTI					115U

#define SGL_ERROR_SGL_BEGINMASKDEFINITION				116U

#define SGL_ERROR_SGL_FORMATGRADIENT					117U

#define SGL_ERROR_SGL_CALLSTATICSEQUENCE				118U

#define SGL_ERROR_SGLU_DOUBLEDIV						119U

#define SGL_ERROR_SGL_CONVERTPOINTTOPIXEL				120U

#define SGL_ERROR_SGL_CONVERTPOINTTOSCREEN				121U

#define SGL_ERROR_SGL_CONVERTPIXELTOPOINT				122U

#define SGL_ERROR_SGL_CONVERTSCREENTOPOINT				123U

#define SGL_ERROR_SGL_CUBICBEZIERBOUNDS					124U

#define SGL_ERROR_SGL_SMOOTHCUBICBEZIERBOUNDS			125U

#define SGL_ERROR_SGL_ELLIPTICALARCBOUNDS				126U

#define SGL_ERROR_SGL_QUADBEZIERBOUNDS					127U

#define SGL_ERROR_SGL_SMOOTHQUADBEZIERBOUNDS			128U

/*+ END OF DEFINITION OF CONSTANTS -------------------------------------------------------*/

#endif                          /* _SGLCONSTANTS_H */
