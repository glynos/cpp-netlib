Motivation
==========

Modern applications that communicate over the internet have never been
more prevalent, ranging through diverse areas such as high performance
servers to embedded systems for smart phones and navigation systems.

Currently, there are no open source network libraries available that
use modern object-oriented techniques in C++.  Any developer will
understand the familiar problem of searching for a protocol library
for their project, failing to find anything suitable and too often
having to hand-roll their own.

By leveraging Boost_, and in particular `Boost.Asio`_, developers can
create portable network C++ applications much more easily.  What is
still lacking is a set of libraries that utilise `Boost.Asio`_ in
order to provide application level support so that C++ developers are
able to develop internet and distributed applications more
effectively.  This is the niche that the developers of the
:mod:`cpp-netlib` see their project filling.

.. _Boost: http://www.boost.org/
.. _`Boost.Asio`: http://www.boost.org/libs/asio/
