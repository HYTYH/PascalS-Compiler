# PascalS-Compiler

A Pascal-S Compiler

This project is based on [Flex](http://dinosaur.compilertools.net/flex/index.html), [Bison](https://www.gnu.org/software/bison/) and [llvm](https://llvm.org/), go through those links for more details.

---
## ✔Getting Started

### Guidance on Installation

#### Ⅰ、Environmental requirements

- linux 64
- suggestion：Ubuntu 16.04 64bit
- the llvm and clang toolchain required for the program will be installed automatically via script

#### Ⅱ、Scrip description

1. the script will automatically check if there are executable files in the current folder
2. the script will automatically check if the g++ llvm toolchain exists in the /usr/bin/ folder
3. if the g++ llvm clang toolchain does not exist, the script will automatically execute the installation
4. finally the script copies the pascal executable into usr/bin/
5. after installation, you can use pascal -help to see how to use it directly

#### Ⅲ、Installation process

1. get the application installer file [here](https://github.com/HYTYH/PascalS-Compiler/releases)

2. go to the pascals_compiler_installer directory

```
cd pascals_compiler_installer
```

3. run the installer.sh script - make the script have execute permission

```
chmod +x ./installer.sh
```

- execute the script file

```
./installer.sh
```

#### Ⅳ、Use instruction

1. use the pascal or pascal -help command to display help
2. compile with pascal [options] filename

```
Options:
-T              Output AST Construct to stdout
-I              Output IR Code to stdout
-O              Output OPT IR Code to stdout
-S              Output Assembly Code to stdout
-A              Output ALL to stdout
```

---
## 👀Notice
- Pascal-S Production

Complete Pascal-S productions are listed in [here](https://github.com/HYTYH/PascalS-Compiler/blob/main/docs/Pascal-S%20Production.md).

- Detailed Design Document

See [Pascal-S_doc](https://github.com/HYTYH/PascalS-Compiler/blob/main/docs/Pascal-S_doc.pdf) for detailed informations.

---

## ✉Contact Us

Feel free to open an issue if you have any questions about our project.
