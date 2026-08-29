#include "myio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* 私有上下文结构 */
struct IOContext {
    FILE *file;
    int error_code;
    int mode;
    char *buffer;       /* 用户缓冲区 */
    size_t buffer_size;
    size_t position;    /* 当前位置 */
    int eof_flag;
};

/* 错误消息映射 */
static const char *error_messages[] = {
    "Success",
    "Failed to open file",
    "Failed to read from file",
    "Failed to write to file",
    "Failed to close file",
    "Failed to seek in file",
    "Memory allocation failed"
};

/* 库初始化 */
int io_init(void) {
    /* 如果有全局资源，在这里初始化 */
    return IO_SUCCESS;
}

void io_cleanup(void) {
    /* 清理全局资源 */
}

/* 打开文件 */
IOContext* io_open(const char *filename, int mode) {
    IOContext *ctx = NULL;
    const char *mode_str = NULL;
    
    /* 分配上下文 */
    ctx = (IOContext*)calloc(1, sizeof(IOContext));
    if (!ctx) {
        return NULL;
    }
    
    /* 转换模式 */
    if (mode & IO_MODE_READ && mode & IO_MODE_WRITE) {
        mode_str = (mode & IO_MODE_BINARY) ? "w+b" : "w+";
    } else if (mode & IO_MODE_READ) {
        mode_str = (mode & IO_MODE_BINARY) ? "rb" : "r";
    } else if (mode & IO_MODE_WRITE) {
        mode_str = (mode & IO_MODE_BINARY) ? "wb" : "w";
    } else if (mode & IO_MODE_APPEND) {
        mode_str = (mode & IO_MODE_BINARY) ? "ab" : "a";
    } else {
        free(ctx);
        return NULL;
    }
    
    /* 打开文件 */
    ctx->file = fopen(filename, mode_str);
    if (!ctx->file) {
        ctx->error_code = IO_ERROR_OPEN_FAIL;
        free(ctx);
        return NULL;
    }
    
    ctx->mode = mode;
    ctx->eof_flag = 0;
    
    return ctx;
}

/* 关闭文件 */
int io_close(IOContext *ctx) {
    if (!ctx) return IO_ERROR_CLOSE_FAIL;
    
    if (ctx->file) {
        if (fclose(ctx->file) != 0) {
            ctx->error_code = IO_ERROR_CLOSE_FAIL;
            return IO_ERROR_CLOSE_FAIL;
        }
    }
    
    if (ctx->buffer) {
        free(ctx->buffer);
    }
    
    free(ctx);
    return IO_SUCCESS;
}

/* 读取文件 */
size_t io_read(IOContext *ctx, void *buffer, size_t size) {
    if (!ctx || !ctx->file || !buffer) {
        if (ctx) ctx->error_code = IO_ERROR_READ_FAIL;
        return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, size, ctx->file);
    if (bytes_read < size && ferror(ctx->file)) {
        ctx->error_code = IO_ERROR_READ_FAIL;
    }
    
    if (feof(ctx->file)) {
        ctx->eof_flag = 1;
    }
    
    return bytes_read;
}

/* 写入文件 */
size_t io_write(IOContext *ctx, const void *buffer, size_t size) {
    if (!ctx || !ctx->file || !buffer) {
        if (ctx) ctx->error_code = IO_ERROR_WRITE_FAIL;
        return 0;
    }
    
    size_t bytes_written = fwrite(buffer, 1, size, ctx->file);
    if (bytes_written < size) {
        ctx->error_code = IO_ERROR_WRITE_FAIL;
    }
    
    return bytes_written;
}

/* 格式化输出 */
int io_printf(IOContext *ctx, const char *format, ...) {
    va_list args;
    int result;
    
    if (!ctx || !ctx->file || !format) {
        if (ctx) ctx->error_code = IO_ERROR_WRITE_FAIL;
        return -1;
    }
    
    va_start(args, format);
    result = vfprintf(ctx->file, format, args);
    va_end(args);
    
    if (result < 0) {
        ctx->error_code = IO_ERROR_WRITE_FAIL;
    }
    
    return result;
}

