========
 Burgos
========

This is Burgos, the prototype distributed ftp indexer
and search engine.

This software is not maintained any more. For a glimpse of the history
of the project and its design goals, see the postmortem
section. Development used to happen at, https://launchpad.net/burgos .

This software is governed by the CeCILL license. You can find the full
license in the file COPYING (or COPYING.fr for the french version).

Postmortem
==========

The goal of Burgos was to be a distributed ftp indexer and search
engine, meaning was many computers would collectively index the ftp
servers on the network without relying on a central server. The
distributed aspect was a legal necessity in the context of a network
of ftp servers containing not-very-legal content so that no one would
have to bear the liability of hosting an indexer of all that illegal
content. In that context, a swarm of Burgos clients were to work
collaboratively to maintain an up-to-date index of servers and share
it on the network without DDoSing the servers.

At some point, the legal requirements changed, and a centralized
alternative, much simpler to implement but much less interesting, was
developed.


Technical details
-----------------

The whole implementation is in C++/Qt.

Technically, the indexer was pretty straightforward: it would just
crawl a ftp server and represent it as a tree. The biggest mistake was
to represent that tree in the program using DOM, which made it very
memory-hungry (I was young and nobody told me it was a bad idea at the
time).

The peers would index out-of-date servers from time to time and
broadcast the index on the network as xml. This last part was never
implemented, so we couldn't tackle the interesting problems of
synchronization to avoid indexing the same server from more than one
client.

The search engine part was pretty simple, being just filtering on a
`QTreeView`, making it nice-to-use, but not very efficient.


Install
=======

First of all, you will need Qt in order to compile it. You can find Qt at this address : http://qt-project.org/ .

To compile Burgos, execute ::

  qmake
  make

If you just want to build burgos-cli, replace the latter by ::

  make sub-src-burgos-cli-pro


Language files
--------------

Linux
~~~~~

If you're under Linux, use the scripts `utils/lupdate.sh` and
`utils/lrelease.sh` to update the language files and create the binary
language files (used by Burgos).

Windows ~~~~~~~ In order to update the language files, we first need
to make a dummy project file containing all source files. For that,
type ::

  qmake -project -o ts.pro

then you can type ::

  lupdate ts.pro

and ::

  lrelease ts.pro

Don't forget to remove ts.pro in order to clean.
