TypeCase
========

This project was originally created during my PhD to analyse data 
collected with the COSY-TOF detector located at Juelich, Germany.
It was meant for physicists to be able to analyze their data collected
at particle accellerators. Myself I'm a physicist as well with a slight 
(back then...) background in programming and computer science.

The major part of the design was finished in 2005, about 13 years 
back now. The last changes made to the code were done before the first
of the modern revisions of the C++ standard (C++-11) was published, so
the code published for my thesis was not modern C++.

And I'm experienced enough now to know that the project is far from 
perfect. I guess everybody is a bit embarrassed if confronted with code 
he produced during his naive youth. 

During the last year I've seen some customers' code-bases that have 
potential for improvement (If I might be so bold...).

I cannot publish a customer's code-base as an example of how one can 
improve a C++ project, when using a sensible build-system and a sensible
amount of design pattern. But I can publish my old typeCase project and
step by step improve (refactor) the project to show what is possible.

Lots of code will just be removed, because it doesn't make sense in a 
modern code-base, like the documentation module. It will be exchanged 
by doxygen. I'm still searching for a replacement for the geometry module,
I think other people already have made excellent libraries for volumes
and areas in 3D-space, so why not make use of them? I'll add a unit-testing
suite (most probably using gtest), the testing code I had seems to be lost.

The first commit is exactly the state that I copied it in 2011. On the 
second commit I have it compile, not skillfully though, with gcc-4.8.
Compile but not link. Using makefiles is just a pain, because you have to
make sure you list the libraries in the correct order.

That's why I introduced cmake with the third commit. Also the makefiles are
removed with this step. Now the command-line applications compile and link.
But the UI-applications aren't added to the compilation yet. The reason
is sad but simple: I'm using Qt5 to compile, but the UI still needs Qt3.
I think, I'll leave that until I rework the UI (which I will most probably
do with qml).

Try it, if you like. Prerequisites are:

* gcc-4.8 (newer versions will lead to compiler errors; yes, the compilers
have become more strict)
* boost program_options
* Qt 5
* root (root.cern.ch)
