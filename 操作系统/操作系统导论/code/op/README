
*** VIRTUALIZATION
cpu.c
- shows the wonders of CPU virtualization
- compile with "gcc -o cpu cpu.c -Wall -Werror"

mem.c (or mem-stack.c or ...)
- same, but for memory
  to get it to work, must turn off address-space randomization
  "make-mem.csh" does this for recent Mac OS X variants

*** CONCURRENCY
threads.v0.c
threads.v1.c 
- show simple shared counter, locks
- if you want to look at the disassembly, use "otool" on macs 
  or "objdump" on linux
  see "disasm-threads.csh" for the mac command
  on recent macs, had to compile very carefully to get an
  easy instruction sequence to examine
  (see make-threads.csh)
- add flag -pthread to compile

*** PERSISTENCE
io.c
- run 'dtrace -s trace-io.d' in another window
  ("trace-io.csh" can do this for you)
  this shows how complex the file system is ...

















