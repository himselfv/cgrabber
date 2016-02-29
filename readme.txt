Windows console wrapper for a GUI app ImgBrd Grabber.


This repo is also an exploration into having "opportunistic" console/GUI apps:
https://blogs.msdn.microsoft.com/oldnewthing/20090101-00/?p=19643

That is, apps that behave like GUI or console app depending on the way you run it.
See: http://www.boku.ru/2016/02/28/posting-to-console-from-gui-app/


CGrabber: an example of such a solution, Windows console wrapper for ImgBrd Grabber.

conapp, dconapp, dguiapp: example applications which read/write to console, written in C and Delphi (to test CGrabber).


guiwrap: another approach to a console/GUI chameleon app (very reliable and slim).


On buffering in stdout: See buffering.txt.

On AttachConsole problems with C/C++: See comments in conapp.cpp.

