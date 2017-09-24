# Raw Strings

A simple, completely incompatible implementation of `strings` from
`GNU Binutils`. Sparked by the lack of 64-bit build of the real deal for a
popular proprietary OS.

### Kickstart
```
git clone https://github.com/ankitpati/rstrings.git
cd rstrings
make

./rstrings rstrings
```

### Installation
To install the binary for the current user,

```
make && make install
```

To install the binary for all users (requires `root`),

```
make && sudo make install
```

### Samples
Place the binary on your `PATH` for easier access. Refer to “Installation.”

```
rstrings infile             # reads from filename, outputs to stdout
rstrings infile outfile     # reads from infile  , outputs to outfile
rstrings                    # reads from stdin   , outputs to stdout
```
