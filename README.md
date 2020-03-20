<p align="center"><img width=256 height=256 src=resources/pokem.png></p>  

# PókeM CLI
*Command line tool to encode, decode and convert mails from Mystery Dungeon games.*

**Note:** Only *Red & Blue* games are currently supported.

-----------------------------------------------------------------------------------------------------------------------------------

## Usage
This is a Command Line Interface (CLI) tool. Just open it in the terminal.  
Windows users can simply open the executable directly.  

## Building from sources

### Requirements:
- Make tool
- C Compiler
- **PókeM**, which can be found [here](github.com/lateo96/pokem) and must be (both library and header) in the `lib` directory. For your convenience, a local (maybe not the latest) copy of **PokéM** can be found in `src/3rdparty` directory, and will be used to build the application.

### Build:
Go to the root of the project and type in the terminal:  

```sh
$ make
```

### Note for Windows users prior to Windows 10 RS2:
**PokéM CLI** can be built with or without color support. Since it uses [*ASCII escape sequences*](https://en.wikipedia.org/wiki/ANSI_escape_code) to represent colors, opening in a terminal without such support will cause a buggy output. Such feature is supported on Windows since **Windows 10 RS2**, and so, users with an earlier version must use the version without colors. To build **PokéM CLI** without color support you must type:  

```sh
$ make all-without-colors
```

-----------------------------------------------------------------------------------------------------------------------------------

***Contact info:*** [*thecrowporation@gmail.com*](mailto:thecrowporation@gmail.com)