# cpp_utils
Small utility functions and classes for c++. MIT license applies. Items marked with /*- require that one and only one source file has #define QUASAR_IMPLEMENTATION before including the header file so as to enable definition rather than just declaration. This is done this way so as to allow for non-template non-inline header-only libraries.
The current list includes the following items:

URI.h: URI parser which breaks down a string passed to the constructor into gradually more granular parts. The constructor assumes the passed string is valid according to the syntax diagram in the Wikipedia page for URI.
