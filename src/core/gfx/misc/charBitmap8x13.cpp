#include "pch.h"
#include "charBitmap.h"

static const uint8          ch0data[] = {0x0};
static const BitmapCharDesc ch0       = {1, 1, 0, 0, 8, ch0data};

static const uint8          ch32data[] = {0x0};
static const BitmapCharDesc ch32       = {1, 1, 0, 0, 8, ch32data};

static const uint8          ch127data[] = {0x0};
static const BitmapCharDesc ch127       = {1, 1, 0, 0, 8, ch127data};

static const uint8          ch160data[] = {0x0};
static const BitmapCharDesc ch160       = {1, 1, 0, 0, 8, ch160data};

/* char: 0xff */

static const uint8 ch255data[] = {
    0x78, 0x84, 0x4, 0x74, 0x8c, 0x84, 0x84, 0x84, 0x0, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch255 = {6, 12, -1, 2, 8, ch255data};

/* char: 0xfe */

static const uint8 ch254data[] = {
    0x80, 0x80, 0xb8, 0xc4, 0x84, 0x84, 0xc4, 0xb8, 0x80, 0x80,
};

static const BitmapCharDesc ch254 = {6, 10, -1, 2, 8, ch254data};

/* char: 0xfd */

static const uint8 ch253data[] = {
    0x78, 0x84, 0x4, 0x74, 0x8c, 0x84, 0x84, 0x84, 0x0, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch253 = {6, 12, -1, 2, 8, ch253data};

/* char: 0xfc */

static const uint8 ch252data[] = {
    0x74, 0x88, 0x88, 0x88, 0x88, 0x88, 0x0, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch252 = {6, 10, -1, 0, 8, ch252data};

/* char: 0xfb */

static const uint8 ch251data[] = {
    0x74, 0x88, 0x88, 0x88, 0x88, 0x88, 0x0, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch251 = {6, 10, -1, 0, 8, ch251data};

/* char: 0xfa */

static const uint8 ch250data[] = {
    0x74, 0x88, 0x88, 0x88, 0x88, 0x88, 0x0, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch250 = {6, 10, -1, 0, 8, ch250data};

/* char: 0xf9 */

static const uint8 ch249data[] = {
    0x74, 0x88, 0x88, 0x88, 0x88, 0x88, 0x0, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch249 = {6, 10, -1, 0, 8, ch249data};

/* char: 0xf8 */

static const uint8 ch248data[] = {
    0x80, 0x78, 0xc4, 0xa4, 0x94, 0x8c, 0x78, 0x4,
};

static const BitmapCharDesc ch248 = {6, 8, -1, 1, 8, ch248data};

/* char: 0xf7 */

static const uint8 ch247data[] = {
    0x20, 0x20, 0x0, 0xf8, 0x0, 0x20, 0x20,
};

static const BitmapCharDesc ch247 = {5, 7, -1, -1, 8, ch247data};

/* char: 0xf6 */

static const uint8 ch246data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x0, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch246 = {6, 10, -1, 0, 8, ch246data};

/* char: 0xf5 */

static const uint8 ch245data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x0, 0x0, 0x50, 0x28,
};

static const BitmapCharDesc ch245 = {6, 10, -1, 0, 8, ch245data};

/* char: 0xf4 */

static const uint8 ch244data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x0, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch244 = {6, 10, -1, 0, 8, ch244data};

/* char: 0xf3 */

static const uint8 ch243data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x0, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch243 = {6, 10, -1, 0, 8, ch243data};

/* char: 0xf2 */

static const uint8 ch242data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x0, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch242 = {6, 10, -1, 0, 8, ch242data};

/* char: 0xf1 */

static const uint8 ch241data[] = {
    0x84, 0x84, 0x84, 0x84, 0xc4, 0xb8, 0x0, 0x0, 0x50, 0x28,
};

static const BitmapCharDesc ch241 = {6, 10, -1, 0, 8, ch241data};

/* char: 0xf0 */

static const uint8 ch240data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x8, 0x50, 0x30, 0x48,
};

static const BitmapCharDesc ch240 = {6, 10, -1, 0, 8, ch240data};

/* char: 0xef */

static const uint8 ch239data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x60, 0x0, 0x0, 0x50, 0x50,
};

static const BitmapCharDesc ch239 = {5, 10, -1, 0, 8, ch239data};

/* char: 0xee */

static const uint8 ch238data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x60, 0x0, 0x0, 0x90, 0x60,
};

static const BitmapCharDesc ch238 = {5, 10, -1, 0, 8, ch238data};

/* char: 0xed */

static const uint8 ch237data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x60, 0x0, 0x0, 0x40, 0x20,
};

static const BitmapCharDesc ch237 = {5, 10, -1, 0, 8, ch237data};

/* char: 0xec */

static const uint8 ch236data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x60, 0x0, 0x0, 0x20, 0x40,
};

static const BitmapCharDesc ch236 = {5, 10, -1, 0, 8, ch236data};

/* char: 0xeb */

