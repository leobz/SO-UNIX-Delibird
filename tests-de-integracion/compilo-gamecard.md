Cargo configuración del test

```shell
$ cat byexample.config             # byexample: +fail-fast
sleep-time=<sleep-time>
```

Limpio compilación anterior

```shell
$ make clean -C ../gamecard/Debug    # byexample: +timeout=10 +fail-fast
<...>
```

Compilo Gamecard

```bash
$ make -C ../gamecard/Debug           # byexample: +timeout=10 +fail-fast
<...>Finished building target: gamecard<...>
```
