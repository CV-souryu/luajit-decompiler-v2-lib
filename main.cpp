#include "main.h"
struct Assertion {
	const std::string message;
	const std::string filePath;
	const std::string function;
	const std::string source;
	const std::string line;
};

//static const HANDLE CONSOLE_OUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
//static const HANDLE CONSOLE_INPUT = GetStdHandle(STD_INPUT_HANDLE);
static bool isCommandLine;
static bool isProgressBarActive = false;
static uint32_t filesSkipped = 0;

static struct {
	bool showHelp = false;
	bool silentAssertions = false;
	bool forceOverwrite = false;
	bool ignoreDebugInfo = false;
	bool minimizeDiffs = false;
	bool unrestrictedAscii = false;
	std::string inputPath;
	std::string outputPath;
	std::string extensionFilter;
} arguments;

struct Directory {
	const std::string path;
	std::vector<Directory> folders;
	std::vector<std::string> files;
};

static std::string string_to_lowercase(const std::string& string) {
	std::string lowercaseString = string;

	for (uint32_t i = lowercaseString.size(); i--;) {
		if (lowercaseString[i] < 'A' || lowercaseString[i] > 'Z') continue;
		lowercaseString[i] += 'a' - 'A';
	}

	return lowercaseString;
}

//static void find_files_recursively(Directory& directory) {
//	WIN32_FIND_DATAA pathData;
//	HANDLE handle = FindFirstFileA((arguments.inputPath + directory.path + '*').c_str(), &pathData);
//	if (handle == INVALID_HANDLE_VALUE) return;
//
//	do {
//		if (pathData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
//			if (!std::strcmp(pathData.cFileName, ".") || !std::strcmp(pathData.cFileName, "..")) continue;
//			directory.folders.emplace_back(Directory{ .path = directory.path + pathData.cFileName + "\\" });
//			find_files_recursively(directory.folders.back());
//			if (!directory.folders.back().files.size() && !directory.folders.back().folders.size()) directory.folders.pop_back();
//			continue;
//		}
//
//		if (!arguments.extensionFilter.size() || arguments.extensionFilter == string_to_lowercase(PathFindExtensionA(pathData.cFileName))) directory.files.emplace_back(pathData.cFileName);
//	} while (FindNextFileA(handle, &pathData));
//
//	FindClose(handle);
//}
//
//static bool decompile_files_recursively(const Directory& directory) {
//	CreateDirectoryA((arguments.outputPath + directory.path).c_str(), NULL);
//	std::string outputFile;
//
//	for (uint32_t i = 0; i < directory.files.size(); i++) {
//		outputFile = directory.files[i];
//		PathRemoveExtensionA(outputFile.data());
//		outputFile = outputFile.c_str();
//		outputFile += ".lua";
//
//		Bytecode bytecode(arguments.inputPath + directory.path + directory.files[i]);
//		Ast ast(bytecode, arguments.ignoreDebugInfo, arguments.minimizeDiffs);
//		Lua lua(bytecode, ast, arguments.outputPath + directory.path + outputFile, arguments.forceOverwrite, arguments.minimizeDiffs, arguments.unrestrictedAscii);
//
//		try {
//			print("--------------------\nInput file: " + bytecode.filePath + "\nReading bytecode...");
//			bytecode();
//			print("Building ast...");
//			ast();
//			print("Writing lua source...");
//			lua();
//			print("Output file: " + lua.filePath);
//		}
//		catch (const Assertion& assertion) {
//			erase_progress_bar();
//
//			if (arguments.silentAssertions) {
//				print("\nError running " + assertion.function + "\nSource: " + assertion.source + ":" + assertion.line + "\n\n" + assertion.message);
//				filesSkipped++;
//				continue;
//			}
//
//			switch (MessageBoxA(NULL, ("Error running " + assertion.function + "\nSource: " + assertion.source + ":" + assertion.line + "\n\nFile: " + assertion.filePath + "\n\n" + assertion.message).c_str(),
//				PROGRAM_NAME, MB_ICONERROR | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3)) {
//			case IDCANCEL:
//				return false;
//			case IDTRYAGAIN:
//				print("Retrying...");
//				i--;
//				continue;
//			case IDCONTINUE:
//				print("File skipped.");
//				filesSkipped++;
//			}
//		}
//		catch (...) {
//			MessageBoxA(NULL, std::string("Unknown exception\n\nFile: " + bytecode.filePath).c_str(), PROGRAM_NAME, MB_ICONERROR | MB_OK);
//			throw;
//		}
//	}
//
//	for (uint32_t i = 0; i < directory.folders.size(); i++) {
//		if (!decompile_files_recursively(directory.folders[i])) return false;
//	}
//
//	return true;
//}
//
//static char* parse_arguments(const int& argc, char** const& argv) {
//	if (argc < 2) return nullptr;
//	arguments.inputPath = argv[1];
//#ifndef _DEBUG
//	if (!isCommandLine) return nullptr;
//#endif
//	bool isInputPathSet = true;
//
//	if (arguments.inputPath.size() && arguments.inputPath.front() == '-') {
//		arguments.inputPath.clear();
//		isInputPathSet = false;
//	}
//
//	std::string argument;
//
//	for (uint32_t i = isInputPathSet ? 2 : 1; i < argc; i++) {
//		argument = argv[i];
//
//		if (argument.size() >= 2 || argument.front() == '-') {
//			if (argument[1] == '-') {
//				argument = argument.c_str() + 2;
//
//				if (argument == "extension") {
//					if (i <= argc - 2) {
//						i++;
//						arguments.extensionFilter = argv[i];
//						continue;
//					}
//				}
//				else if (argument == "force_overwrite") {
//					arguments.forceOverwrite = true;
//					continue;
//				}
//				else if (argument == "help") {
//					arguments.showHelp = true;
//					continue;
//				}
//				else if (argument == "ignore_debug_info") {
//					arguments.ignoreDebugInfo = true;
//					continue;
//				}
//				else if (argument == "minimize_diffs") {
//					arguments.minimizeDiffs = true;
//				}
//				else if (argument == "output") {
//					if (i <= argc - 2) {
//						i++;
//						arguments.outputPath = argv[i];
//						continue;
//					}
//				}
//				else if (argument == "silent_assertions") {
//					arguments.silentAssertions = true;
//					continue;
//				}
//				else if (argument == "unrestricted_ascii") {
//					arguments.unrestrictedAscii = true;
//					continue;
//				}
//			}
//			else if (argument.size() == 2) {
//				switch (argument[1]) {
//				case 'e':
//					if (i > argc - 2) break;
//					i++;
//					arguments.extensionFilter = argv[i];
//					continue;
//				case 'f':
//					arguments.forceOverwrite = true;
//					continue;
//				case '?':
//				case 'h':
//					arguments.showHelp = true;
//					continue;
//				case 'i':
//					arguments.ignoreDebugInfo = true;
//					continue;
//				case 'm':
//					arguments.minimizeDiffs = true;
//					continue;
//				case 'o':
//					if (i > argc - 2) break;
//					i++;
//					arguments.outputPath = argv[i];
//					continue;
//				case 's':
//					arguments.silentAssertions = true;
//					continue;
//				case 'u':
//					arguments.unrestrictedAscii = true;
//					continue;
//				}
//			}
//		}
//
//		return argv[i];
//	}
//
//	return nullptr;
//}