static const uint8 ch235data[] = {
    0x78, 0x84, 0x80, 0xfc, 0x84, 0x78, 0x0, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch235 = {6, 10, -1, 0, 8, ch235data};

/* char: 0xea */

static const uint8 ch234data[] = {
    0x78, 0x84, 0x80, 0xfc, 0x84, 0x78, 0x0, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch234 = {6, 10, -1, 0, 8, ch234data};

/* char: 0xe9 */

static const uint8 ch233data[] = {
    0x78, 0x84, 0x80, 0xfc, 0x84, 0x78, 0x0, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch233 = {6, 10, -1, 0, 8, ch233data};

/* char: 0xe8 */

static const uint8 ch232data[] = {
    0x78, 0x84, 0x80, 0xfc, 0x84, 0x78, 0x0, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch232 = {6, 10, -1, 0, 8, ch232data};

/* char: 0xe7 */

static const uint8 ch231data[] = {
    0x20, 0x10, 0x78, 0x84, 0x80, 0x80, 0x84, 0x78,
};

static const BitmapCharDesc ch231 = {6, 8, -1, 2, 8, ch231data};

/* char: 0xe6 */

static const uint8 ch230data[] = {
    0x6c, 0x92, 0x90, 0x7c, 0x12, 0x6c,
};

static const BitmapCharDesc ch230 = {7, 6, 0, 0, 8, ch230data};

/* char: 0xe5 */

static const uint8 ch229data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78, 0x0, 0x30, 0x48, 0x30,
};

static const BitmapCharDesc ch229 = {6, 10, -1, 0, 8, ch229data};

/* char: 0xe4 */

static const uint8 ch228data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78, 0x0, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch228 = {6, 10, -1, 0, 8, ch228data};

/* char: 0xe3 */

static const uint8 ch227data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78, 0x0, 0x0, 0x50, 0x28,
};

static const BitmapCharDesc ch227 = {6, 10, -1, 0, 8, ch227data};

/* char: 0xe2 */

static const uint8 ch226data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78, 0x0, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch226 = {6, 10, -1, 0, 8, ch226data};

/* char: 0xe1 */

static const uint8 ch225data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78, 0x0, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch225 = {6, 10, -1, 0, 8, ch225data};

/* char: 0xe0 */

static const uint8 ch224data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78, 0x0, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch224 = {6, 10, -1, 0, 8, ch224data};

/* char: 0xdf */

static const uint8 ch223data[] = {
    0x80, 0xb8, 0xc4, 0x84, 0x84, 0xf8, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch223 = {6, 9, -1, 1, 8, ch223data};

/* char: 0xde */

static const uint8 ch222data[] = {
    0x80, 0x80, 0x80, 0xf8, 0x84, 0x84, 0x84, 0xf8, 0x80,
};

static const BitmapCharDesc ch222 = {6, 9, -1, 0, 8, ch222data};

/* char: 0xdd */

static const uint8 ch221data[] = {
    0x20, 0x20, 0x20, 0x20, 0x50, 0x88, 0x88, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch221 = {5, 10, -1, 0, 8, ch221data};

/* char: 0xdc */

static const uint8 ch220data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch220 = {6, 10, -1, 0, 8, ch220data};

/* char: 0xdb */

static const uint8 ch219data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch219 = {6, 10, -1, 0, 8, ch219data};

/* char: 0xda */

static const uint8 ch218data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch218 = {6, 10, -1, 0, 8, ch218data};

/* char: 0xd9 */

static const uint8 ch217data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch217 = {6, 10, -1, 0, 8, ch217data};

/* char: 0xd8 */

static const uint8 ch216data[] = {
    0x80, 0x78, 0xc4, 0xa4, 0xa4, 0xa4, 0x94, 0x94, 0x8c, 0x78, 0x4,
};

static const BitmapCharDesc ch216 = {6, 11, -1, 1, 8, ch216data};

/* char: 0xd7 */

static const uint8 ch215data[] = {
    0x84, 0x48, 0x30, 0x30, 0x48, 0x84,
};

static const BitmapCharDesc ch215 = {6, 6, -1, -1, 8, ch215data};

/* char: 0xd6 */

static const uint8 ch214data[] = {
    0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x0, 0x28, 0x28,
};

static const BitmapCharDesc ch214 = {7, 10, 0, 0, 8, ch214data};

/* char: 0xd5 */

static const uint8 ch213data[] = {
    0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x0, 0x28, 0x14,
};

static const BitmapCharDesc ch213 = {7, 10, 0, 0, 8, ch213data};

/* char: 0xd4 */

static const uint8 ch212data[] = {
    0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x0, 0x24, 0x18,
};

static const BitmapCharDesc ch212 = {7, 10, 0, 0, 8, ch212data};

/* char: 0xd3 */

static const uint8 ch211data[] = {
    0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x0, 0x10, 0x8,
};

static const BitmapCharDesc ch211 = {7, 10, 0, 0, 8, ch211data};

/* char: 0xd2 */

static const uint8 ch210data[] = {
    0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x0, 0x8, 0x10,
};

static const BitmapCharDesc ch210 = {7, 10, 0, 0, 8, ch210data};

/* char: 0xd1 */

static const uint8 ch209data[] = {
    0x82, 0x86, 0x8a, 0x92, 0xa2, 0xc2, 0x82, 0x0, 0x28, 0x14,
};

static const BitmapCharDesc ch209 = {7, 10, 0, 0, 8, ch209data};

/* char: 0xd0 */

static const uint8 ch208data[] = {
    0xfc, 0x42, 0x42, 0x42, 0xe2, 0x42, 0x42, 0x42, 0xfc,
};

static const BitmapCharDesc ch208 = {7, 9, 0, 0, 8, ch208data};

/* char: 0xcf */

static const uint8 ch207data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xf8, 0x0, 0x50, 0x50,
};

static const BitmapCharDesc ch207 = {5, 10, -1, 0, 8, ch207data};

/* char: 0xce */

static const uint8 ch206data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xf8, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch206 = {5, 10, -1, 0, 8, ch206data};

/* char: 0xcd */

static const uint8 ch205data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xf8, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch205 = {5, 10, -1, 0, 8, ch205data};

/* char: 0xcc */

static const uint8 ch204data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xf8, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch204 = {5, 10, -1, 0, 8, ch204data};

/* char: 0xcb */

static const uint8 ch203data[] = {
    0xfc, 0x80, 0x80, 0xf0, 0x80, 0x80, 0xfc, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch203 = {6, 10, -1, 0, 8, ch203data};

/* char: 0xca */

static const uint8 ch202data[] = {
    0xfc, 0x80, 0x80, 0xf0, 0x80, 0x80, 0xfc, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch202 = {6, 10, -1, 0, 8, ch202data};

/* char: 0xc9 */

static const uint8 ch201data[] = {
    0xfc, 0x80, 0x80, 0xf0, 0x80, 0x80, 0xfc, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch201 = {6, 10, -1, 0, 8, ch201data};

/* char: 0xc8 */

static const uint8 ch200data[] = {
    0xfc, 0x80, 0x80, 0xf0, 0x80, 0x80, 0xfc, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch200 = {6, 10, -1, 0, 8, ch200data};

/* char: 0xc7 */

static const uint8 ch199data[] = {
    0x20, 0x10, 0x78, 0x84, 0x80, 0x80, 0x80, 0x80, 0x80, 0x84, 0x78,
};

static const BitmapCharDesc ch199 = {6, 11, -1, 2, 8, ch199data};

/* char: 0xc6 */

static const uint8 ch198data[] = {
    0x9e, 0x90, 0x90, 0xf0, 0x9c, 0x90, 0x90, 0x90, 0x6e,
};

static const BitmapCharDesc ch198 = {7, 9, 0, 0, 8, ch198data};

/* char: 0xc5 */

static const uint8 ch197data[] = {
    0x84, 0x84, 0xfc, 0x84, 0x84, 0x48, 0x30, 0x30, 0x48, 0x30,
};

static const BitmapCharDesc ch197 = {6, 10, -1, 0, 8, ch197data};

/* char: 0xc4 */

static const uint8 ch196data[] = {
    0x84, 0x84, 0xfc, 0x84, 0x84, 0x48, 0x30, 0x0, 0x48, 0x48,
};

static const BitmapCharDesc ch196 = {6, 10, -1, 0, 8, ch196data};

/* char: 0xc3 */

static const uint8 ch195data[] = {
    0x84, 0x84, 0xfc, 0x84, 0x84, 0x48, 0x30, 0x0, 0x50, 0x28,
};

static const BitmapCharDesc ch195 = {6, 10, -1, 0, 8, ch195data};

/* char: 0xc2 */

static const uint8 ch194data[] = {
    0x84, 0x84, 0xfc, 0x84, 0x84, 0x48, 0x30, 0x0, 0x48, 0x30,
};

static const BitmapCharDesc ch194 = {6, 10, -1, 0, 8, ch194data};

/* char: 0xc1 */

static const uint8 ch193data[] = {
    0x84, 0x84, 0xfc, 0x84, 0x84, 0x48, 0x30, 0x0, 0x20, 0x10,
};

static const BitmapCharDesc ch193 = {6, 10, -1, 0, 8, ch193data};

/* char: 0xc0 */

static const uint8 ch192data[] = {
    0x84, 0x84, 0xfc, 0x84, 0x84, 0x48, 0x30, 0x0, 0x10, 0x20,
};

static const BitmapCharDesc ch192 = {6, 10, -1, 0, 8, ch192data};

/* char: 0xbf */

static const uint8 ch191data[] = {
    0x78, 0x84, 0x84, 0x80, 0x40, 0x20, 0x20, 0x0, 0x20,
};

static const BitmapCharDesc ch191 = {6, 9, -1, 0, 8, ch191data};

/* char: 0xbe */

static const uint8 ch190data[] = {
    0x6, 0x1a, 0x12, 0xa, 0x66, 0x92, 0x10, 0x20, 0x90, 0x60,
};

static const BitmapCharDesc ch190 = {7, 10, 0, 0, 8, ch190data};

/* char: 0xbd */

static const uint8 ch189data[] = {
    0x1e, 0x10, 0xc, 0x2, 0xf2, 0x4c, 0x40, 0x40, 0xc0, 0x40,
};

static const BitmapCharDesc ch189 = {7, 10, 0, 0, 8, ch189data};

/* char: 0xbc */

static const uint8 ch188data[] = {
    0x6, 0x1a, 0x12, 0xa, 0xe6, 0x42, 0x40, 0x40, 0xc0, 0x40,
};

static const BitmapCharDesc ch188 = {7, 10, 0, 0, 8, ch188data};

/* char: 0xbb */

static const uint8 ch187data[] = {
    0x90, 0x48, 0x24, 0x12, 0x24, 0x48, 0x90,
};

static const BitmapCharDesc ch187 = {7, 7, 0, -1, 8, ch187data};

/* char: 0xba */

static const uint8 ch186data[] = {
    0xf0, 0x0, 0x60, 0x90, 0x90, 0x60,
};

static const BitmapCharDesc ch186 = {4, 6, -1, -3, 8, ch186data};

/* char: 0xb9 */

static const uint8 ch185data[] = {
    0xe0, 0x40, 0x40, 0x40, 0xc0, 0x40,
};

static const BitmapCharDesc ch185 = {3, 6, -1, -4, 8, ch185data};

/* char: 0xb8 */

static const uint8 ch184data[] = {
    0xc0,
    0x40,
};

static const BitmapCharDesc ch184 = {2, 2, -3, 2, 8, ch184data};

/* char: 0xb7 */

static const uint8 ch183data[] = {
    0xc0,
};

static const BitmapCharDesc ch183 = {2, 1, -3, -4, 8, ch183data};

/* char: 0xb6 */

static const uint8 ch182data[] = {
    0x28, 0x28, 0x28, 0x28, 0x68, 0xe8, 0xe8, 0xe8, 0x7c,
};

static const BitmapCharDesc ch182 = {6, 9, -1, 0, 8, ch182data};

/* char: 0xb5 */

static const uint8 ch181data[] = {
    0x80, 0xb4, 0xcc, 0x84, 0x84, 0x84, 0x84,
};

static const BitmapCharDesc ch181 = {6, 7, -1, 1, 8, ch181data};

/* char: 0xb4 */

static const uint8 ch180data[] = {
    0x80,
    0x40,
};

static const BitmapCharDesc ch180 = {2, 2, -3, -8, 8, ch180data};

/* char: 0xb3 */

static const uint8 ch179data[] = {
    0x60, 0x90, 0x10, 0x20, 0x90, 0x60,
};

static const BitmapCharDesc ch179 = {4, 6, -1, -4, 8, ch179data};

/* char: 0xb2 */

static const uint8 ch178data[] = {
    0xf0, 0x80, 0x60, 0x10, 0x90, 0x60,
};

static const BitmapCharDesc ch178 = {4, 6, -1, -4, 8, ch178data};

/* char: 0xb1 */

static const uint8 ch177data[] = {
    0xf8, 0x0, 0x20, 0x20, 0xf8, 0x20, 0x20,
};

static const BitmapCharDesc ch177 = {5, 7, -1, -1, 8, ch177data};

/* char: 0xb0 */

static const uint8 ch176data[] = {
    0x60,
    0x90,
    0x90,
    0x60,
};

static const BitmapCharDesc ch176 = {4, 4, -2, -5, 8, ch176data};

/* char: 0xaf */

static const uint8 ch175data[] = {
    0xfc,
};

static const BitmapCharDesc ch175 = {6, 1, -1, -8, 8, ch175data};

/* char: 0xae */

static const uint8 ch174data[] = {
    0x38, 0x44, 0xaa, 0xb2, 0xaa, 0xaa, 0x92, 0x44, 0x38,
};

static const BitmapCharDesc ch174 = {7, 9, 0, -1, 8, ch174data};

/* char: 0xad */

static const uint8 ch173data[] = {
    0xfc,
};

static const BitmapCharDesc ch173 = {6, 1, -1, -4, 8, ch173data};

/* char: 0xac */

static const uint8 ch172data[] = {
    0x4,
    0x4,
    0x4,
    0xfc,
};

static const BitmapCharDesc ch172 = {6, 4, -1, -1, 8, ch172data};

/* char: 0xab */

static const uint8 ch171data[] = {
    0x12, 0x24, 0x48, 0x90, 0x48, 0x24, 0x12,
};

static const BitmapCharDesc ch171 = {7, 7, 0, -1, 8, ch171data};

/* char: 0xaa */

static const uint8 ch170data[] = {
    0xf8, 0x0, 0x78, 0x88, 0x78, 0x8, 0x70,
};

static const BitmapCharDesc ch170 = {5, 7, -1, -2, 8, ch170data};

/* char: 0xa9 */

static const uint8 ch169data[] = {
    0x38, 0x44, 0x92, 0xaa, 0xa2, 0xaa, 0x92, 0x44, 0x38,
};

static const BitmapCharDesc ch169 = {7, 9, 0, -1, 8, ch169data};

/* char: 0xa8 */

static const uint8 ch168data[] = {
    0xd8,
};

static const BitmapCharDesc ch168 = {5, 1, -1, -8, 8, ch168data};

/* char: 0xa7 */

static const uint8 ch167data[] = {
    0x60, 0x90, 0x10, 0x60, 0x90, 0x90, 0x60, 0x80, 0x90, 0x60,
};

static const BitmapCharDesc ch167 = {4, 10, -2, 0, 8, ch167data};

/* char: 0xa6 */

static const uint8 ch166data[] = {
    0x80, 0x80, 0x80, 0x80, 0x0, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch166 = {1, 9, -3, 0, 8, ch166data};

/* char: 0xa5 */

static const uint8 ch165data[] = {
    0x10, 0x10, 0x7c, 0x10, 0x7c, 0x28, 0x44, 0x82, 0x82,
};

static const BitmapCharDesc ch165 = {7, 9, 0, 0, 8, ch165data};

/* char: 0xa4 */

static const uint8 ch164data[] = {
    0x84, 0x78, 0x48, 0x48, 0x78, 0x84,
};

static const BitmapCharDesc ch164 = {6, 6, -1, -1, 8, ch164data};

/* char: 0xa3 */

static const uint8 ch163data[] = {
    0xdc, 0x62, 0x20, 0x20, 0x20, 0x70, 0x20, 0x22, 0x1c,
};

static const BitmapCharDesc ch163 = {7, 9, 0, 0, 8, ch163data};

/* char: 0xa2 */

static const uint8 ch162data[] = {
    0x20, 0x70, 0xa8, 0xa0, 0xa0, 0xa8, 0x70, 0x20,
};

static const BitmapCharDesc ch162 = {5, 8, -1, -1, 8, ch162data};

/* char: 0xa1 */

static const uint8 ch161data[] = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0, 0x80,
};

static const BitmapCharDesc ch161 = {1, 9, -3, 0, 8, ch161data};

/* char: 0x7e '~' */

static const uint8 ch126data[] = {
    0x90,
    0xa8,
    0x48,
};

static const BitmapCharDesc ch126 = {5, 3, -1, -6, 8, ch126data};

/* char: 0x7d '}' */

static const uint8 ch125data[] = {
    0xe0, 0x10, 0x10, 0x20, 0x18, 0x20, 0x10, 0x10, 0xe0,
};

static const BitmapCharDesc ch125 = {5, 9, -1, 0, 8, ch125data};

/* char: 0x7c '|' */

static const uint8 ch124data[] = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch124 = {1, 9, -3, 0, 8, ch124data};

/* char: 0x7b '{' */

static const uint8 ch123data[] = {
    0x38, 0x40, 0x40, 0x20, 0xc0, 0x20, 0x40, 0x40, 0x38,
};

static const BitmapCharDesc ch123 = {5, 9, -2, 0, 8, ch123data};

/* char: 0x7a 'z' */

static const uint8 ch122data[] = {
    0xfc, 0x40, 0x20, 0x10, 0x8, 0xfc,
};

static const BitmapCharDesc ch122 = {6, 6, -1, 0, 8, ch122data};

/* char: 0x79 'y' */

static const uint8 ch121data[] = {
    0x78, 0x84, 0x4, 0x74, 0x8c, 0x84, 0x84, 0x84,
};

static const BitmapCharDesc ch121 = {6, 8, -1, 2, 8, ch121data};

/* char: 0x78 'x' */

static const uint8 ch120data[] = {
    0x84, 0x48, 0x30, 0x30, 0x48, 0x84,
};

static const BitmapCharDesc ch120 = {6, 6, -1, 0, 8, ch120data};

/* char: 0x77 'w' */

static const uint8 ch119data[] = {
    0x44, 0xaa, 0x92, 0x92, 0x82, 0x82,
};

static const BitmapCharDesc ch119 = {7, 6, 0, 0, 8, ch119data};

/* char: 0x76 'v' */

static const uint8 ch118data[] = {
    0x20, 0x50, 0x50, 0x88, 0x88, 0x88,
};

static const BitmapCharDesc ch118 = {5, 6, -1, 0, 8, ch118data};

/* char: 0x75 'u' */

static const uint8 ch117data[] = {
    0x74, 0x88, 0x88, 0x88, 0x88, 0x88,
};

static const BitmapCharDesc ch117 = {6, 6, -1, 0, 8, ch117data};

/* char: 0x74 't' */

static const uint8 ch116data[] = {
    0x38, 0x44, 0x40, 0x40, 0x40, 0xf8, 0x40, 0x40,
};

static const BitmapCharDesc ch116 = {6, 8, -1, 0, 8, ch116data};

/* char: 0x73 's' */

static const uint8 ch115data[] = {
    0x78, 0x84, 0x18, 0x60, 0x84, 0x78,
};

static const BitmapCharDesc ch115 = {6, 6, -1, 0, 8, ch115data};

/* char: 0x72 'r' */

static const uint8 ch114data[] = {
    0x40, 0x40, 0x40, 0x40, 0x44, 0xb8,
};

static const BitmapCharDesc ch114 = {6, 6, -1, 0, 8, ch114data};

/* char: 0x71 'q' */

static const uint8 ch113data[] = {
    0x4, 0x4, 0x4, 0x74, 0x8c, 0x84, 0x8c, 0x74,
};

static const BitmapCharDesc ch113 = {6, 8, -1, 2, 8, ch113data};

/* char: 0x70 'p' */

static const uint8 ch112data[] = {
    0x80, 0x80, 0x80, 0xb8, 0xc4, 0x84, 0xc4, 0xb8,
};

static const BitmapCharDesc ch112 = {6, 8, -1, 2, 8, ch112data};

/* char: 0x6f 'o' */

static const uint8 ch111data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch111 = {6, 6, -1, 0, 8, ch111data};

/* char: 0x6e 'n' */

static const uint8 ch110data[] = {
    0x84, 0x84, 0x84, 0x84, 0xc4, 0xb8,
};

static const BitmapCharDesc ch110 = {6, 6, -1, 0, 8, ch110data};

/* char: 0x6d 'm' */

static const uint8 ch109data[] = {
    0x82, 0x92, 0x92, 0x92, 0x92, 0xec,
};

static const BitmapCharDesc ch109 = {7, 6, 0, 0, 8, ch109data};

/* char: 0x6c 'l' */

static const uint8 ch108data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60,
};

static const BitmapCharDesc ch108 = {5, 9, -1, 0, 8, ch108data};

/* char: 0x6b 'k' */

static const uint8 ch107data[] = {
    0x84, 0x88, 0x90, 0xe0, 0x90, 0x88, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch107 = {6, 9, -1, 0, 8, ch107data};

/* char: 0x6a 'j' */

static const uint8 ch106data[] = {
    0x70, 0x88, 0x88, 0x8, 0x8, 0x8, 0x8, 0x18, 0x0, 0x8,
};

static const BitmapCharDesc ch106 = {5, 10, -1, 2, 8, ch106data};

/* char: 0x69 'i' */

static const uint8 ch105data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x60, 0x0, 0x20,
};

static const BitmapCharDesc ch105 = {5, 8, -1, 0, 8, ch105data};

/* char: 0x68 'h' */

static const uint8 ch104data[] = {
    0x84, 0x84, 0x84, 0x84, 0xc4, 0xb8, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch104 = {6, 9, -1, 0, 8, ch104data};

/* char: 0x67 'g' */

static const uint8 ch103data[] = {
    0x78, 0x84, 0x78, 0x80, 0x70, 0x88, 0x88, 0x74,
};

static const BitmapCharDesc ch103 = {6, 8, -1, 2, 8, ch103data};

/* char: 0x66 'f' */

static const uint8 ch102data[] = {
    0x40, 0x40, 0x40, 0x40, 0xf8, 0x40, 0x40, 0x44, 0x38,
};

static const BitmapCharDesc ch102 = {6, 9, -1, 0, 8, ch102data};

/* char: 0x65 'e' */

static const uint8 ch101data[] = {
    0x78, 0x84, 0x80, 0xfc, 0x84, 0x78,
};

static const BitmapCharDesc ch101 = {6, 6, -1, 0, 8, ch101data};

/* char: 0x64 'd' */

static const uint8 ch100data[] = {
    0x74, 0x8c, 0x84, 0x84, 0x8c, 0x74, 0x4, 0x4, 0x4,
};

static const BitmapCharDesc ch100 = {6, 9, -1, 0, 8, ch100data};

/* char: 0x63 'c' */

static const uint8 ch99data[] = {
    0x78, 0x84, 0x80, 0x80, 0x84, 0x78,
};

static const BitmapCharDesc ch99 = {6, 6, -1, 0, 8, ch99data};

/* char: 0x62 'b' */

static const uint8 ch98data[] = {
    0xb8, 0xc4, 0x84, 0x84, 0xc4, 0xb8, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch98 = {6, 9, -1, 0, 8, ch98data};

/* char: 0x61 'a' */

static const uint8 ch97data[] = {
    0x74, 0x8c, 0x84, 0x7c, 0x4, 0x78,
};

static const BitmapCharDesc ch97 = {6, 6, -1, 0, 8, ch97data};

/* char: 0x60 '`' */

static const uint8 ch96data[] = {
    0x10,
    0x60,
    0xe0,
};

static const BitmapCharDesc ch96 = {4, 3, -2, -6, 8, ch96data};

/* char: 0x5f '_' */

static const uint8 ch95data[] = {
    0xfe,
};

static const BitmapCharDesc ch95 = {7, 1, 0, 1, 8, ch95data};

/* char: 0x5e '^' */

static const uint8 ch94data[] = {
    0x88,
    0x50,
    0x20,
};

static const BitmapCharDesc ch94 = {5, 3, -1, -6, 8, ch94data};

/* char: 0x5d ']' */

static const uint8 ch93data[] = {
    0xf0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xf0,
};

static const BitmapCharDesc ch93 = {4, 9, -1, 0, 8, ch93data};

/* char: 0x5c '\' */

static const uint8 ch92data[] = {
    0x2, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x80,
};

static const BitmapCharDesc ch92 = {7, 9, 0, 0, 8, ch92data};

/* char: 0x5b '[' */

static const uint8 ch91data[] = {
    0xf0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xf0,
};

static const BitmapCharDesc ch91 = {4, 9, -2, 0, 8, ch91data};

/* char: 0x5a 'Z' */

static const uint8 ch90data[] = {
    0xfc, 0x80, 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0xfc,
};

static const BitmapCharDesc ch90 = {6, 9, -1, 0, 8, ch90data};

/* char: 0x59 'Y' */

static const uint8 ch89data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x28, 0x44, 0x82, 0x82,
};

static const BitmapCharDesc ch89 = {7, 9, 0, 0, 8, ch89data};

/* char: 0x58 'X' */

static const uint8 ch88data[] = {
    0x82, 0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82, 0x82,
};

static const BitmapCharDesc ch88 = {7, 9, 0, 0, 8, ch88data};

/* char: 0x57 'W' */

static const uint8 ch87data[] = {
    0x44, 0xaa, 0x92, 0x92, 0x92, 0x82, 0x82, 0x82, 0x82,
};

static const BitmapCharDesc ch87 = {7, 9, 0, 0, 8, ch87data};

/* char: 0x56 'V' */

static const uint8 ch86data[] = {
    0x10, 0x28, 0x28, 0x28, 0x44, 0x44, 0x44, 0x82, 0x82,
};

static const BitmapCharDesc ch86 = {7, 9, 0, 0, 8, ch86data};

/* char: 0x55 'U' */

static const uint8 ch85data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84,
};

static const BitmapCharDesc ch85 = {6, 9, -1, 0, 8, ch85data};

/* char: 0x54 'T' */

static const uint8 ch84data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xfe,
};

static const BitmapCharDesc ch84 = {7, 9, 0, 0, 8, ch84data};

/* char: 0x53 'S' */

static const uint8 ch83data[] = {
    0x78, 0x84, 0x4, 0x4, 0x78, 0x80, 0x80, 0x84, 0x78,
};

static const BitmapCharDesc ch83 = {6, 9, -1, 0, 8, ch83data};

/* char: 0x52 'R' */

static const uint8 ch82data[] = {
    0x84, 0x88, 0x90, 0xa0, 0xf8, 0x84, 0x84, 0x84, 0xf8,
};

static const BitmapCharDesc ch82 = {6, 9, -1, 0, 8, ch82data};

/* char: 0x51 'Q' */

static const uint8 ch81data[] = {
    0x4, 0x78, 0x94, 0xa4, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch81 = {6, 10, -1, 1, 8, ch81data};

/* char: 0x50 'P' */

static const uint8 ch80data[] = {
    0x80, 0x80, 0x80, 0x80, 0xf8, 0x84, 0x84, 0x84, 0xf8,
};

static const BitmapCharDesc ch80 = {6, 9, -1, 0, 8, ch80data};

/* char: 0x4f 'O' */

static const uint8 ch79data[] = {
    0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch79 = {6, 9, -1, 0, 8, ch79data};

/* char: 0x4e 'N' */

static const uint8 ch78data[] = {
    0x84, 0x84, 0x84, 0x8c, 0x94, 0xa4, 0xc4, 0x84, 0x84,
};

static const BitmapCharDesc ch78 = {6, 9, -1, 0, 8, ch78data};

/* char: 0x4d 'M' */

static const uint8 ch77data[] = {
    0x82, 0x82, 0x82, 0x92, 0x92, 0xaa, 0xc6, 0x82, 0x82,
};

static const BitmapCharDesc ch77 = {7, 9, 0, 0, 8, ch77data};

/* char: 0x4c 'L' */

static const uint8 ch76data[] = {
    0xfc, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch76 = {6, 9, -1, 0, 8, ch76data};

/* char: 0x4b 'K' */

static const uint8 ch75data[] = {
    0x84, 0x88, 0x90, 0xa0, 0xc0, 0xa0, 0x90, 0x88, 0x84,
};

static const BitmapCharDesc ch75 = {6, 9, -1, 0, 8, ch75data};

/* char: 0x4a 'J' */

static const uint8 ch74data[] = {
    0x70, 0x88, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x3c,
};

static const BitmapCharDesc ch74 = {6, 9, -1, 0, 8, ch74data};

/* char: 0x49 'I' */

static const uint8 ch73data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xf8,
};

static const BitmapCharDesc ch73 = {5, 9, -1, 0, 8, ch73data};

/* char: 0x48 'H' */

static const uint8 ch72data[] = {
    0x84, 0x84, 0x84, 0x84, 0xfc, 0x84, 0x84, 0x84, 0x84,
};

static const BitmapCharDesc ch72 = {6, 9, -1, 0, 8, ch72data};

/* char: 0x47 'G' */

static const uint8 ch71data[] = {
    0x74, 0x8c, 0x84, 0x9c, 0x80, 0x80, 0x80, 0x84, 0x78,
};

static const BitmapCharDesc ch71 = {6, 9, -1, 0, 8, ch71data};

/* char: 0x46 'F' */

static const uint8 ch70data[] = {
    0x80, 0x80, 0x80, 0x80, 0xf0, 0x80, 0x80, 0x80, 0xfc,
};

static const BitmapCharDesc ch70 = {6, 9, -1, 0, 8, ch70data};

/* char: 0x45 'E' */

static const uint8 ch69data[] = {
    0xfc, 0x80, 0x80, 0x80, 0xf0, 0x80, 0x80, 0x80, 0xfc,
};

static const BitmapCharDesc ch69 = {6, 9, -1, 0, 8, ch69data};

/* char: 0x44 'D' */

static const uint8 ch68data[] = {
    0xfc, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xfc,
};

static const BitmapCharDesc ch68 = {7, 9, 0, 0, 8, ch68data};

/* char: 0x43 'C' */

static const uint8 ch67data[] = {
    0x78, 0x84, 0x80, 0x80, 0x80, 0x80, 0x80, 0x84, 0x78,
};

static const BitmapCharDesc ch67 = {6, 9, -1, 0, 8, ch67data};

/* char: 0x42 'B' */

static const uint8 ch66data[] = {
    0xfc, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x42, 0xfc,
};

static const BitmapCharDesc ch66 = {7, 9, 0, 0, 8, ch66data};

/* char: 0x41 'A' */

static const uint8 ch65data[] = {
    0x84, 0x84, 0x84, 0xfc, 0x84, 0x84, 0x84, 0x48, 0x30,
};

static const BitmapCharDesc ch65 = {6, 9, -1, 0, 8, ch65data};

/* char: 0x40 '@' */

static const uint8 ch64data[] = {
    0x78, 0x80, 0x94, 0xac, 0xa4, 0x9c, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch64 = {6, 9, -1, 0, 8, ch64data};

/* char: 0x3f '?' */

static const uint8 ch63data[] = {
    0x10, 0x0, 0x10, 0x10, 0x8, 0x4, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch63 = {6, 9, -1, 0, 8, ch63data};

/* char: 0x3e '>' */

static const uint8 ch62data[] = {
    0x80, 0x40, 0x20, 0x10, 0x8, 0x10, 0x20, 0x40, 0x80,
};

static const BitmapCharDesc ch62 = {5, 9, -1, 0, 8, ch62data};

/* char: 0x3d '=' */

static const uint8 ch61data[] = {
    0xfc,
    0x0,
    0x0,
    0xfc,
};

static const BitmapCharDesc ch61 = {6, 4, -1, -2, 8, ch61data};

/* char: 0x3c '<' */

static const uint8 ch60data[] = {
    0x8, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x8,
};

static const BitmapCharDesc ch60 = {5, 9, -2, 0, 8, ch60data};

/* char: 0x3b ';' */

static const uint8 ch59data[] = {
    0x80, 0x60, 0x70, 0x0, 0x0, 0x20, 0x70, 0x20,
};

static const BitmapCharDesc ch59 = {4, 8, -1, 1, 8, ch59data};

/* char: 0x3a ':' */

static const uint8 ch58data[] = {
    0x40, 0xe0, 0x40, 0x0, 0x0, 0x40, 0xe0, 0x40,
};

static const BitmapCharDesc ch58 = {3, 8, -2, 1, 8, ch58data};

/* char: 0x39 '9' */

static const uint8 ch57data[] = {
    0x70, 0x8, 0x4, 0x4, 0x74, 0x8c, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch57 = {6, 9, -1, 0, 8, ch57data};

/* char: 0x38 '8' */

static const uint8 ch56data[] = {
    0x78, 0x84, 0x84, 0x84, 0x78, 0x84, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch56 = {6, 9, -1, 0, 8, ch56data};

/* char: 0x37 '7' */

static const uint8 ch55data[] = {
    0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x8, 0x4, 0xfc,
};

static const BitmapCharDesc ch55 = {6, 9, -1, 0, 8, ch55data};

/* char: 0x36 '6' */

static const uint8 ch54data[] = {
    0x78, 0x84, 0x84, 0xc4, 0xb8, 0x80, 0x80, 0x40, 0x38,
};

static const BitmapCharDesc ch54 = {6, 9, -1, 0, 8, ch54data};

/* char: 0x35 '5' */

static const uint8 ch53data[] = {
    0x78, 0x84, 0x4, 0x4, 0xc4, 0xb8, 0x80, 0x80, 0xfc,
};

static const BitmapCharDesc ch53 = {6, 9, -1, 0, 8, ch53data};

/* char: 0x34 '4' */

static const uint8 ch52data[] = {
    0x8, 0x8, 0xfc, 0x88, 0x88, 0x48, 0x28, 0x18, 0x8,
};

static const BitmapCharDesc ch52 = {6, 9, -1, 0, 8, ch52data};

/* char: 0x33 '3' */

static const uint8 ch51data[] = {
    0x78, 0x84, 0x4, 0x4, 0x38, 0x10, 0x8, 0x4, 0xfc,
};

static const BitmapCharDesc ch51 = {6, 9, -1, 0, 8, ch51data};

/* char: 0x32 '2' */

static const uint8 ch50data[] = {
    0xfc, 0x80, 0x40, 0x30, 0x8, 0x4, 0x84, 0x84, 0x78,
};

static const BitmapCharDesc ch50 = {6, 9, -1, 0, 8, ch50data};

/* char: 0x31 '1' */

static const uint8 ch49data[] = {
    0xf8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xa0, 0x60, 0x20,
};

static const BitmapCharDesc ch49 = {5, 9, -1, 0, 8, ch49data};

/* char: 0x30 '0' */

static const uint8 ch48data[] = {
    0x30, 0x48, 0x84, 0x84, 0x84, 0x84, 0x84, 0x48, 0x30,
};

static const BitmapCharDesc ch48 = {6, 9, -1, 0, 8, ch48data};

/* char: 0x2f '/' */

static const uint8 ch47data[] = {
    0x80, 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x2,
};

static const BitmapCharDesc ch47 = {7, 9, 0, 0, 8, ch47data};

/* char: 0x2e '.' */

static const uint8 ch46data[] = {
    0x40,
    0xe0,
    0x40,
};

static const BitmapCharDesc ch46 = {3, 3, -2, 1, 8, ch46data};

/* char: 0x2d '-' */

static const uint8 ch45data[] = {
    0xfc,
};

static const BitmapCharDesc ch45 = {6, 1, -1, -4, 8, ch45data};

/* char: 0x2c ',' */

static const uint8 ch44data[] = {
    0x80,
    0x60,
    0x70,
};

static const BitmapCharDesc ch44 = {4, 3, -1, 1, 8, ch44data};

/* char: 0x2b '+' */

static const uint8 ch43data[] = {
    0x20, 0x20, 0xf8, 0x20, 0x20,
};

static const BitmapCharDesc ch43 = {5, 5, -1, -2, 8, ch43data};

/* char: 0x2a '*' */

static const uint8 ch42data[] = {
    0x48, 0x30, 0xfc, 0x30, 0x48,
};

static const BitmapCharDesc ch42 = {6, 5, -1, -2, 8, ch42data};

/* char: 0x29 ')' */

static const uint8 ch41data[] = {
    0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80,
};

static const BitmapCharDesc ch41 = {3, 9, -2, 0, 8, ch41data};

/* char: 0x28 '(' */

static const uint8 ch40data[] = {
    0x20, 0x40, 0x40, 0x80, 0x80, 0x80, 0x40, 0x40, 0x20,
};

static const BitmapCharDesc ch40 = {3, 9, -3, 0, 8, ch40data};

/* char: 0x27 ''' */

static const uint8 ch39data[] = {
    0x80,
    0x60,
    0x70,
};

static const BitmapCharDesc ch39 = {4, 3, -1, -6, 8, ch39data};

/* char: 0x26 '&' */

static const uint8 ch38data[] = {
    0x74, 0x88, 0x94, 0x60, 0x90, 0x90, 0x60,
};

static const BitmapCharDesc ch38 = {6, 7, -1, 0, 8, ch38data};

/* char: 0x25 '%' */

static const uint8 ch37data[] = {
    0x88, 0x54, 0x48, 0x20, 0x10, 0x10, 0x48, 0xa4, 0x44,
};

static const BitmapCharDesc ch37 = {6, 9, -1, 0, 8, ch37data};

/* char: 0x24 '$' */

static const uint8 ch36data[] = {
    0x20, 0xf0, 0x28, 0x70, 0xa0, 0x78, 0x20,
};

static const BitmapCharDesc ch36 = {5, 7, -1, -1, 8, ch36data};

/* char: 0x23 '#' */

static const uint8 ch35data[] = {
    0x48, 0x48, 0xfc, 0x48, 0xfc, 0x48, 0x48,
};

static const BitmapCharDesc ch35 = {6, 7, -1, -1, 8, ch35data};

/* char: 0x22 '"' */

static const uint8 ch34data[] = {
    0x90,
    0x90,
    0x90,
};

static const BitmapCharDesc ch34 = {4, 3, -2, -6, 8, ch34data};

/* char: 0x21 '!' */

static const uint8 ch33data[] = {
    0x80, 0x0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch33 = {1, 9, -3, 0, 8, ch33data};

/* char: 0x1f */

static const uint8 ch31data[] = {
    0x80,
};

static const BitmapCharDesc ch31 = {1, 1, -3, -3, 8, ch31data};

/* char: 0x1e */

static const uint8 ch30data[] = {
    0xdc, 0x62, 0x20, 0x20, 0x20, 0x70, 0x20, 0x22, 0x1c,
};

static const BitmapCharDesc ch30 = {7, 9, 0, 0, 8, ch30data};

/* char: 0x1d */

static const uint8 ch29data[] = {
    0x80, 0x40, 0xfe, 0x10, 0xfe, 0x4, 0x2,
};

static const BitmapCharDesc ch29 = {7, 7, 0, 0, 8, ch29data};

/* char: 0x1c */

static const uint8 ch28data[] = {
    0x88, 0x48, 0x48, 0x48, 0x48, 0xfc,
};

static const BitmapCharDesc ch28 = {6, 6, -1, 0, 8, ch28data};

/* char: 0x1b */

static const uint8 ch27data[] = {
    0xfe, 0x80, 0x20, 0x8, 0x2, 0x8, 0x20, 0x80,
};

static const BitmapCharDesc ch27 = {7, 8, 0, 0, 8, ch27data};

/* char: 0x1a */

static const uint8 ch26data[] = {
    0xfe, 0x2, 0x8, 0x20, 0x80, 0x20, 0x8, 0x2,
};

static const BitmapCharDesc ch26 = {7, 8, 0, 0, 8, ch26data};

/* char: 0x19 */

static const uint8 ch25data[] = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch25 = {1, 13, -3, 2, 8, ch25data};

/* char: 0x18 */

static const uint8 ch24data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0xff,
};

static const BitmapCharDesc ch24 = {8, 6, 0, 2, 8, ch24data};

/* char: 0x17 */

static const uint8 ch23data[] = {
    0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

static const BitmapCharDesc ch23 = {8, 8, 0, -3, 8, ch23data};

/* char: 0x16 */

static const uint8 ch22data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

static const BitmapCharDesc ch22 = {4, 13, 0, 2, 8, ch22data};

/* char: 0x15 */

static const uint8 ch21data[] = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0xf8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch21 = {5, 13, -3, 2, 8, ch21data};

/* char: 0x14 */

static const uint8 ch20data[] = {
    0xff,
};

static const BitmapCharDesc ch20 = {8, 1, 0, 1, 8, ch20data};

/* char: 0x13 */

static const uint8 ch19data[] = {
    0xff,
};

static const BitmapCharDesc ch19 = {8, 1, 0, -1, 8, ch19data};

/* char: 0x12 */

static const uint8 ch18data[] = {
    0xff,
};

static const BitmapCharDesc ch18 = {8, 1, 0, -3, 8, ch18data};

/* char: 0x11 */

static const uint8 ch17data[] = {
    0xff,
};

static const BitmapCharDesc ch17 = {8, 1, 0, -5, 8, ch17data};

/* char: 0x10 */

static const uint8 ch16data[] = {
    0xff,
};

static const BitmapCharDesc ch16 = {8, 1, 0, -7, 8, ch16data};

/* char: 0xf */

static const uint8 ch15data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

static const BitmapCharDesc ch15 = {8, 13, 0, 2, 8, ch15data};

/* char: 0xe */

static const uint8 ch14data[] = {
    0xf8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch14 = {5, 8, -3, -3, 8, ch14data};

/* char: 0xd */

static const uint8 ch13data[] = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0xf8,
};

static const BitmapCharDesc ch13 = {5, 6, -3, 2, 8, ch13data};

/* char: 0xc */

static const uint8 ch12data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0xf0,
};

static const BitmapCharDesc ch12 = {4, 6, 0, 2, 8, ch12data};

/* char: 0xb */

static const uint8 ch11data[] = {
    0xf0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

static const BitmapCharDesc ch11 = {4, 8, 0, -3, 8, ch11data};

/* char: 0xa */

static const uint8 ch10data[] = {
    0x8, 0x8, 0x8, 0x8, 0x3e, 0x20, 0x50, 0x88, 0x88,
};

static const BitmapCharDesc ch10 = {7, 9, 0, 2, 8, ch10data};

/* char: 0x9 */

static const uint8 ch9data[] = {
    0x3e, 0x20, 0x20, 0x20, 0x88, 0x98, 0xa8, 0xc8, 0x88,
};

static const BitmapCharDesc ch9 = {7, 9, 0, 2, 8, ch9data};

/* char: 0x8 */

static const uint8 ch8data[] = {
    0xfe, 0x10, 0x10, 0xfe, 0x10, 0x10,
};

static const BitmapCharDesc ch8 = {7, 6, 0, 0, 8, ch8data};

/* char: 0x7 */

static const uint8 ch7data[] = {
    0x70,
    0x88,
    0x88,
    0x70,
};

static const BitmapCharDesc ch7 = {5, 4, -1, -5, 8, ch7data};

/* char: 0x6 */

static const uint8 ch6data[] = {
    0x20, 0x20, 0x3c, 0x20, 0x3e, 0xf8, 0x80, 0x80, 0x80,
};

static const BitmapCharDesc ch6 = {7, 9, 0, 2, 8, ch6data};

/* char: 0x5 */

static const uint8 ch5data[] = {
    0x22, 0x22, 0x3c, 0x22, 0x3c, 0x78, 0x80, 0x80, 0x78,
};

static const BitmapCharDesc ch5 = {7, 9, 0, 2, 8, ch5data};

/* char: 0x4 */

static const uint8 ch4data[] = {
    0x10, 0x10, 0x1c, 0x10, 0x9e, 0x80, 0xe0, 0x80, 0xf0,
};

static const BitmapCharDesc ch4 = {7, 9, 0, 2, 8, ch4data};

/* char: 0x3 */

static const uint8 ch3data[] = {
    0x8, 0x8, 0x8, 0x3e, 0x88, 0x88, 0xf8, 0x88, 0x88,
};

static const BitmapCharDesc ch3 = {7, 9, 0, 2, 8, ch3data};

/* char: 0x2 */

static const uint8 ch2data[] = {
    0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa,
};

static const BitmapCharDesc ch2 = {8, 12, 0, 2, 8, ch2data};

/* char: 0x1 */

static const uint8 ch1data[] = {
    0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10,
};

static const BitmapCharDesc ch1 = {7, 7, 0, -1, 8, ch1data};

/* char NULL */
static const BitmapCharDesc & chNull = ch20;

const BitmapCharDesc * const gBitmapChars8x13[] = {
    &ch0,    &ch1,    &ch2,    &ch3,    &ch4,    &ch5,    &ch6,    &ch7,    &ch8,    &ch9,    &ch10,   &ch11,   &ch12,   &ch13,   &ch14,   &ch15,
    &ch16,   &ch17,   &ch18,   &ch19,   &ch20,   &ch21,   &ch22,   &ch23,   &ch24,   &ch25,   &ch26,   &ch27,   &ch28,   &ch29,   &ch30,   &ch31,
    &ch32,   &ch33,   &ch34,   &ch35,   &ch36,   &ch37,   &ch38,   &ch39,   &ch40,   &ch41,   &ch42,   &ch43,   &ch44,   &ch45,   &ch46,   &ch47,
    &ch48,   &ch49,   &ch50,   &ch51,   &ch52,   &ch53,   &ch54,   &ch55,   &ch56,   &ch57,   &ch58,   &ch59,   &ch60,   &ch61,   &ch62,   &ch63,
    &ch64,   &ch65,   &ch66,   &ch67,   &ch68,   &ch69,   &ch70,   &ch71,   &ch72,   &ch73,   &ch74,   &ch75,   &ch76,   &ch77,   &ch78,   &ch79,
    &ch80,   &ch81,   &ch82,   &ch83,   &ch84,   &ch85,   &ch86,   &ch87,   &ch88,   &ch89,   &ch90,   &ch91,   &ch92,   &ch93,   &ch94,   &ch95,
    &ch96,   &ch97,   &ch98,   &ch99,   &ch100,  &ch101,  &ch102,  &ch103,  &ch104,  &ch105,  &ch106,  &ch107,  &ch108,  &ch109,  &ch110,  &ch111,
    &ch112,  &ch113,  &ch114,  &ch115,  &ch116,  &ch117,  &ch118,  &ch119,  &ch120,  &ch121,  &ch122,  &ch123,  &ch124,  &ch125,  &ch126,  &ch127,
    &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull,
    &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull, &chNull,
    &ch160,  &ch161,  &ch162,  &ch163,  &ch164,  &ch165,  &ch166,  &ch167,  &ch168,  &ch169,  &ch170,  &ch171,  &ch172,  &ch173,  &ch174,  &ch175,
    &ch176,  &ch177,  &ch178,  &ch179,  &ch180,  &ch181,  &ch182,  &ch183,  &ch184,  &ch185,  &ch186,  &ch187,  &ch188,  &ch189,  &ch190,  &ch191,
    &ch192,  &ch193,  &ch194,  &ch195,  &ch196,  &ch197,  &ch198,  &ch199,  &ch200,  &ch201,  &ch202,  &ch203,  &ch204,  &ch205,  &ch206,  &ch207,
    &ch208,  &ch209,  &ch210,  &ch211,  &ch212,  &ch213,  &ch214,  &ch215,  &ch216,  &ch217,  &ch218,  &ch219,  &ch220,  &ch221,  &ch222,  &ch223,
    &ch224,  &ch225,  &ch226,  &ch227,  &ch228,  &ch229,  &ch230,  &ch231,  &ch232,  &ch233,  &ch234,  &ch235,  &ch236,  &ch237,  &ch238,  &ch239,
    &ch240,  &ch241,  &ch242,  &ch243,  &ch244,  &ch245,  &ch246,  &ch247,  &ch248,  &ch249,  &ch250,  &ch251,  &ch252,  &ch253,  &ch254,  &ch255,
};
