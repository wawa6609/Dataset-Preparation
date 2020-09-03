#include "FileNotFoundException.h"
FileNotFoundException::FileNotFoundException(const string& msg, int err_num, int err_off) :
    runtime_error(msg)
{
    error_number = err_num;
    error_offset = err_off;

}