/* 格式化输入 */
int io_scanf(IOContext *ctx, const char *format, ...) {
    va_list args;
    int result;
    
    if (!ctx || !ctx->file || !format) {
        if (ctx) ctx->error_code = IO_ERROR_READ_FAIL;
        return -1;
    }
    
    va_start(args, format);
    result = vfscanf(ctx->file, format, args);
    va_end(args);
    
    if (result == EOF) {
        ctx->eof_flag = 1;
        if (ferror(ctx->file)) {
            ctx->error_code = IO_ERROR_READ_FAIL;
        }
    }
    
    return result;
}

/* 定位操作 */
int io_seek(IOContext *ctx, long offset, IOSeekOrigin origin) {
    int whence;
    
    if (!ctx || !ctx->file) {
        return IO_ERROR_SEEK_FAIL;
    }
    
    switch (origin) {
        case IO_SEEK_SET: whence = SEEK_SET; break;
        case IO_SEEK_CUR: whence = SEEK_CUR; break;
        case IO_SEEK_END: whence = SEEK_END; break;
        default: return IO_ERROR_SEEK_FAIL;
    }
    
    if (fseek(ctx->file, offset, whence) != 0) {
        ctx->error_code = IO_ERROR_SEEK_FAIL;
        return IO_ERROR_SEEK_FAIL;
    }
    
    ctx->eof_flag = 0;
    return IO_SUCCESS;
}

/* 获取当前位置 */
long io_tell(IOContext *ctx) {
    if (!ctx || !ctx->file) return -1;
    return ftell(ctx->file);
}

/* 判断文件是否结束 */
int io_eof(IOContext *ctx) {
    if (!ctx) return 1;
    return ctx->eof_flag || feof(ctx->file);
}

/* 重置到文件开头 */
void io_rewind(IOContext *ctx) {
    if (ctx && ctx->file) {
        rewind(ctx->file);
        ctx->eof_flag = 0;
    }
}

/* 获取错误码 */
int io_error(IOContext *ctx) {
    if (!ctx) return -1;
    return ctx->error_code;
}

/* 获取错误信息 */
const char* io_error_string(int error_code) {
    if (error_code < 0 || error_code >= sizeof(error_messages)/sizeof(error_messages[0])) {
        return "Unknown error";
    }
    return error_messages[error_code];
}

/* 清除错误状态 */
void io_clear_error(IOContext *ctx) {
    if (ctx) {
        ctx->error_code = IO_SUCCESS;
        clearerr(ctx->file);
    }
}

/* 获取文件大小 */
size_t io_get_file_size(IOContext *ctx) {
    long current_pos;
    long file_size;
    
    if (!ctx || !ctx->file) return 0;
    
    current_pos = ftell(ctx->file);
    if (current_pos < 0) return 0;
    
    if (fseek(ctx->file, 0, SEEK_END) != 0) return 0;
    
    file_size = ftell(ctx->file);
    if (file_size < 0) {
        fseek(ctx->file, current_pos, SEEK_SET);
        return 0;
    }
    
    fseek(ctx->file, current_pos, SEEK_SET);
    return (size_t)file_size;
}

/* 复制文件 */
int io_copy_file(const char *src, const char *dest) {
    IOContext *src_ctx = NULL;
    IOContext *dest_ctx = NULL;
    char buffer[4096];
    size_t bytes_read;
    int result = IO_SUCCESS;
    
    src_ctx = io_open(src, IO_MODE_READ | IO_MODE_BINARY);
    if (!src_ctx) {
        return IO_ERROR_OPEN_FAIL;
    }
    
    dest_ctx = io_open(dest, IO_MODE_WRITE | IO_MODE_BINARY);
    if (!dest_ctx) {
        result = IO_ERROR_OPEN_FAIL;
        goto cleanup;
    }
    
    while ((bytes_read = io_read(src_ctx, buffer, sizeof(buffer))) > 0) {
        if (io_write(dest_ctx, buffer, bytes_read) != bytes_read) {
            result = IO_ERROR_WRITE_FAIL;
            goto cleanup;
        }
    }
    
    if (io_error(src_ctx) != IO_SUCCESS) {
        result = io_error(src_ctx);
    }

cleanup:
    if (src_ctx) io_close(src_ctx);
    if (dest_ctx) io_close(dest_ctx);
    
    return result;
}

/* 检查文件是否存在 */
int io_file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}