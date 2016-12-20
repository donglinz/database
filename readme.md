This is a simple program that allows people to read and write FoxPro files with the .bdf file extension.
This is NOT a complete implementation, it was done without ANY official specifications and was done by reverse engineering a file with a hex editor to a large degree.  It currently supports numbers,integers, floats, doubles, and character strings and logicals.  It will treat all unhandled data types as if they are character fields.

I used the QtCreator development tool to build this project, but it is not dependent on Qt, it is just plain ansi c++, so any compiler should work fine.
I only used the QtCreator because I prefer it as my c++ IDE.
The purpose of the project is not to provide a compiled binary, but a c++ and h file to include in your own projects.

see http://www.ostafichuk.com for more information

