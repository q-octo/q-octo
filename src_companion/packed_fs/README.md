# Packed File System

Converts files under `/files` into hex dumps and stores them in unsigned char
arrays.

1. Add files to `/files`
2. Run `make` from the project root. This will generate `include/packed_fs.h
3. Include `packed_fs.h` and use `mg_unpack()` to get a char array.

Example:
The two pointers are optional (can be replaced with nullptr).
```cpp
size_t htmlSize;
time_t lastModified;
const char* html = mg_unpack("/index.html", &htmlSize, &lastModified);
```