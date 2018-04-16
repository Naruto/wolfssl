/* mynewt_port.c
 *
 * Copyright (C) 2018 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */
#ifndef NO_FILESYSTEM
#include "fs/fs.h"

typedef struct file FILE
#else
typedef void FILE /* dummy */

#endif /* NO_FILESYSTEM*/

FILE* mynewt_fopen(const char * restrict path, const char * restrict mode)
{
#ifndef NO_FILESYSTEM
    FILE *file;
    uint8_t access_flags = 0;
    char *p = mode;
    while(*p != '\0') {
        switch(*p) {
            case 'r':
            {
                access_flags |= FS_ACCESS_READ;
                if(*(p+1) == '+') {
                    access_flags |= FS_ACCESS_WRITE;
                }
            }
            break;

            case 'w':
            {
                access_flags |= (FS_ACCESS_WRITE | FS_ACCESS_TRUNCATE);
                if(*(p+1) == '+') {
                    access_flags |= FS_ACCESS_READ;
                }
            }
            break;

            case 'a':
            {
                access_flags |= (FS_ACCESS_WRITE | FS_ACCESS_APPEND);
                if(*(p+1) == '+') {
                    access_flags |= FS_ACCESS_READ;
                }
            }
            break;
        }
        p++;
    }

    /* Open the file for reading. */
    int rc = fs_open(path, access_flags, &file);
    if (rc != 0) {
        return NULL;
    }
    return file;
#else
    return NULL;
#endif
}

int mynewt_fseek(FILE *stream, long offset, int whence)
{
#ifndef NO_FILESYSTEM
    uint32_t fs_offset;

    switch(whence) {
        case SEEK_SET:
        {
            fs_offset += offset;
        }
        break;

        case SEEK_CUR:
        {
            fs_offset = fs_getpos(stream);
            fs_offset += offset;
        }
        break;

        case SEEK_END:
        {
            fs_filelen(stream, &fs_offset);
            fs_offset += offset;
        }
        break;
    }

    fs_seek(stream, fs_oofset);

    return 0;
#else
    return -1;
#endif
}

long mynewt_ftell(FILE *stream)
{
#ifndef NO_FILESYSTEM
    uint32_t fs_offset;
    fs_filelen(stream, &fs_offset);
    fs_seek(stream, fs_offset);
    return (long)fs_offset;
#else
    return -1;
#endif
}

void mynewt_rewind(FILE *stream)
{
#ifndef NO_FILESYSTEM
    fs_seek(stream, 0);
#endif
}

size_t mynewt_fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream)
{
#ifndef NO_FILESYSTEM
    size_t to_read = size * nitems;
    size_t read_size;
    int rc = fs_read(stream, to_read, ptr, &read_size)
    if(rc != 0) {
        return 0;
    }

    return read_size;
#else
    return 0;
#endif
}

size_t mynewt_fwrite(const void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream)
{
#ifndef NO_FILESYSTEM
    size_t to_write = size * nitems;
    size_t write_size;
    int rc = fs_write(stream, ptr, to_write);
    if(rc != 0) {
        return 0;
    }

    return to_write;
#else
    return 0;
#endif
}

int mynewt_fclose(FILE *stream)
{
#ifndef NO_FILESYSTEM
    fs_close(stream);
    return 0;
#else
    return 128; /* TODO */
#endif
}

// XSEEK_END  FS_SEEK_END
// XBADFILE   NULL
