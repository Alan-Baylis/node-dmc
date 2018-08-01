# dmc-node

Attempt at nodejs bindings with CMake.js and Native Abstractions for Node.js for
https://github.com/planaria/dmc

Very experimental, do not use

## Requirement

- MacOS or Linux for now
- git
- make
- cmake
- Boost C++ Libraries (>= 1.64.0)
- `yarn global add cmake-js`

## Usage

### First time

```bash
# Go take a coffee, might take a while
./configure.sh
```

### The rest

You're here early. Currently the binding export the original DMC test that
generates a cube.

Once correctly built, you should have a `build/Release/dmc.node` file that
contains everything. Import it and have fun, I guess:

```js
const dmc = require('./build/Release/dmc');
const voxels = dmc.getTestTriangles();
```

A sample test writes generated triangles to the console : `node test.js`. You
can also uncomment lines 103 and 104 in `include/dmc/dmc.cc` to create a valid
STL file when running this test.

## Potential deployment problems

Eigen (the math lib used by DMC) uses Boost for some parts of its building. And
since DMC requires Eigen, it is built before from a fresh git clone. Boost is
not required for Eigen to run, but since I have my local Boost install, cmake
gets the package correctly on install and builds the correct headers. **I have
no idea if this will work on a setup without local Boost, please file an issue
if this is the case**.

The funny thing is that the point of Cmake.js is to be able to add libs like
Boost to nodejs native libs. So the project depends on the cmake.js Boost util
package `boost-lib` that gets a fresh Boost install if you don't have one.
**BUT** this happens after Eigen is built, hence the potential problems.

Since Boost is non essential to Eigen, I might implement a workaround to remove
the build parts that depend on it. Moreover since DMC only depends on core Eigen
features.

Regarding Windows support, it's not configured yet, but nothing should stop it
from running with a modern MSVC compiler, since DMC was originally made as a VS
2017 project. Will look into it as soon as I can test it.

## License

- Original code by planaria using the BSD 3-Clause License
- node binding using the MIT License

See [LICENSE](LICENSE)
