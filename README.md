# mako - full bitcoin implementation in C

__Mako__ is a from-scratch bitcoin reimplementation, written in "almost-C89"
(i.e. it can be compiled by a C89 compiler if `<stdint.h>` is available -- no
other C99 features are used).

Mako is more-or-less dependency-less. It only vendors lmdb, which has a
remarkably small codebase. Mako aims to support any POSIX.1-2001 operating
system as well as Windows XP and up.

Mako has a re-usable architecture. The core library (libmako) does no IO, and
has almost every tool needed for working with bitcoin. The fullnode (libnode)
is also a separate library which the final executable links to.

## Development Status

__Do not use mako in production__. Mako is under heavy development and almost
guaranteed to have a significant amount of bugs at this point in time.

The node itself is currently incomplete for various reasons, including:

- A number of RPC calls are missing (notably essential things like
  `getblocktemplate`).
- The _entire_ wallet RPC is currently missing, along with the wallet itself.
- Consensus & policy rules are _mostly_ complete: mako supports softforks up to
  and including segwit, but not later additions like taproot.
- A number of tests still need to be written.
- I haven't taken the time to have mako fully sync mainnet, only signet. Mako
  passes all of the transaction and script test vectors from bitcoin core, but
  there's no telling what consensus issue may arise in its current state.

Some caveats:

- Mako uses LMDB for the utxo database: on boot, it will allocate a
  16G sparse file (this will also use 16G of virtual memory). If you are on a
  platform which does not support sparse files, 16G of actual disk space will be
  used.
- Because the mainnet utxo database is currently several gigs (>=5G), mako will
  likely never be able to sync mainnet on a 32-bit machine as a 32-bit address
  space lacks the ability to map a 5 gigabyte file into memory.

## Build & Usage (for experimentation only)

Mako currently has a very simple CMake build which doesn't properly test for
features, so don't expect it to work on more obscure platforms. So far, mako
has only been tested on Linux and Win32 (cross-compiled with mingw).

`cmake . && make` will produce two binaries: `mako and makod`. The arguments
mimic `bitcoin-cli` and `bitcoind` respectively.

## Why?

There are a few reasons mako needed to exist:

- Because it is C, all data structures and primitives are written by hand and
  are not subject to any particular platform's implementation of them. On top
  of that, mako makes very sparing use of the C standard library. This makes
  mako more auditable than a bitcoin implementation written in [C++][cxx], JS,
  Rust, Go, etc.
- A low-level, portable, and re-usable codebase for bitcoin is useful for a
  number of projects.
- Contrary to what some people might tell you, multiple implementations of a
  protocol are a good thing. In bitcoin's case, they are _necessary_ to
  mitigate the harm of developer centralization.
- The bitcoin protocol itself should be recorded in as many places as possible
  for posterity.
- Mako is planned to be used as the base for a port to the [handshake][hns]
  protocol, among other things.

## Contribution and License Agreement

If you contribute code to this project, you are implicitly allowing your code
to be distributed under the MIT license. You are also implicitly verifying that
all code is your original work. `</legalese>`

## License

- Copyright (c) 2021, Christopher Jeffrey (MIT License).

See LICENSE for more info.

[cxx]: http://harmful.cat-v.org/software/c++/linus
[hns]: https://handshake.org/
