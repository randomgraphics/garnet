Files under this directory are public external libraries needed by Garnet.

Subdirectory structure should be like:

- bin
  - <platform>
    - <build>
- inc
  - <platform>
    - <build>
- lib
  - <platform>
    - <build>

<platform> : common, win32, amd64, ia64, cygwin, linux, ....
<build>    : common, debug, release, stdbg, strel
