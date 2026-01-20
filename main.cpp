#include "main.h"

struct Assertion {
	const std::string message;
	const std::string filePath;
	const std::string function;
	const std::string source;
	const std::string line;
};

void assert(const bool& assertion, const std::string& message, const std::string& filePath, const std::string& function, const std::string& source, const uint32_t& line) {
	if (!assertion) throw Assertion{
		.message = message,
		.filePath = filePath,
		.function = function,
		.source = source,
		.line = std::to_string(line)
	};
}

std::string byte_to_string(const uint8_t& byte) {
	char string[] = "0x00";
	uint8_t digit;

	for (uint8_t i = 2; i--;) {
		digit = (byte >> i * 4) & 0xF;
		string[3 - i] = digit >= 0xA ? 'A' + digit - 0xA : '0' + digit;
	}

	return string;
}

/*
 EX - 简化导出接口（反编译入口）

 参数:
   x - C-string，输入字节码文件路径
   y - C-string，输出 Lua 源文件路径

 返回:
   true  - 反编译成功并写入输出
   false - 捕获到断言错误（Assertion），操作未成功完成

 异常:
   其他未知异常会重新抛出，调用方应负责捕获和处理
 */
bool EX(const char* x, const char* y) {
	std::string inputPath(x);
	std::string outputPath(y);

	// 构建反编译所需对象（仅构造，实际逻辑在下面调用）
	Bytecode bytecode(inputPath);
	Ast ast(bytecode, false, false);
	Lua lua(bytecode, ast, outputPath, false, false, false);

	try {
		// 执行反编译流程：读取字节码 -> 构建 AST -> 写出 Lua 源
		bytecode();
		ast();
		lua();
		return true;
	}
	catch (const Assertion& assertion) {
		// 断言失败：不抛出异常，返回 false。
		// 若需要详细错误信息，可在调用方扩展日志或修改此处以记录 assertion 内容。
		return false;
	}
	catch (...) {
		// 未知异常：保持原有行为，重新抛出以便上层处理
		throw;
	}
}

// End of file