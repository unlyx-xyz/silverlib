/* 
Tint Library project - Made by: unlyx.xyz
04-26-2026
Version: 1.2.2
*/

#ifndef TINT_LIB_H
#define TINT_LIB_H

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
======================
==###DECLARATIONS###==
======================
*/

typedef enum {
    ESCAPE_CODE_OCTAL = 0,
    ESCAPE_CODE_HEXADECIMAL = 2,
} TL_EscapeCodesIndex;
extern const char* const TL_EscapeCodes[];

typedef enum {
    TL_CURSOR_CONTROL_HOME = 0,
    TL_CURSOR_CONTROL_LINE_COLUMN = 1,
    TL_CURSOR_CONTROL_UP = 2,
    TL_CURSOR_CONTROL_DOWN = 3,
    TL_CURSOR_CONTROL_RIGHT = 4,
    TL_CURSOR_CONTROL_LEFT = 5,
    TL_CURSOR_CONTROL_COLUMN = 6,
} TL_CursorControlsIndex;
extern const char* const TL_CursorControls[];

typedef enum {
    TL_ERASE_FUNCTION_DISPLAY = 0,
    TL_ERASE_FUNCTION_CURSOR_END = 1,
    TL_ERASE_FUNCTION_CURSOR_BEGIN = 2,
    TL_ERASE_FUNCTION_SCREEN = 3,
    TL_ERASE_FUNCTION_LINE_CURSOR_END = 4,
    TL_ERASE_FUNCTION_LINE_CURSOR_BEGIN = 5,
    TL_ERASE_FUNCTION_LINE = 6,
} TL_EraseFunctionsIndex;
extern const char* const TL_EraseFunctions[];

typedef enum {
    TL_GRAPHICS_MODE_RESET = 0,
    TL_GRAPHICS_MODE_BOLD = 1,
    TL_GRAPHICS_MODE_DIM = 2,
    TL_GRAPHICS_MODE_ITALIC = 3,
    TL_GRAPHICS_MODE_UNDERLINE = 4,
    TL_GRAPHICS_MODE_BLINKING = 5,
    TL_GRAPHICS_MODE_INVERSE = 6,
    TL_GRAPHICS_MODE_HIDDEN = 7,
    TL_GRAPHICS_MODE_STRIKETHROUGH = 8,
} TL_GraphicModesIndex;
typedef struct {
    bool bold;
    bool dim;
    bool italic;
    bool underline;
    bool blinking;
    bool inverse;
    bool hidden;
    bool strikethrough;
} TL_GraphicModeConfig;
extern const char* const TL_GraphicModes[];

