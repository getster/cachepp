Tutorial Introduction
====

In order to showcase the features of this framework, we will go through a (rather contrived) example of implementing both `CacheInterface` (the cache) and 
`LineInterface` (the data). Let us examine a use-case which will motivate us to write a cache in the first place:

> One day at CashCo., a programmer received a complaint that an important computing subcomponent in the *Department of Efficiency and Such* was very slow. After some 
> research, he realized that this component was processing a large number of files, all of which it kept in memory while processing. He decides that in order to deal 
> with this problem, he will use a cache to load different files to and from memory. He decides that an LRU cache eviction scheme would be the best way to accomplish 
> this.

Table of Contents
----

* [Setting Up](intro.md)
* [Implementing the Cache Interface](cache.md)
* [Implementing the Line Interface](line.md)
* [Testing the Cache](testing.md)

Setting Up
----

Take a look at the [tutorial directory](../../tutorial/), which has been populated with the libraries necessary for this exercise. In case you wish to work along, use 
this folder as a working directory. The included `Makefile` will compile as-is, and will produce an executable called `tutorial.app`. All header files in this tutorial 
should go into the [tutorial/include/src](../../tutorial/include/src/) directory, and all source files into the [tutorial/src/](../../tutorial/src/) directory.

We have added `tutorial/include/` to the include path to make including header files much simpler -- a header file `tutorial/include/src/foo.h` can be included in all 
files within the tutorial scope as `#include "src/foo.h"`.

If you are creating a project from scratch, simply copy the base tutorial directory, `cd` into it and *replace the* `external/cachepp` *link with a* `git clone` *of the 
repository*:

```bash
cd tutorial/external
rm cachepp
git clone https://github.com/cripplet/cachepp.git
cd cachepp
# get all dependencies
git submodule update --init --recursive
cd ../../
```

All other files within `tutorial/` are symbolically linked from `tutorial/external/cachepp/` and so will automatically work as long as `tutorial/external/cachepp/` is 
properly initialized.

In the future, **we will assume** `tutorial/` **as the root directory** -- that is, when we refer to `/` in the documentation we mean `tutorial/` (e.g. `/src/` refers to 
`tutorial/src/`, etc.).

Please direct any questions to [gmail](mailto:minke.zhang@gmail.com). Any issues with the tutorial should be addressed on the project 
[issues](https://github.com/cripplet/cachepp/issues) page.

Further Reading
----

* Next -- [Implementing the Cache Interface](cache.md)
