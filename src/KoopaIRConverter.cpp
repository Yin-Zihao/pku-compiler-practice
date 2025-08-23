#include "KoopaIRConverter.h"
#include <cassert>
#include <iostream>

koopa_raw_program_t KoopaIRConverter::ConvertToRawProgram(const std::string& ir_str) {
    // 1. 解析文本IR
    koopa_program_t program;
    koopa_error_code_t ret = koopa_parse_from_string(ir_str.c_str(), &program);
    CheckErrorCode(ret);

    // 2. 创建builder并转换为raw program
    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    koopa_raw_program_t raw_program = koopa_build_raw_program(builder, program);

    // 3. 清理临时资源
    koopa_delete_program(program);
    koopa_delete_raw_program_builder(builder);

    return raw_program;
}

void KoopaIRConverter::CheckErrorCode(koopa_error_code_t error_code) {
    if (error_code != KOOPA_EC_SUCCESS) {
        std::cerr << "Koopa IR conversion error: " << error_code << std::endl;
        assert(false && "Koopa IR conversion failed");
    }
}