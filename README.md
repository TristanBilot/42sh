# 42sh

## Command substitution

```shell
echo $(ls -la && pwd)
```

## Arithmetic substitution

```shell
echo $((2*33/2-2))
```


```shell
echo $((!3&&1 || 3))
```

```shell
echo $((~2- ~3 + ~1&& ~4/4-1))
```

## Redirections and pipes

```shell
./42sh 2>/dev/null >/dev/null
```

```shell
tree . | ./42sh >> file
```

```shell
cat< test_suite.py
```

## Variables and aliases

```shell
var=$(echo 42); echo $var
```

```shell
echo $RANDOM${UID}
```

```shell
alias ll="ls -l"; ll; unalias ll
```

## Loops

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

## Conditions

```shell
if cat README; then echo success; elif ls; then echo success; else echo failed; fi
```

```shell
if echo toto; then echo success; else echo failed; fi
```

```shell
case a in b | c | d ) hello & ;; esac
```

## Binary operators

```shell
( a || ( ( b && c; ) || ls\n ); )
```

## Signals and History

Key|Signal|
:-:|:--
<kbd>Ctrl</kbd>+<kbd>C</kbd><br><kbd>Ctrl</kbd>+<kbd>Z</kbd><br><kbd>Ctrl</kbd>+<kbd>L</kbd>|Kill current process<br>Suspend curent process<br>Clear shell
<kbd>Tab</kbd><br>|Autocompletion
<kbd>^</kbd><br><kbd>v</kbd>|Browse history
<kbd><</kbd><br><kbd>></kbd>|Browse current command


