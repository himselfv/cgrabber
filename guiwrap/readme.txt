This folder demostrates another approach: mark the core app as /SUBSYSTEM:CONSOLE and write a thin GUI wrapper which starts it with DETACHED_PROCESS.

This might be the easiest and most reliable approach in all regards.

1. No flickering console window in GUI mode (DETACHED_PROCESS does not create one).

2. There's a clear way to tell whether the core is running as a console:
  GetConsoleWindow() != NULL

3. If running in a console mode, the core is truly a console app => no troubles with CRT misbehaving in "pseudo-console" environments.

4. Tiniest possible wrapper (simply start the process).

The downside is that you need exclusive cooperation from the core app. Other methods often allow the core to be more or less oblivious of the exact way wrapper is implemented. Here you have to make a choice to switch to console base.