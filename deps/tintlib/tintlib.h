/* 
Tint Library project - Made by: unlyx.xyz
04-26-2026
Version: 1.3.1
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
    ANSI_C0_ESC_OCTAL = 0,
    ANSI_C0_ESC_HEXADECIMAL = 2,
} TL_ANSIC0ESCIndex;
extern const char* const TL_ANSIC0ESC[];

typedef enum {
    ANSI_C1_CSI_SGR_RESET = 0,
    ANSI_C1_CSI_SGR_BOLD = 1,
    ANSI_C1_CSI_SGR_DIM = 2,
    ANSI_C1_CSI_SGR_ITALIC = 3,
    ANSI_C1_CSI_SGR_UNDERLINE = 4,
    ANSI_C1_CSI_SGR_BLINKING = 5,
    ANSI_C1_CSI_SGR_INVERSE = 6,
    ANSI_C1_CSI_SGR_HIDDEN = 7,
    ANSI_C1_CSI_SGR_STRIKETHROUGH = 8,
} TL_SGRDisplayAttributesIndex;
typedef struct {
    bool bold;
    bool dim;
    bool italic;
    bool underline;
    bool blinking;
    bool inverse;
    bool hidden;
    bool strikethrough;
} TL_SGRDisplayAttributes;
extern const char* const TL_SGRDisplayAttributesPS[];

typedef enum {
    ANSI_C1_CSI_SGR_FG_BLACK = 0,
    ANSI_C1_CSI_SGR_FG_RED = 1,
    ANSI_C1_CSI_SGR_FG_GREEN = 2,
    ANSI_C1_CSI_SGR_FG_YELLOW = 3,
    ANSI_C1_CSI_SGR_FG_BLUE = 4,
    ANSI_C1_CSI_SGR_FG_MAGENTA = 5,
    ANSI_C1_CSI_SGR_FG_CYAN = 6,
    ANSI_C1_CSI_SGR_FG_WHITE = 7,
    ANSI_C1_CSI_SGR_FG_DEFAULT = 8,
    ANSI_C1_CSI_SGR_BG_BLACK = 9,
    ANSI_C1_CSI_SGR_BG_RED = 10,
    ANSI_C1_CSI_SGR_BG_GREEN = 11,
    ANSI_C1_CSI_SGR_BG_YELLOW = 12,
    ANSI_C1_CSI_SGR_BG_BLUE = 13,
    ANSI_C1_CSI_SGR_BG_MAGENTA = 14,
    ANSI_C1_CSI_SGR_BG_CYAN = 15,
    ANSI_C1_CSI_SGR_BG_WHITE = 16,
    ANSI_C1_CSI_SGR_BG_DEFAULT = 17,
    ANSI_C1_CSI_SGR_FG_BRIGHT_BLACK = 18,
    ANSI_C1_CSI_SGR_FG_BRIGHT_RED = 19,
    ANSI_C1_CSI_SGR_FG_BRIGHT_GREEN = 20,
    ANSI_C1_CSI_SGR_FG_BRIGHT_YELLOW = 21,
    ANSI_C1_CSI_SGR_FG_BRIGHT_BLUE = 22,
    ANSI_C1_CSI_SGR_FG_BRIGHT_MAGENTA = 23,
    ANSI_C1_CSI_SGR_FG_BRIGHT_CYAN = 24,
    ANSI_C1_CSI_SGR_FG_BRIGHT_WHITE = 25,
    ANSI_C1_CSI_SGR_BG_BRIGHT_BLACK = 26,
    ANSI_C1_CSI_SGR_BG_BRIGHT_RED = 27,
    ANSI_C1_CSI_SGR_BG_BRIGHT_GREEN = 28,
    ANSI_C1_CSI_SGR_BG_BRIGHT_YELLOW = 29,
    ANSI_C1_CSI_SGR_BG_BRIGHT_BLUE = 30,
    ANSI_C1_CSI_SGR_BG_BRIGHT_MAGENTA = 31,
    ANSI_C1_CSI_SGR_BG_BRIGHT_CYAN = 32,
    ANSI_C1_CSI_SGR_BG_BRIGHT_WHITE = 33,
} TL_SGRColors3Bit4BitIndex;
typedef struct {
    TL_SGRColors3Bit4BitIndex foreground;
    TL_SGRColors3Bit4BitIndex background;
} TL_SGRColors3Bit4Bit;
extern const char* const TL_SGRColors3Bit4BitPS[];

typedef struct {
    int foreground;
    int background;
} TL_SGRColors8Bit;

typedef struct {
    bool enable;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} TL_RGB;

typedef struct {
    TL_RGB foreground;
    TL_RGB background;
} TL_SGRColorsTruecolor;

typedef enum {
    ANSI_C1_CSI_SGR_DISPLAYATTRIBUTES = 0,
    ANSI_C1_CSI_SGR_3BIT4BIT = 1,
    ANSI_C1_CSI_SGR_8BIT = 2,
    ANSI_C1_CSI_SGR_TRUECOLOR = 3,
} _tl_sequence_type;

typedef struct {
    TL_SGRDisplayAttributes da;
    TL_SGRColors3Bit4Bit c3bit4bit;
    TL_SGRColors8Bit c8bit;
    TL_SGRColorsTruecolor ctruecolor;
} TL_Sequence;

/*
========================
==###IMPLEMENTATION###==
========================
*/

