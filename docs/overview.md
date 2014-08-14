Beginner's Guide
====

Motivation
----

Add motivation here.

* stemmed from [giga](https://github.com/cripplet/giga.git)
* practice in good interface design
* lack of C++ caching frameworks

Installation
----

Get a copy of the `cachepp` framework library, including all dependencies.

```bash
git clone https://github.com/cripplet/cachepp.git
cd cachepp
git submodule update --init --recursive
make test
```

Updating
----

Updating the framework and all dependencies.

```bash
git pull
git submodule foreach --recursive git checkout master
git submodule foreach --recursive git pull
```

Further Reading
----

* [file structure](structure.md)

	A walkthrough of the folder hierarchy in this library.

* [api](api/index.md)

	The API documentation index.