random stuff
============

*harness.c* - A harness for testing shellcode. Takes shellcode in a file
(or statically in the compiled executable), allocates some RWX mem for
it, and runs it.

*fork-and-run-payload.c* - Find the directory of the currently running
binary and execute an adjacent file. Useful for macOS app bundles where
you don't always know where you are executing from.

*setargv0.c* - Example code for modifying argv0.


