#ifndef MYIO_H
#define MYIO_H

#include <stdio.h>
#include <stdarg.h>

/* 错误码定义 */
typedef enum {
    IO_SUCCESS = 0,
    IO_ERROR_OPEN_FAIL,
    IO_ERROR_READ_FAIL,
    IO_ERROR_WRITE_FAIL,
    IO_ERROR_CLOSE_FAIL,
    IO_ERROR_SEEK_FAIL,
    IO_ERROR_MEMORY_FAIL
} IOErrorCode;

/* IO句柄结构体（隐藏实现） */
typedef struct IOContext IOContext;

/* 文件打开模式 */
typedef enum {
    IO_MODE_READ = 1,
    IO_MODE_WRITE = 2,
    IO_MODE_APPEND = 4,
    IO_MODE_BINARY = 8
} IOMode;

/* 文件定位方式 */
typedef enum {
    IO_SEEK_SET = 0,
    IO_SEEK_CUR = 1,
    IO_SEEK_END = 2
} IOSeekOrigin;

/* 库初始化/清理 */
int io_init(void);
void io_cleanup(void);

/* 文件操作 */
IOContext* io_open(const char *filename, int mode);
int io_close(IOContext *ctx);

/* 读写操作 */
size_t io_read(IOContext *ctx, void *buffer, size_t size);
size_t io_write(IOContext *ctx, const void *buffer, size_t size);

/* 格式化读写 */
int io_printf(IOContext *ctx, const char *format, ...);
int io_scanf(IOContext *ctx, const char *format, ...);

/* 定位操作 */
int io_seek(IOContext *ctx, long offset, IOSeekOrigin origin);
long io_tell(IOContext *ctx);
int io_eof(IOContext *ctx);
void io_rewind(IOContext *ctx);

/* 错误处理 */
int io_error(IOContext *ctx);
const char* io_error_string(int error_code);
void io_clear_error(IOContext *ctx);

/* 工具函数 */
size_t io_get_file_size(IOContext *ctx);
int io_copy_file(const char *src, const char *dest);
int io_file_exists(const char *filename);

#endif