I'm currently working through the excellent OpenGL tutorial series at http://ogldev.atspace.co.uk and making my solutions available just incase they happen to be helpful to anybody else. I'm trying to reference the author's provided source as little as possible, so my solutions wont match theirs very closely.

Each example builds directly on and/or refactors the previous. Each example is also self-contained for ease of project setup, and thus duplicates a bunch of files.

Dependencies:
- SDL2
- cmake
- OpenGL 3.3+ core profile support

To build any of the samples run the following in any of the sample directories:

```
mkdir build && cd build
cmake ..
make
```
