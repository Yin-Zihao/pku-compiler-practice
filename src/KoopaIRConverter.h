#pragma once
#include <string>
#include "koopa.h"  // 引用koopa库头文件

class KoopaIRConverter {
public:
    // 将文本形式IR转换为raw program
    static koopa_raw_program_t ConvertToRawProgram(const std::string& ir_str);
    
private:
    // 错误处理辅助函数
    static void CheckErrorCode(koopa_error_code_t error_code);
};