#ifdef TINT_LIB_IMPLEMENTATION

#ifndef TINT_LIB_C0_ESC
#define TINT_LIB_C0_ESC ANSI_C0_ESC_OCTAL
#endif

const char* const TL_ANSIC0ESC[] = {"\033", "\x1B"};
const char* const TL_SGRDisplayAttributesPS[] = {"0", "1", "2", "3", "4", "5", "7", "8", "9"};
const char* const TL_SGRColors3Bit4BitPS[] = {"30", "31", "32", "33", "34", "35", "36", "37", "39", "40", "41", "42", "43", "44", "45", "46", "47", "49", "90" "91", "92", "93", "94", "95", "96", "97", "100", "101", "102", "103", "104", "105", "106", "107"};

#define _apply_property(msg, property, presentation)\
        do {\
            if (property) {\
                snprintf(msg + strlen(msg), sizeof(msg) - strlen(msg), "%s;", presentation);\
            }\
        } while(0)\

static inline int _tl_build_sequence(FILE *stream, _tl_sequence_type type, TL_Sequence sequence, const char* fmt, ...) {

    size_t msg_seq_len = 256;
    char msg_seq[msg_seq_len];
    memset(msg_seq, 0, msg_seq_len);

    strcpy(msg_seq, TL_ANSIC0ESC[TINT_LIB_C0_ESC]);
    strcat(msg_seq, "[");

    _apply_property(msg_seq, sequence.da.bold, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_BOLD]);
    _apply_property(msg_seq, sequence.da.dim, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_DIM]);
    _apply_property(msg_seq, sequence.da.italic, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_ITALIC]);
    _apply_property(msg_seq, sequence.da.underline, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_UNDERLINE]);
    _apply_property(msg_seq, sequence.da.blinking, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_BLINKING]);
    _apply_property(msg_seq, sequence.da.inverse, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_INVERSE]);
    _apply_property(msg_seq, sequence.da.hidden, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_HIDDEN]);
    _apply_property(msg_seq, sequence.da.strikethrough, TL_SGRDisplayAttributesPS[ANSI_C1_CSI_SGR_STRIKETHROUGH]);

    switch(type) {
        case ANSI_C1_CSI_SGR_3BIT4BIT:
            snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "%s;%s;", TL_SGRColors3Bit4BitPS[sequence.c3bit4bit.foreground], TL_SGRColors3Bit4BitPS[sequence.c3bit4bit.background]);
            break;
        case ANSI_C1_CSI_SGR_8BIT:
            if(sequence.c8bit.foreground >= 0) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "38;5;%d;", sequence.c8bit.foreground);
            }
            if(sequence.c8bit.background >= 0) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "48;5;%d;", sequence.c8bit.background);
            }
            break;
        case ANSI_C1_CSI_SGR_TRUECOLOR:
            if(sequence.ctruecolor.foreground.enable) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "38;2;%d;%d;%d;", sequence.ctruecolor.foreground.r, sequence.ctruecolor.foreground.g, sequence.ctruecolor.foreground.b); 
            }
            if(sequence.ctruecolor.background.enable) {
                snprintf(msg_seq + strlen(msg_seq), sizeof(msg_seq) - strlen(msg_seq), "48;2;%d;%d;%d;", sequence.ctruecolor.background.r, sequence.ctruecolor.background.g, sequence.ctruecolor.background.b);
            }
            break;
        case ANSI_C1_CSI_SGR_DISPLAYATTRIBUTES:
            break;
    }

    strcat(msg_seq, "m");

    char msg[2048] = {0};
    snprintf(msg, sizeof(msg), "%s%s%s[0m", msg_seq, fmt, TL_ANSIC0ESC[TINT_LIB_C0_ESC]);

    va_list args;
    va_start(args, fmt);
    int bytes_printed = vfprintf(stream, msg, args);
    va_end(args);

    return bytes_printed;

};

