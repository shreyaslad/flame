# flame

![alt text](flame.PNG "FlameOS")

flame was originally based off this [os tutorial](https://github.com/cfenollosa/os-tutorial).

## Building

Dependencies:
- `git` @ Stable
- `make` @ Stable
- `x86_64-elf-gcc` >= 8.0.0
- `binutils` @ Stable
- `xorriso` >= 1.2.9
- `nasm` @ Stable
- `qemu` @ Stable (>= 4.2.0 for MacOS 15+)

```
make
```

## Running

```
make run
```

## Debugging

```
make debug
```

## License

MIT License

Copyright (c) 2019-2020 Shreyas Lad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