typedef enum {
    TL_COLOR_CODE_FG_BLACK = 0,
    TL_COLOR_CODE_FG_RED = 1,
    TL_COLOR_CODE_FG_GREEN = 2,
    TL_COLOR_CODE_FG_YELLOW = 3,
    TL_COLOR_CODE_FG_BLUE = 4,
    TL_COLOR_CODE_FG_MAGENTA = 5,
    TL_COLOR_CODE_FG_CYAN = 6,
    TL_COLOR_CODE_FG_WHITE = 7,
    TL_COLOR_CODE_FG_DEFAULT = 8,
    TL_COLOR_CODE_RESET = 9,
    TL_COLOR_CODE_FG_BRIGHT_BLACK = 10,
    TL_COLOR_CODE_FG_BRIGHT_RED = 11,
    TL_COLOR_CODE_FG_BRIGHT_GREEN = 12,
    TL_COLOR_CODE_FG_BRIGHT_YELLOW = 13,
    TL_COLOR_CODE_FG_BRIGHT_BLUE = 14,
    TL_COLOR_CODE_FG_BRIGHT_MAGENTA = 15,
    TL_COLOR_CODE_FG_BRIGHT_CYAN = 16,
    TL_COLOR_CODE_FG_BRIGHT_WHITE = 17,
    TL_COLOR_CODE_BG_BLACK = 18,
    TL_COLOR_CODE_BG_RED = 19,
    TL_COLOR_CODE_BG_GREEN = 20,
    TL_COLOR_CODE_BG_YELLOW = 21,
    TL_COLOR_CODE_BG_BLUE = 22,
    TL_COLOR_CODE_BG_MAGENTA = 23,
    TL_COLOR_CODE_BG_CYAN = 24,
    TL_COLOR_CODE_BG_WHITE = 25,
    TL_COLOR_CODE_BG_DEFAULT = 26,
    TL_COLOR_CODE_BG_BRIGHT_BLACK = 27,
    TL_COLOR_CODE_BG_BRIGHT_RED = 28,
    TL_COLOR_CODE_BG_BRIGHT_GREEN = 29,
    TL_COLOR_CODE_BG_BRIGHT_YELLOW = 30,
    TL_COLOR_CODE_BG_BRIGHT_BLUE = 31,
    TL_COLOR_CODE_BG_BRIGHT_MAGENTA = 32,
    TL_COLOR_CODE_BG_BRIGHT_CYAN = 33,
    TL_COLOR_CODE_BG_BRIGHT_WHITE = 34,
} TL_IndexedColorsIndex;
typedef struct {
    TL_IndexedColorsIndex foreground;
    TL_IndexedColorsIndex background;
} TL_IndexedColorConfig;
extern const char* const TL_IndexedColors[];

typedef struct {
    int foreground;
    int background;
} TL_8BitConfig;

typedef struct {
    bool enable;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} TL_RGB;

typedef struct {
    TL_RGB foreground;
    TL_RGB background;
} TL_TruecolorConfig;

typedef enum {
    SEQUENCE_INDEXED = 0,
    SEQUENCE_8BIT = 1,
    SEQUENCE_TRUECOLOR = 2,
    SEQUENCE_JUST_GRAPHICS = 3,
} _tl_sequence_type;

typedef struct {
    TL_GraphicModeConfig graphics;
    TL_IndexedColorConfig indexed;
    TL_8BitConfig eight_bit;
    TL_TruecolorConfig truecolor;
} TL_Sequence;

TL_Sequence TL_InitSequence(void);

/*
========================
==###IMPLEMENTATION###==
========================
*/

#ifdef TINT_LIB_IMPLEMENTATION

#ifndef TINT_LIB_ESCAPE_CODE
#define TINT_LIB_ESCAPE_CODE ESCAPE_CODE_OCTAL
#endif

const char* const TL_EscapeCodes[] = {"\033", "\x1B"};
const char* const TL_CursorControls[] = {"H", "A", "B", "C", "D", "G"};
const char* const TL_EraseFunctions[] = {"J", "0J", "1J", "2J", "0K", "1K", "2K"};
const char* const TL_GraphicModes[] = {"0", "1", "2", "3", "4", "5", "7", "8", "9"};
const char* const TL_IndexedColors[] = {"30", "31", "32", "33", "34", "35", "36", "37", "39", "0", "90", "91", "92", "93", "94", "95", "96", "97" "40", "41", "42", "43", "44", "45", "46", "47", "49", "100", "101", "102", "103", "104", "105", "106", "107"};

