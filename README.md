# 42sh

42sh was a team project made at EPITA in which we had to implement a POSIX shell from scratch using pure C. No memory leaks were allowed and we needed to implement by ourselves most of useful C functions.
Our 42sh do not use standard builtins so we had to create the famous ones like echo, source, continue, break, exit, export...

Authors: 
Bilot Tristan |
Delfau Nora |
Salemi Enzar |
Thambithurai Madushan

- [x] POSIX compliant
- [x] no memory leaks
- [x] no warnings/errors
- [x] tested on ArchLinux, Ubuntu, MacOS

## Usage
### Build the shell (3 ways)
    1. ./build.sh
    2. mkdir build && cd build && cmake . && make
    3. cmake . && make
    
### Run the shell
    1. ./build/42sh
    2. ./42sh (only if built with the #3 way)
    
### Generate Doxygen documentations
    1. ./build.sh doc         (this will build/rebuild the 42sh before)
    2. cd build && make doc   (if the shell is already built)
    3. ./documentation.sh     (if the shell is already built)
    4. make doc               (if shell built with the 3rd way)

### Read Doxygen documentaions
    -- evince doc/latex/refman.pdf
    -- cd doc/man/man3 && man ./[SOURCE FILE NAME].3
    -- firefox doc/html/index.html

### Run the python testsuite
    1. ./build.sh check       (this will build/rebuild the 42sh before)
    2. cd build && make check (if the shell is already built)
    3. make check             (if shell built with the 3rd way)

### Clean the project (3 ways)
    1. ./clean.sh
    2. cd build && make clean
    3. make clean

## Commands
### Command substitution

```shell
echo $(ls -la && pwd)
```

### Arithmetic substitution

```shell
echo $((2*33/2-2))
```

```shell
echo $((!3&&1 || 3))
```

```shell
echo $((~2- ~3 + ~1&& ~4/4-1))
```

### Redirections and pipes

```shell
./42sh 2>/dev/null >/dev/null
```

```shell
tree . | ./42sh >> file
```

```shell
cat< test_suite.py
```

### Variables and aliases

```shell
var=$(echo 42); echo $var
```

```shell
echo $RANDOM${UID}
```

```shell
alias ll="ls -l"; ll; unalias ll
```

```shell
export PWD="/bin"; export
```

### Loops

```shell
for i in a b c d e; do echo "Hello, world !"; done
```

```shell
for i in {0..42}; do echo "current: $i"; done
```

```shell
while ! echo a | echo b | echo c; do echo toto; done
```

```shell
until echo a | echo b | echo c; do echo toto; done
```

### Conditions

```shell
if cat README; then echo success; elif ls; then echo success; else echo failed; fi
```

```shell
if echo toto; then echo success; else echo failed; fi
```

```shell
case a in b | c | d ) hello & ;; esac
```

### Binary operators

```shell
( a || ( ( b && c; ) || ls\n ); )
```

### Signals and History

Key|Signal|
:-:|:--
<kbd>Ctrl</kbd>+<kbd>C</kbd><br><kbd>Ctrl</kbd>+<kbd>Z</kbd><br><kbd>Ctrl</kbd>+<kbd>L</kbd>|Kill current process<br>Suspend curent process<br>Clear shell
<kbd>Tab</kbd><br>|Autocompletion
<kbd>^</kbd><br><kbd>v</kbd>|Browse history
<kbd><</kbd><br><kbd>></kbd>|Browse current command


