## LuaJIT Decompiler v2

*LuaJIT Decompiler v2* is a replacement tool for the old and now mostly defunct python decompiler.  
The project fixes all of the bugs and quirks the python decompiler had while also offering  
full support for gotos and stripped bytecode including locals and upvalues.

## Usage

1. Head to the release section and download the latest executable.
2. Drag and drop a valid LuaJIT bytecode file or a folder containing such files onto the exe.  
Alternatively, run the program in a command prompt. Use `-?` to show usage and options.
3. All successfully decompiled `.lua` files are placed by default into the `output` folder  
located in the same directory as the exe.

Feel free to [report any issues](https://github.com/marsinator358/luajit-decompiler-v2/issues/new) you have.

## Building on macOS ✨

You can build a macOS dynamic library (.dylib) with the bundled CMake build script:

```bash
./build.sh
# Output will be installed to ./dist, the dylib will be in ./dist/lib
```

Notes:
- The CMake setup sets `INSTALL_NAME_DIR` to `@rpath` for macOS so the library is relocatable.
- If you need a specific architecture, pass `-DCMAKE_OSX_ARCHITECTURES="x86_64"` (or `arm64`) to CMake.

macOS build fixes (2026-01-20):
- Replaced Windows-style `..\\main.h` includes with POSIX `../main.h` to support macOS toolchains.
- Fixed nested-type declarations that caused compiler errors on clang (removed redundant `Ast::` / `Bytecode::` qualifiers inside headers included within the class body).

If you had previous build issues on macOS, pull the latest changes and run `./build.sh` again.

## TODO

* bytecode big endian support
* improved decompilation logic for conditional assignments

---

This project uses an boolean expression decompilation algorithm that is based on this paper:  
[www.cse.iitd.ac.in/~sak/reports/isec2016-paper.pdf](https://www.cse.iitd.ac.in/~sak/reports/isec2016-paper.pdf)
