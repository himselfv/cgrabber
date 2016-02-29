This is CGrabber, Windows console wrapper for a GUI app ImgBrd Grabber.


ImgrBrd Grabber has command-line functionality but when compiled for GUI, it functions differently from what you'd expect of a console app:

* It cannot write to or read from console (even if it runs in invisible mode)

* When you start it from the command line, the interpreter doesn't wait for it to finish.

* You cannot check the returned error code.


CGrabber fixes these shortcomings. You can use it exactly like a command-line app. The command line is passed to Grabber.exe, so the syntax is the same (you can omit the "-c").