TL_Sequence TL_InitSequence(void) {

    TL_Sequence seq;

    memset(&seq.graphics.bold, 0, sizeof(seq.graphics.bold));
    memset(&seq.graphics.dim, 0, sizeof(seq.graphics.dim));
    memset(&seq.graphics.italic, 0, sizeof(seq.graphics.italic));
    memset(&seq.graphics.underline, 0, sizeof(seq.graphics.underline));
    memset(&seq.graphics.blinking, 0, sizeof(seq.graphics.blinking));
    memset(&seq.graphics.inverse, 0, sizeof(seq.graphics.inverse));
    memset(&seq.graphics.hidden, 0, sizeof(seq.graphics.hidden));
    memset(&seq.graphics.strikethrough, 0, sizeof(seq.graphics.strikethrough));
    memset(&seq.graphics, 0, sizeof(seq.graphics));

    memset(&seq.indexed.foreground, 0, sizeof(seq.indexed.foreground));
    memset(&seq.indexed.background, 0, sizeof(seq.indexed.background));
    memset(&seq.indexed, 0, sizeof(seq.indexed));

    memset(&seq.eight_bit.foreground, 0, sizeof(seq.eight_bit.foreground));
    memset(&seq.eight_bit.background, 0, sizeof(seq.eight_bit.background));
    memset(&seq.eight_bit, 0, sizeof(seq.eight_bit));

    memset(&seq.truecolor.foreground.r, 0, sizeof(seq.truecolor.foreground.r));
    memset(&seq.truecolor.foreground.g, 0, sizeof(seq.truecolor.foreground.g));
    memset(&seq.truecolor.foreground.b, 0, sizeof(seq.truecolor.foreground.b));
    memset(&seq.truecolor.foreground.enable, 0, sizeof(seq.truecolor.foreground.enable));
    memset(&seq.truecolor.foreground, 0, sizeof(seq.truecolor.foreground));
    memset(&seq.truecolor.background.r, 0, sizeof(seq.truecolor.background.r));
    memset(&seq.truecolor.background.g, 0, sizeof(seq.truecolor.background.g));
    memset(&seq.truecolor.background.b, 0, sizeof(seq.truecolor.background.b));
    memset(&seq.truecolor.background.enable, 0, sizeof(seq.truecolor.background.enable));
    memset(&seq.truecolor.background, 0, sizeof(seq.truecolor.background));
    memset(&seq.truecolor, 0, sizeof(seq.truecolor));

    memset(&seq, 0, sizeof(seq));

    return seq;

};

#define _apply_property(msg, property, presentation)\
        do {\
            if (property) {\
                snprintf(msg + strlen(msg), sizeof(msg) - strlen(msg), "%s;", presentation);\
            }\
        } while(0)\

static inline const char* _tl_build_sequence(FILE *stream, _tl_sequence_type type, TL_Sequence sequence, const char* fmt, ...) {

    size_t msg_seq_len = 256;
    char msg_seq[msg_seq_len];
    memset(msg_seq, 0, msg_seq_len);

    strcpy(msg_seq, TL_EscapeCodes[TINT_LIB_ESCAPE_CODE]);
    strcat(msg_seq, "[");

    _apply_property(msg_seq, sequence.graphics.bold, TL_GraphicModes[TL_GRAPHICS_MODE_BOLD]);
    _apply_property(msg_seq, sequence.graphics.dim, TL_GraphicModes[TL_GRAPHICS_MODE_DIM]);
    _apply_property(msg_seq, sequence.graphics.italic, TL_GraphicModes[TL_GRAPHICS_MODE_ITALIC]);
    _apply_property(msg_seq, sequence.graphics.underline, TL_GraphicModes[TL_GRAPHICS_MODE_UNDERLINE]);
    _apply_property(msg_seq, sequence.graphics.blinking, TL_GraphicModes[TL_GRAPHICS_MODE_BLINKING]);
    _apply_property(msg_seq, sequence.graphics.inverse, TL_GraphicModes[TL_GRAPHICS_MODE_INVERSE]);
    _apply_property(msg_seq, sequence.graphics.hidden, TL_GraphicModes[TL_GRAPHICS_MODE_HIDDEN]);
    _apply_property(msg_seq, sequence.graphics.strikethrough, TL_GraphicModes[TL_GRAPHICS_MODE_STRIKETHROUGH]);

    switch(type) {
        case SEQUENCE_INDEXED:
            snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "%s;%s;", TL_IndexedColors[sequence.indexed.foreground], TL_IndexedColors[sequence.indexed.background]);
            break;
        case SEQUENCE_8BIT:
            if(sequence.eight_bit.foreground >= 0) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "38;5;%d;", sequence.eight_bit.foreground);
            }
            if(sequence.eight_bit.background >= 0) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "48;5;%d;", sequence.eight_bit.background);
            }
            break;
        case SEQUENCE_TRUECOLOR:
            if(sequence.truecolor.foreground.enable) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "38;2;%d;%d;%d;", sequence.truecolor.foreground.r, sequence.truecolor.foreground.g, sequence.truecolor.foreground.b); 
            }
            if(sequence.truecolor.background.enable) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "48;2;%d;%d;%d;", sequence.truecolor.background.r, sequence.truecolor.background.g, sequence.truecolor.background.b);
            }
            break;
        case SEQUENCE_JUST_GRAPHICS:
            break;
    }

    strcat(msg_seq, "m");

    char msg[2048] = {0};
    snprintf(msg, sizeof(msg), "%s%s%s[0m", msg_seq, fmt, TL_EscapeCodes[TINT_LIB_ESCAPE_CODE]);

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, msg, args);
    va_end(args);

};

