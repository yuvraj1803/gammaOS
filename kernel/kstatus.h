/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __KSTATUS_H__
#define __KSTATUS_H__

enum ERRORS{

    SUCCESS = 0,
    ERR_INVARG,
    ERR_MALLOC_FAIL,
    ERR_INVPATH,
    ERR_TASK_FAIL,
    ERR_FS_FAIL,
    ERR_FS_INV_SIGN,
    ERR_FS_ROOT_NOT_FOUND,
    ERR_FS_INV_FILE_MODE,
    ERR_FS_FOPEN_FAILED,
    ERR_FD_NOT_FOUND,
    ERR_MAX_PROCESS_LIMIT_REACHED,
    ERR_PROCESS_LOAD_FAIL,
    ERR_PROCESS_CREATION_FAILED

};

#endif