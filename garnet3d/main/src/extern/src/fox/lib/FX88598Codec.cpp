#include "xincs.h"
#include "fxver.h"
#include "fxdefs.h"
#include "FXHash.h"
#include "FXStream.h"
#include "FXTextCodec.h"
#include "FX88598Codec.h"

namespace FX {

FXIMPLEMENT(FX88598Codec,FXTextCodec,NULL,0)


//// Created by codec tool on 03/25/2005 from: 8859-8.TXT ////
const unsigned short forward_data[256]={
   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,   15,
   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
   96,   97,   98,   99,   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
   160,  65533, 162,  163,  164,  165,  166,  167,  168,  169,  215,  171,  172,  173,  174,  175,
   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  247,  187,  188,  189,  190,  65533,
   65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533,
   65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 65533, 8215,
   1488, 1489, 1490, 1491, 1492, 1493, 1494, 1495, 1496, 1497, 1498, 1499, 1500, 1501, 1502, 1503,
   1504, 1505, 1506, 1507, 1508, 1509, 1510, 1511, 1512, 1513, 1514, 65533, 65533, 8206, 8207, 65533,
  };


const unsigned char reverse_plane[17]={
  0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  };

const unsigned char reverse_pages[73]={
  0,  35, 66, 66, 66, 66, 66, 66, 130,66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66,
  };

const unsigned short reverse_block[194]={
  0,   16,  32,  48,  64,  80,  96,  112, 128, 144, 160, 176, 191, 200, 191, 209,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  225, 241, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 252, 268, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
  191, 191,
  };

const unsigned char reverse_data[284]={
   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,   15,
   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
   96,   97,   98,   99,   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
   160,  26,   162,  163,  164,  165,  166,  167,  168,  169,  26,   171,  172,  173,  174,  175,
   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  26,   187,  188,  189,  190,  26,
   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   170,
   26,   26,   26,   26,   26,   26,   26,   26,   186,  26,   26,   26,   26,   26,   26,   26,
   26,   224,  225,  226,  227,  228,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,
   239,  240,  241,  242,  243,  244,  245,  246,  247,  248,  249,  250,  26,   26,   26,   26,
   26,   26,   26,   26,   26,   26,   26,   26,   26,   26,   253,  254,  26,   26,   26,   26,
   26,   26,   26,   223,  26,   26,   26,   26,   26,   26,   26,   26,
  };


FXint FX88598Codec::mb2wc(FXwchar& wc,const FXchar* src,FXint nsrc) const {
  if(nsrc<1) return -1;
  wc=forward_data[(FXuchar)src[0]];
  return 1;
  }


FXint FX88598Codec::wc2mb(FXchar* dst,FXint ndst,FXwchar wc) const {
  if(ndst<1) return -1;
  dst[0]=reverse_data[reverse_block[reverse_pages[reverse_plane[wc>>16]+((wc>>10)&63)]+((wc>>4)&63)]+(wc&15)];
  return 1;
  }

FXint FX88598Codec::mibEnum() const {
  return 11;
  }


const FXchar* FX88598Codec::name() const {
  return "ISO-8859-8";
  }


const FXchar* FX88598Codec::mimeName() const {
  return "ISO-8859-8";
  }


const FXchar* const* FX88598Codec::aliases() const {
  static const FXchar *const list[]={"iso8859-8","ISO-8859-8","ISO_8859-8","hebrew","iso-ir-138","csISOLatinHebrew",NULL};
  return list;
  }

}