#define TL_IndexedPrintf(sequence, ...) _tl_build_sequence(stdout, SEQUENCE_INDEXED, sequence, ##__VA_ARGS__)
#define TL_8BitPrintf(sequence, ...) _tl_build_sequence(stdout, SEQUENCE_8BIT, sequence, ##__VA_ARGS__)
#define TL_TruecolorPrintf(sequence, ...) _tl_build_sequence(stdout, SEQUENCE_TRUECOLOR, sequence, ##__VA_ARGS__)
#define TL_GraphicsPrintf(sequence, ...) _tl_build_sequence(stdout, SEQUENCE_JUST_GRAPHICS, sequence, ##__VA_ARGS__)

#define TL_IndexedfPrintf(stream, sequence, ...) _tl_build_sequence(stream, SEQUENCE_INDEXED, sequence, ##__VA_ARGS__)
#define TL_8BitfPrintf(stream, sequence, ...) _tl_build_sequence(stream, SEQUENCE_8BIT, sequence, ##__VA_ARGS__)
#define TL_TruecolorfPrintf(stream, sequence, ...) _tl_build_sequence(stream, SEQUENCE_TRUECOLOR, sequence, ##__VA_ARGS__)
#define TL_GraphicsfPrintf(stream, sequence, ...) _tl_build_sequence(stream, SEQUENCE_TRUECOLOR, sequence, ##__VA_ARGS__)

void _tl_copy_sequence(int type, TL_Sequence src, TL_Sequence *dest) {
    
    switch (type) {
        case -1:
            dest->graphics = src.graphics;
            dest->indexed = src.indexed;
            dest->eight_bit = src.eight_bit;
            dest->truecolor = src.truecolor;
            break;
        case SEQUENCE_INDEXED:
            dest->indexed = src.indexed;
            break;
        case SEQUENCE_8BIT:
            dest->eight_bit = src.eight_bit;
            break;
        case SEQUENCE_TRUECOLOR:
            dest->truecolor = src.truecolor;
            break;
        case SEQUENCE_JUST_GRAPHICS:
            dest->graphics = src.graphics;
            break;
    } 

};

#define TL_CopySequence(src, dest) _tl_copy_sequence(-1, src, dest)
#define TL_CopyIndexed(src, dest) _tl_copy_sequence(SEQUENCE_INDEXED, src, dest)
#define TL_Copy8bit(src, dest) _tl_copy_sequence(SEQUENCE_8BIT, src, dest)
#define TL_CopyTruecolor(src, dest) _tl_copy_sequence(SEQUENCE_TRUECOLOR, src, dest)
#define TL_CopyGraphics(src, dest) _tl_copy_sequence(SEQUENCE_JUST_GRAPHICS, src, dest)

#endif

#endif