#define TL_printfc3bit4bit(sequence, ...) _tl_build_sequence(stdout, ANSI_C1_CSI_SGR_3BIT4BIT, sequence, ##__VA_ARGS__)
#define TL_printfc8bit(sequence, ...) _tl_build_sequence(stdout, ANSI_C1_CSI_SGR_8BIT, sequence, ##__VA_ARGS__)
#define TL_printfcTruecolor(sequence, ...) _tl_build_sequence(stdout, ANSI_C1_CSI_SGR_TRUECOLOR, sequence, ##__VA_ARGS__)
#define TL_printfDa(sequence, ...) _tl_build_sequence(stdout, ANSI_C1_CSI_SGR_DISPLAYATTRIBUTES, sequence, ##__VA_ARGS__)

#define TL_fprintfc3bit4bit(stream, sequence, ...) _tl_build_sequence(stream, ANSI_C1_CSI_SGR_3BIT4BIT, sequence, ##__VA_ARGS__)
#define TL_fprintfc8bit(stream, sequence, ...) _tl_build_sequence(stream, ANSI_C1_CSI_SGR_8BIT, sequence, ##__VA_ARGS__)
#define TL_fprintfcTruecolor(stream, sequence, ...) _tl_build_sequence(stream, ANSI_C1_CSI_SGR_TRUECOLOR, sequence, ##__VA_ARGS__)
#define TL_fprintfDa(stream, sequence, ...) _tl_build_sequence(stream, ANSI_C1_CSI_SGR_DISPLAYATTRIBUTES, sequence, ##__VA_ARGS__)

void _tl_copy_sequence(int type, TL_Sequence src, TL_Sequence *dest) {
    
    switch (type) {
        case -1:
            dest->da = src.da;
            dest->c3bit4bit = src.c3bit4bit;
            dest->c8bit = src.c8bit;
            dest->ctruecolor = src.ctruecolor;
            break;
        case ANSI_C1_CSI_SGR_3BIT4BIT:
            dest->c3bit4bit = src.c3bit4bit;
            break;
        case ANSI_C1_CSI_SGR_8BIT:
            dest->c8bit = src.c8bit;
            break;
        case ANSI_C1_CSI_SGR_TRUECOLOR:
            dest->ctruecolor = src.ctruecolor;
            break;
        case ANSI_C1_CSI_SGR_DISPLAYATTRIBUTES:
            dest->da = src.da;
            break;
    } 

};

#define TL_CopySequence(src, dest) _tl_copy_sequence(-1, src, dest)
#define TL_CopySequence3bit4bit(src, dest) _tl_copy_sequence(ANSI_C1_CSI_SGR_3BIT4BIT, src, dest)
#define TL_CopySequence8bit(src, dest) _tl_copy_sequence(ANSI_C1_CSI_SGR_8BIT, src, dest)
#define TL_CopySequenceTruecolor(src, dest) _tl_copy_sequence(ANSI_C1_CSI_SGR_TRUECOLOR, src, dest)
#define TL_CopySequenceDa(src, dest) _tl_copy_sequence(ANSI_C1_CSI_SGR_DISPLAYATTRIBUTES, src, dest)

#endif

#endif
