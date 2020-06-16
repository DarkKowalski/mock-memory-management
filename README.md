# Memory Management

Second assignment for 2020 OS course, SSE, Tongji Univ

## Dev Environment

- AMD Ryzen 7 4800U (Zen2)
- GNU/Linux 5.7.0 (Arch Linux)
- clang 10

## FreeList

build

```bash
CC=clang ./build_freelist.sh
```

run

```bash
./first_match < dataset/dataset.txt
./best_match < dataset/dataset.txt
```

## Dataset

Use I/O redirection. See `dataset/dataset.txt`

```
+--------------------------------------+
| header   | operation number          |
+--------------------------------------+
| allocate | a pid size                |
+--------------------------------------+
| free     | f pid                     |
+--------------------------------------+
```