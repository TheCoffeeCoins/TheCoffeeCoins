thecoffeecoins Core
=============

Setup
---------------------
thecoffeecoins Core is the original thecoffeecoins client and it builds the backbone of the network. It downloads and, by default, stores the entire history of thecoffeecoins transactions, which requires a few hundred gigabytes of disk space. Depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

To download thecoffeecoins Core, visit [thecoffeecoinscore.org](https://thecoffeecoinscore.org/en/download/).

Running
---------------------
The following are some helpful notes on how to run thecoffeecoins Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/thecoffeecoins-qt` (GUI) or
- `bin/thecoffeecoinsd` (headless)

### Windows

Unpack the files into a directory, and then run thecoffeecoins-qt.exe.

### macOS

Drag thecoffeecoins Core to your applications folder, and then run thecoffeecoins Core.

### Need Help?

* See the documentation at the [thecoffeecoins Wiki](https://en.thecoffeecoins.it/wiki/Main_Page)
for help and more information.
* Ask for help on [#thecoffeecoins](https://webchat.freenode.net/#thecoffeecoins) on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#thecoffeecoins).
* Ask for help on the [thecoffeecoinsTalk](https://thecoffeecoinstalk.org/) forums, in the [Technical Support board](https://thecoffeecoinstalk.org/index.php?board=4.0).

Building
---------------------
The following are developer notes on how to build thecoffeecoins Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [FreeBSD Build Notes](build-freebsd.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)
- [Android Build Notes](build-android.md)
- [Gitian Building Guide (External Link)](https://github.com/thecoffeecoins-core/docs/blob/master/gitian-building.md)

Development
---------------------
The thecoffeecoins repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://doxygen.thecoffeecoinscore.org/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [thecoffeecoinsTalk](https://thecoffeecoinstalk.org/) forums, in the [Development & Technical Discussion board](https://thecoffeecoinstalk.org/index.php?board=6.0).
* Discuss project-specific development on #thecoffeecoins-core-dev on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#thecoffeecoins-core-dev).
* Discuss general thecoffeecoins development on #thecoffeecoins-dev on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#thecoffeecoins-dev).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [thecoffeecoins.conf Configuration File](thecoffeecoins-conf.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Memory](reduce-memory.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)
- [PSBT support](psbt.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