//static void wait_for_exit() {
//	if (isCommandLine) return;
//	print("Press enter to exit.");
//	input();
//}


//void print(const std::string& message) {
//	WriteConsoleA(CONSOLE_OUTPUT, (message + '\n').data(), message.size() + 1, NULL, NULL);
//}

//std::string input() {
//	static char BUFFER[1024];
//
//	FlushConsoleInputBuffer(CONSOLE_INPUT);
//	DWORD charsRead;
//	return ReadConsoleA(CONSOLE_INPUT, BUFFER, sizeof(BUFFER), &charsRead, NULL) && charsRead > 2 ? std::string(BUFFER, charsRead - 2) : "";
//}

//void print_progress_bar(const double& progress, const double& total) {
//	static char PROGRESS_BAR[] = "\r[====================]";
//
//	const uint8_t threshold = std::round(20 / total * progress);
//
//	for (uint8_t i = 20; i--;) {
//		PROGRESS_BAR[i + 2] = i < threshold ? '=' : ' ';
//	}
//
//	WriteConsoleA(CONSOLE_OUTPUT, PROGRESS_BAR, sizeof(PROGRESS_BAR) - 1, NULL, NULL);
//	isProgressBarActive = true;
//}
//
//void erase_progress_bar() {
//	static constexpr char PROGRESS_BAR_ERASER[] = "\r                      \r";
//
//	if (!isProgressBarActive) return;
//	WriteConsoleA(CONSOLE_OUTPUT, PROGRESS_BAR_ERASER, sizeof(PROGRESS_BAR_ERASER) - 1, NULL, NULL);
//	isProgressBarActive = false;
//}

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


bool EX(const char* x, const char* y)
{

	std::string inputPath(x);   // 进入 C++ 世界后再用
	std::string outputPath(y);
	Bytecode bytecode(inputPath);
	Ast ast(bytecode, false, false);
	Lua lua(bytecode, ast, outputPath, false, false, false);

	try {
		//print("--------------------\nInput file: " + bytecode.filePath + "\nReading bytecode...");
		bytecode();
		//print("Building ast...");
		ast();
		//print("Writing lua source...");
		lua();
		//print("Output file: " + lua.filePath);
		return true;

	}
	catch (const Assertion& assertion) {
		//print("\nError running " + assertion.function + "\nSource: " + assertion.source + ":" + assertion.line + "\n\n" + assertion.message);
		//erase_progress_bar();

		//if (arguments.silentAssertions) {
		//	print("\nError running " + assertion.function + "\nSource: " + assertion.source + ":" + assertion.line + "\n\n" + assertion.message);
		//	//filesSkipped++;
		//	return false;
		//}
		return false;

		/*switch (MessageBoxA(NULL, ("Error running " + assertion.function + "\nSource: " + assertion.source + ":" + assertion.line + "\n\nFile: " + assertion.filePath + "\n\n" + assertion.message).c_str(),
			PROGRAM_NAME, MB_ICONERROR | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3)) {
		case IDCANCEL:
			return false;
		case IDTRYAGAIN:
			print("Retrying...");
			return false;
		case IDCONTINUE:
			print("File skipped.");
			return false;
		}*/
	}
	catch (...) {

		//return false;

		//MessageBoxA(NULL, std::string("Unknown exception\n\nFile: " + bytecode.filePath).c_str(), PROGRAM_NAME, MB_ICONERROR | MB_OK);
		throw;
	}

	// 示例：把两个路径简单打印并写个日志
	//std::string in(x);
	//std::string out(y);

	//std::ofstream log("log.txt", std::ios::app);
	//log << "EX called: " << in << " -> " << out << '\n';
	// 这里继续你的业务
}