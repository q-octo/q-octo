#include <stddef.h>
#include <string.h>
#include <time.h>

#if defined(__cplusplus)
extern "C" {
#endif
const char *mg_unlist(size_t no);
const char *mg_unpack(const char *, size_t *, time_t *);
#if defined(__cplusplus)
}
#endif

static const unsigned char v1[] = {
  60, 104, 116, 109, 108,  62,  10,  10,  60, 104, 101,  97, // <html>..<hea
 100,  62,  10,  32,  32,  32,  32,  60, 115,  99, 114, 105, // d>.    <scri
 112, 116,  62, 118,  97, 114,  32,  99, 111, 110, 110, 101, // pt>var conne
  99, 116, 105, 111, 110,  32,  61,  32, 110, 101, 119,  32, // ction = new 
  87, 101,  98,  83, 111,  99, 107, 101, 116,  40,  39, 119, // WebSocket('w
 115,  58,  47,  47, 114, 111, 118, 101, 114,  46, 108, 111, // s://rover.lo
  99,  97, 108,  58,  56,  49,  47,  39,  44,  32,  91,  39, // cal:81/', ['
  97, 114, 100, 117, 105, 110, 111,  39,  93,  41,  59,  32, // arduino']); 
  99, 111, 110, 110, 101,  99, 116, 105, 111, 110,  46, 111, // connection.o
 110, 111, 112, 101, 110,  32,  61,  32, 102, 117, 110,  99, // nopen = func
 116, 105, 111, 110,  32,  40,  41,  32, 123,  32,  99, 111, // tion () { co
 110, 110, 101,  99, 116, 105, 111, 110,  46, 115, 101, 110, // nnection.sen
 100,  40,  39,  67, 111, 110, 110, 101,  99, 116,  32,  39, // d('Connect '
  32,  43,  32, 110, 101, 119,  32,  68,  97, 116, 101,  40, //  + new Date(
  41,  41,  59,  32, 125,  59,  32,  99, 111, 110, 110, 101, // )); }; conne
  99, 116, 105, 111, 110,  46, 111, 110, 101, 114, 114, 111, // ction.onerro
 114,  32,  61,  32, 102, 117, 110,  99, 116, 105, 111, 110, // r = function
  32,  40, 101, 114, 114, 111, 114,  41,  32, 123,  32,  99, //  (error) { c
 111, 110, 115, 111, 108, 101,  46, 108, 111, 103,  40,  39, // onsole.log('
  87, 101,  98,  83, 111,  99, 107, 101, 116,  32,  69, 114, // WebSocket Er
 114, 111, 114,  32,  39,  44,  32, 101, 114, 114, 111, 114, // ror ', error
  41,  59,  32, 125,  59,  32,  99, 111, 110, 110, 101,  99, // ); }; connec
 116, 105, 111, 110,  46, 111, 110, 109, 101, 115, 115,  97, // tion.onmessa
 103, 101,  32,  61,  32, 102, 117, 110,  99, 116, 105, 111, // ge = functio
 110,  32,  40, 101,  41,  32, 123,  32,  99, 111, 110, 115, // n (e) { cons
 111, 108, 101,  46, 108, 111, 103,  40,  39,  83, 101, 114, // ole.log('Ser
 118, 101, 114,  58,  32,  39,  44,  32, 101,  46, 100,  97, // ver: ', e.da
 116,  97,  41,  59,  32, 125,  59,  32, 102, 117, 110,  99, // ta); }; func
 116, 105, 111, 110,  32, 115, 101, 110, 100,  82,  71,  66, // tion sendRGB
  40,  41,  32, 123,  32, 118,  97, 114,  32, 114,  32,  61, // () { var r =
  32, 112,  97, 114, 115, 101,  73, 110, 116,  40, 100, 111, //  parseInt(do
  99, 117, 109, 101, 110, 116,  46, 103, 101, 116,  69, 108, // cument.getEl
 101, 109, 101, 110, 116,  66, 121,  73, 100,  40,  39, 114, // ementById('r
  39,  41,  46, 118,  97, 108, 117, 101,  41,  46, 116, 111, // ').value).to
  83, 116, 114, 105, 110, 103,  40,  49,  54,  41,  59,  32, // String(16); 
 118,  97, 114,  32, 103,  32,  61,  32, 112,  97, 114, 115, // var g = pars
 101,  73, 110, 116,  40, 100, 111,  99, 117, 109, 101, 110, // eInt(documen
 116,  46, 103, 101, 116,  69, 108, 101, 109, 101, 110, 116, // t.getElement
  66, 121,  73, 100,  40,  39, 103,  39,  41,  46, 118,  97, // ById('g').va
 108, 117, 101,  41,  46, 116, 111,  83, 116, 114, 105, 110, // lue).toStrin
 103,  40,  49,  54,  41,  59,  32, 118,  97, 114,  32,  98, // g(16); var b
  32,  61,  32, 112,  97, 114, 115, 101,  73, 110, 116,  40, //  = parseInt(
 100, 111,  99, 117, 109, 101, 110, 116,  46, 103, 101, 116, // document.get
  69, 108, 101, 109, 101, 110, 116,  66, 121,  73, 100,  40, // ElementById(
  39,  98,  39,  41,  46, 118,  97, 108, 117, 101,  41,  46, // 'b').value).
 116, 111,  83, 116, 114, 105, 110, 103,  40,  49,  54,  41, // toString(16)
  59,  32, 105, 102,  32,  40, 114,  46, 108, 101, 110, 103, // ; if (r.leng
 116, 104,  32,  60,  32,  50,  41,  32, 123,  32, 114,  32, // th < 2) { r 
  61,  32,  39,  48,  39,  32,  43,  32, 114,  59,  32, 125, // = '0' + r; }
  32, 105, 102,  32,  40, 103,  46, 108, 101, 110, 103, 116, //  if (g.lengt
 104,  32,  60,  32,  50,  41,  32, 123,  32, 103,  32,  61, // h < 2) { g =
  32,  39,  48,  39,  32,  43,  32, 103,  59,  32, 125,  32, //  '0' + g; } 
 105, 102,  32,  40,  98,  46, 108, 101, 110, 103, 116, 104, // if (b.length
  32,  60,  32,  50,  41,  32, 123,  32,  98,  32,  61,  32, //  < 2) { b = 
  39,  48,  39,  32,  43,  32,  98,  59,  32, 125,  32, 118, // '0' + b; } v
  97, 114,  32, 114, 103,  98,  32,  61,  32,  39,  35,  39, // ar rgb = '#'
  32,  43,  32, 114,  32,  43,  32, 103,  32,  43,  32,  98, //  + r + g + b
  59,  32,  99, 111, 110, 115, 111, 108, 101,  46, 108, 111, // ; console.lo
 103,  40,  39,  82,  71,  66,  58,  32,  39,  32,  43,  32, // g('RGB: ' + 
 114, 103,  98,  41,  59,  32,  99, 111, 110, 110, 101,  99, // rgb); connec
 116, 105, 111, 110,  46, 115, 101, 110, 100,  40, 114, 103, // tion.send(rg
  98,  41,  59,  32, 125,  60,  47, 115,  99, 114, 105, 112, // b); }</scrip
 116,  62,  10,  60,  47, 104, 101,  97, 100,  62,  10,  10, // t>.</head>..
  60,  98, 111, 100, 121,  62,  76,  69,  68,  32,  67, 111, // <body>LED Co
 110, 116, 114, 111, 108,  58,  60,  98, 114,  32,  47,  62, // ntrol:<br />
  60,  98, 114,  32,  47,  62,  82,  58,  32,  60, 105, 110, // <br />R: <in
 112, 117, 116,  32, 105, 100,  61,  34, 114,  34,  32, 116, // put id="r" t
 121, 112, 101,  61,  34, 114,  97, 110, 103, 101,  34,  32, // ype="range" 
 109, 105, 110,  61,  34,  48,  34,  32, 109,  97, 120,  61, // min="0" max=
  34,  50,  53,  53,  34,  32, 115, 116, 101, 112,  61,  34, // "255" step="
  49,  34,  32, 111, 110, 105, 110, 112, 117, 116,  61,  34, // 1" oninput="
 115, 101, 110, 100,  82,  71,  66,  40,  41,  59,  34,  32, // sendRGB();" 
  47,  62,  60,  98, 114,  32,  47,  62,  71,  58,  10,  32, // /><br />G:. 
  32,  32,  32,  60, 105, 110, 112, 117, 116,  32, 105, 100, //    <input id
  61,  34, 103,  34,  32, 116, 121, 112, 101,  61,  34, 114, // ="g" type="r
  97, 110, 103, 101,  34,  32, 109, 105, 110,  61,  34,  48, // ange" min="0
  34,  32, 109,  97, 120,  61,  34,  50,  53,  53,  34,  32, // " max="255" 
 115, 116, 101, 112,  61,  34,  49,  34,  32, 111, 110, 105, // step="1" oni
 110, 112, 117, 116,  61,  34, 115, 101, 110, 100,  82,  71, // nput="sendRG
  66,  40,  41,  59,  34,  32,  47,  62,  60,  98, 114,  32, // B();" /><br 
  47,  62,  66,  58,  32,  60, 105, 110, 112, 117, 116,  32, // />B: <input 
 105, 100,  61,  34,  98,  34,  32, 116, 121, 112, 101,  61, // id="b" type=
  34, 114,  97, 110, 103, 101,  34,  10,  32,  32,  32,  32, // "range".    
  32,  32,  32,  32, 109, 105, 110,  61,  34,  48,  34,  32, //     min="0" 
 109,  97, 120,  61,  34,  50,  53,  53,  34,  32, 115, 116, // max="255" st
 101, 112,  61,  34,  49,  34,  32, 111, 110, 105, 110, 112, // ep="1" oninp
 117, 116,  61,  34, 115, 101, 110, 100,  82,  71,  66,  40, // ut="sendRGB(
  41,  59,  34,  32,  47,  62,  60,  98, 114,  32,  47,  62, // );" /><br />
  60,  47,  98, 111, 100, 121,  62,  10,  10,  60,  47, 104, // </body>..</h
 116, 109, 108,  62, 0 // tml>
};
static const unsigned char v2[] = {
  60, 104, 116, 109, 108,  62,  10,  10,  60, 104, 101,  97, // <html>..<hea
 100,  62,  10,  32,  32,  32,  32,  60, 115,  99, 114, 105, // d>.    <scri
 112, 116,  62, 118,  97, 114,  32,  99, 111, 110, 110, 101, // pt>var conne
  99, 116, 105, 111, 110,  32,  61,  32, 110, 101, 119,  32, // ction = new 
  87, 101,  98,  83, 111,  99, 107, 101, 116,  40,  39, 119, // WebSocket('w
 115,  58,  47,  47, 114, 111, 118, 101, 114,  46, 108, 111, // s://rover.lo
  99,  97, 108,  58,  56,  49,  47,  39,  44,  32,  91,  39, // cal:81/', ['
  97, 114, 100, 117, 105, 110, 111,  39,  93,  41,  59,  32, // arduino']); 
  99, 111, 110, 110, 101,  99, 116, 105, 111, 110,  46, 111, // connection.o
 110, 111, 112, 101, 110,  32,  61,  32, 102, 117, 110,  99, // nopen = func
 116, 105, 111, 110,  32,  40,  41,  32, 123,  32,  99, 111, // tion () { co
 110, 110, 101,  99, 116, 105, 111, 110,  46, 115, 101, 110, // nnection.sen
 100,  40,  39,  67, 111, 110, 110, 101,  99, 116,  32,  39, // d('Connect '
  32,  43,  32, 110, 101, 119,  32,  68,  97, 116, 101,  40, //  + new Date(
  41,  41,  59,  32, 125,  59,  32,  99, 111, 110, 110, 101, // )); }; conne
  99, 116, 105, 111, 110,  46, 111, 110, 101, 114, 114, 111, // ction.onerro
 114,  32,  61,  32, 102, 117, 110,  99, 116, 105, 111, 110, // r = function
  32,  40, 101, 114, 114, 111, 114,  41,  32, 123,  32,  99, //  (error) { c
 111, 110, 115, 111, 108, 101,  46, 108, 111, 103,  40,  39, // onsole.log('
  87, 101,  98,  83, 111,  99, 107, 101, 116,  32,  69, 114, // WebSocket Er
 114, 111, 114,  32,  39,  44,  32, 101, 114, 114, 111, 114, // ror ', error
  41,  59,  32, 125,  59,  32,  99, 111, 110, 110, 101,  99, // ); }; connec
 116, 105, 111, 110,  46, 111, 110, 109, 101, 115, 115,  97, // tion.onmessa
 103, 101,  32,  61,  32, 102, 117, 110,  99, 116, 105, 111, // ge = functio
 110,  32,  40, 101,  41,  32, 123,  32,  99, 111, 110, 115, // n (e) { cons
 111, 108, 101,  46, 108, 111, 103,  40,  39,  83, 101, 114, // ole.log('Ser
 118, 101, 114,  58,  32,  39,  44,  32, 101,  46, 100,  97, // ver: ', e.da
 116,  97,  41,  59,  32, 125,  59,  32, 102, 117, 110,  99, // ta); }; func
 116, 105, 111, 110,  32, 115, 101, 110, 100,  82,  71,  66, // tion sendRGB
  40,  41,  32, 123,  32, 118,  97, 114,  32, 114,  32,  61, // () { var r =
  32, 112,  97, 114, 115, 101,  73, 110, 116,  40, 100, 111, //  parseInt(do
  99, 117, 109, 101, 110, 116,  46, 103, 101, 116,  69, 108, // cument.getEl
 101, 109, 101, 110, 116,  66, 121,  73, 100,  40,  39, 114, // ementById('r
  39,  41,  46, 118,  97, 108, 117, 101,  41,  46, 116, 111, // ').value).to
  83, 116, 114, 105, 110, 103,  40,  49,  54,  41,  59,  32, // String(16); 
 118,  97, 114,  32, 103,  32,  61,  32, 112,  97, 114, 115, // var g = pars
 101,  73, 110, 116,  40, 100, 111,  99, 117, 109, 101, 110, // eInt(documen
 116,  46, 103, 101, 116,  69, 108, 101, 109, 101, 110, 116, // t.getElement
  66, 121,  73, 100,  40,  39, 103,  39,  41,  46, 118,  97, // ById('g').va
 108, 117, 101,  41,  46, 116, 111,  83, 116, 114, 105, 110, // lue).toStrin
 103,  40,  49,  54,  41,  59,  32, 118,  97, 114,  32,  98, // g(16); var b
  32,  61,  32, 112,  97, 114, 115, 101,  73, 110, 116,  40, //  = parseInt(
 100, 111,  99, 117, 109, 101, 110, 116,  46, 103, 101, 116, // document.get
  69, 108, 101, 109, 101, 110, 116,  66, 121,  73, 100,  40, // ElementById(
  39,  98,  39,  41,  46, 118,  97, 108, 117, 101,  41,  46, // 'b').value).
 116, 111,  83, 116, 114, 105, 110, 103,  40,  49,  54,  41, // toString(16)
  59,  32, 105, 102,  32,  40, 114,  46, 108, 101, 110, 103, // ; if (r.leng
 116, 104,  32,  60,  32,  50,  41,  32, 123,  32, 114,  32, // th < 2) { r 
  61,  32,  39,  48,  39,  32,  43,  32, 114,  59,  32, 125, // = '0' + r; }
  32, 105, 102,  32,  40, 103,  46, 108, 101, 110, 103, 116, //  if (g.lengt
 104,  32,  60,  32,  50,  41,  32, 123,  32, 103,  32,  61, // h < 2) { g =
  32,  39,  48,  39,  32,  43,  32, 103,  59,  32, 125,  32, //  '0' + g; } 
 105, 102,  32,  40,  98,  46, 108, 101, 110, 103, 116, 104, // if (b.length
  32,  60,  32,  50,  41,  32, 123,  32,  98,  32,  61,  32, //  < 2) { b = 
  39,  48,  39,  32,  43,  32,  98,  59,  32, 125,  32, 118, // '0' + b; } v
  97, 114,  32, 114, 103,  98,  32,  61,  32,  39,  35,  39, // ar rgb = '#'
  32,  43,  32, 114,  32,  43,  32, 103,  32,  43,  32,  98, //  + r + g + b
  59,  32,  99, 111, 110, 115, 111, 108, 101,  46, 108, 111, // ; console.lo
 103,  40,  39,  82,  71,  66,  58,  32,  39,  32,  43,  32, // g('RGB: ' + 
 114, 103,  98,  41,  59,  32,  99, 111, 110, 110, 101,  99, // rgb); connec
 116, 105, 111, 110,  46, 115, 101, 110, 100,  40, 114, 103, // tion.send(rg
  98,  41,  59,  32, 125,  60,  47, 115,  99, 114, 105, 112, // b); }</scrip
 116,  62,  10,  60,  47, 104, 101,  97, 100,  62,  10,  10, // t>.</head>..
  60,  98, 111, 100, 121,  62,  76,  69,  68,  32,  67, 111, // <body>LED Co
 110, 116, 114, 111, 108,  58,  60,  98, 114,  32,  47,  62, // ntrol:<br />
  60,  98, 114,  32,  47,  62,  82,  58,  32,  60, 105, 110, // <br />R: <in
 112, 117, 116,  32, 105, 100,  61,  34, 114,  34,  32, 116, // put id="r" t
 121, 112, 101,  61,  34, 114,  97, 110, 103, 101,  34,  32, // ype="range" 
 109, 105, 110,  61,  34,  48,  34,  32, 109,  97, 120,  61, // min="0" max=
  34,  50,  53,  53,  34,  32, 115, 116, 101, 112,  61,  34, // "255" step="
  49,  34,  32, 111, 110, 105, 110, 112, 117, 116,  61,  34, // 1" oninput="
 115, 101, 110, 100,  82,  71,  66,  40,  41,  59,  34,  32, // sendRGB();" 
  47,  62,  60,  98, 114,  32,  47,  62,  71,  58,  10,  32, // /><br />G:. 
  32,  32,  32,  60, 105, 110, 112, 117, 116,  32, 105, 100, //    <input id
  61,  34, 103,  34,  32, 116, 121, 112, 101,  61,  34, 114, // ="g" type="r
  97, 110, 103, 101,  34,  32, 109, 105, 110,  61,  34,  48, // ange" min="0
  34,  32, 109,  97, 120,  61,  34,  50,  53,  53,  34,  32, // " max="255" 
 115, 116, 101, 112,  61,  34,  49,  34,  32, 111, 110, 105, // step="1" oni
 110, 112, 117, 116,  61,  34, 115, 101, 110, 100,  82,  71, // nput="sendRG
  66,  40,  41,  59,  34,  32,  47,  62,  60,  98, 114,  32, // B();" /><br 
  47,  62,  66,  58,  32,  60, 105, 110, 112, 117, 116,  32, // />B: <input 
 105, 100,  61,  34,  98,  34,  32, 116, 121, 112, 101,  61, // id="b" type=
  34, 114,  97, 110, 103, 101,  34,  10,  32,  32,  32,  32, // "range".    
  32,  32,  32,  32, 109, 105, 110,  61,  34,  48,  34,  32, //     min="0" 
 109,  97, 120,  61,  34,  50,  53,  53,  34,  32, 115, 116, // max="255" st
 101, 112,  61,  34,  49,  34,  32, 111, 110, 105, 110, 112, // ep="1" oninp
 117, 116,  61,  34, 115, 101, 110, 100,  82,  71,  66,  40, // ut="sendRGB(
  41,  59,  34,  32,  47,  62,  60,  98, 114,  32,  47,  62, // );" /><br />
  60,  47,  98, 111, 100, 121,  62,  10,  10,  60,  47, 104, // </body>..</h
 116, 109, 108,  62, 0 // tml>
};

static const struct packed_file {
  const char *name;
  const unsigned char *data;
  size_t size;
  time_t mtime;
} packed_files[] = {
  {"/index copy.html", v1, sizeof(v1), 1712242203},
  {"/index.html", v2, sizeof(v2), 1712237145},
  {NULL, NULL, 0, 0}
};

static int scmp(const char *a, const char *b) {
  while (*a && (*a == *b)) a++, b++;
  return *(const unsigned char *) a - *(const unsigned char *) b;
}
const char *mg_unlist(size_t no) {
  return packed_files[no].name;
}
const char *mg_unpack(const char *name, size_t *size, time_t *mtime) {
  const struct packed_file *p;
  for (p = packed_files; p->name != NULL; p++) {
    if (scmp(p->name, name) != 0) continue;
    if (size != NULL) *size = p->size - 1;
    if (mtime != NULL) *mtime = p->mtime;
    return (const char *) p->data;
  }
  return NULL;
}