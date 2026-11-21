*This project has been created as part of the 42 curriculum by cfaris.*

# get_next_line

## Description

**get_next_line** is a C function that reads a file descriptor **one line at a time**:

```c
char *get_next_line(int fd);
```

Each call returns the next line of the file pointed to by `fd`, **including the final `\n`** when there is one. It returns `NULL` when there is nothing left to read, or when an error occurs.

Example with a file containing `Bonjour\nSalut\nCoucou`:

| Call | Returned value |
|------|----------------|
| 1st  | `"Bonjour\n"` |
| 2nd  | `"Salut\n"` |
| 3rd  | `"Coucou"` (end of file, so no `\n`) |
| 4th  | `NULL` |

Rules of the project (mandatory part):

- Only `read`, `malloc` and `free` are allowed. No `libft`, no `lseek`, no global variable.
- The number of bytes read at each `read()` is the macro `BUFFER_SIZE`, set at compilation with `-D BUFFER_SIZE=n`. The code must work with any `n > 0`, and must also compile without the flag.
- It must work with regular files and with standard input.
- Files delivered: `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`.

## Instructions

### Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

The `-D` option defines a macro from the command line: `-D BUFFER_SIZE=42` is equivalent to writing `#define BUFFER_SIZE 42` at the top of every compiled `.c` file.

If `-D BUFFER_SIZE=...` is omitted, the header defines a default value (42). `main.c` is a test program and is **not** part of the project.

### Example of `main.c`

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

### Execution

```bash
./gnl                # reads test.txt
./gnl < test.txt     # standard input can be used by calling get_next_line(0)
```

The caller owns every returned line and must `free` it.

## Algorithm: explanation and justification

### In short

First, the function receives a file descriptor, which lets it access a file. Its goal is to take each line ending with a `\n` and return it, so that the caller (the `main`) can use it. We never know in advance how many characters a line contains, and `read()` does not stop at the end of a line: it simply reads `BUFFER_SIZE` bytes. So a single `read()` can give half a line, exactly one line, or several lines at once.

To solve this, `get_next_line` uses a **`static` variable** (the *stash*). It keeps the text that was already read but not yet returned, even after the function exits. When the function is called again, it first looks at what the stash already contains, and it decides whether it must call `read()` again to get more characters.

Each call always does **three jobs, in this order**:

1. **Read** until the stash contains a `\n` (or until the end of the file): `read_stash`.
2. **Extract** the first line of the stash, up to the `\n` included: `ft_extract_line`.
3. **Keep the rest** in the stash for the next call: `ft_keep_rest`.

### Where the memory lives

| Element | Where | Lifetime |
|---------|-------|----------|
| the pointer `stash` (`static`) | static data zone | the whole program |
| the text pointed to by `stash` | heap (`malloc`) | until we `free` it |
| `buf` | heap (`malloc`) | one call |
| the returned `line` | heap (`malloc`) | until the caller `free`s it |

### Function by function

#### `get_next_line(int fd)`

1. **Check** that `fd >= 0` and `BUFFER_SIZE > 0`. If not, return `NULL`. This is done first, before any allocation. A `BUFFER_SIZE` of 0 would make `read()` return 0, which looks like an end of file.
2. **Allocate `buf`** with `malloc(BUFFER_SIZE + 1)`: `BUFFER_SIZE` bytes for `read()`, plus one for the `'\0'`. It is on the heap and not a local array: with a huge `BUFFER_SIZE` (for example 10000000) a local array overflows the stack and crashes. If `malloc` fails, free the stash and return `NULL`.
3. **Fill the stash**: `stash = read_stash(fd, stash, buf)`.
4. **Free `buf`**: it was only a temporary reading area. It is freed here, once, for every possible way out of the function.
5. **Nothing to return?** If `stash` is `NULL` or empty (`stash[0] == '\0'`), then we are at the end of the file, or an error happened. Free the stash, set it to `NULL`, return `NULL`. The pointer is tested **before** `stash[0]`, otherwise we would read inside a `NULL` pointer.
6. **Extract the line**: `line = ft_extract_line(stash)`. If it fails, free the stash and return `NULL`.
7. **Keep the rest**: `stash = ft_keep_rest(stash)`. The old stash is freed by that function, so `stash` must be overwritten with the new pointer.
8. **Return `line`.**

The variable `static char *stash;` is **not** initialised by a separate line such as `stash = NULL;`, because such an instruction would run at every call and erase the saved text. A `static` variable is automatically `NULL` at the beginning.

#### `read_stash(int fd, char *stash, char *buf)`

1. `ret = 1`, so that the loop starts.
2. **Loop** `while (!ft_strchr_gnl(stash) && ret > 0)`: we keep reading while the stash has **no** `\n` **and** `read()` did not return `0`. The second condition is essential: a file that does not end with `\n` would otherwise make the loop infinite.
3. `ret = read(fd, buf, BUFFER_SIZE)`: `ret` is the number of bytes really read.
4. If `ret < 0`: an error occurred. Free the stash and return `NULL`. (`buf` is freed by the caller.) Without this `free`, the only pointer to the stash would be lost when `get_next_line` overwrites it with `NULL`, which is a memory leak.
5. `buf[ret] = '\0'`: `read()` does not add a `'\0'`. It is placed just **after the bytes read**, so old characters left by a previous `read()` are ignored.
6. `stash = ft_strjoin_gnl(stash, buf)`: append `buf` to the stash. If it returns `NULL` (failed `malloc`), return `NULL`. The old stash was already freed inside `ft_strjoin_gnl`.
7. When the loop stops (a `\n` was found, or end of file), return `stash`.

Only a return value of **`0`** means end of file. On standard input or a pipe, `read()` can return fewer bytes than `BUFFER_SIZE` without being at the end.

#### `free_stash(char **stash)`

1. `free(*stash)`: free the block.
2. `*stash = NULL`: the real pointer becomes `NULL`.
3. `return (NULL)`, so that we can write `return (free_stash(&stash));`.

It receives the **address** of the pointer (`&stash`, so a `char **`). With a simple copy of the pointer, the block would be freed but the real `stash` would still point to freed memory (risk of a double `free` at the next call).

#### `ft_strlen_gnl(char *str)`

1. If `str` is `NULL`, return `0` (the stash is `NULL` at the first call).
2. Start with `i = 0` and increase `i` while `str[i]` is not `'\0'`.
3. Return `i`. The `'\0'` is not counted.

#### `ft_strchr_gnl(char *str)`

1. If `str` is `NULL`, return `0`.
2. Walk through the string. As soon as a character is `'\n'`, return `1`.
3. If the end is reached, return `0`.

#### `ft_strjoin_gnl(char *dst, char *buf)`

1. `malloc` a new block of `len(dst) + len(buf) + 1` bytes. The `+ 1` is for the final `'\0'`.
2. If `malloc` fails, `free(dst)` and return `NULL` (otherwise the old stash would leak).
3. Copy `dst` into the new block with `while (dst && dst[i])`. The test `dst` comes first because `dst` is `NULL` at the first call, and reading `dst[0]` on a `NULL` pointer would crash. The loop tests the current character directly and does not call `ft_strlen_gnl` at each turn, which would make the copy very slow.
4. Copy `buf` right after, at position `i + j`.
5. Write the `'\0'` at the end.
6. `free(dst)`: an allocated block cannot grow, so a new bigger block is created and the old one is released.
7. Return the new block.

#### `ft_extract_line(char *stash)`

1. Count `len` while the character is neither `'\0'` nor `'\n'`.
2. If the loop stopped on a `'\n'`, do `len++` so that the `'\n'` is included in the line.
3. `malloc(len + 1)`; return `NULL` if it fails.
4. Copy `len` characters, then write the `'\0'`.

If there is no `'\n'` (last line of a file without final newline), the loop stops on the `'\0'` and the whole remaining text is the line.

#### `ft_keep_rest(char *stash)`

1. Find the `'\n'` with `i`, then do `i++` to jump **after** it. Without this `i++`, the `'\n'` would be copied again at the beginning of the rest and an empty line would appear that is not in the file. The test `stash[i] == '\n'` is needed because without a `'\n'` the loop stopped on `'\0'`, and `i++` would go outside the string.
2. `rest = NULL`: we suppose there is nothing to keep.
3. If `stash[i] != '\0'`, some text remains: `rest = malloc(len(stash) - i + 1)`. `len(stash) - i` is the number of characters kept, and `+ 1` is for the `'\0'`. Without it, the `'\0'` would be written outside the block.
4. Copy the remaining characters into `rest` and write the `'\0'`.
5. `free(stash)`: the old stash is always released.
6. Return `rest`. A `NULL` result means "nothing left in the stash".

### Worked example

File `Salut\nHi\n` with `BUFFER_SIZE=4` (the successive reads give `Salu`, then `t\nHi`, then `\n`).

| Call | What happens | `stash` at the end |
|------|--------------|--------------------|
| 1st | `read` → `Salu` (no `\n`, read again), `read` → `t\nHi` → stash `"Salut\nHi"`. Extract `"Salut\n"`, keep `"Hi"`. **Returns `"Salut\n"`.** | `"Hi"` |
| 2nd | No `\n` in `"Hi"`, so `read` → `\n` → stash `"Hi\n"`. Extract `"Hi\n"`, nothing left. **Returns `"Hi\n"`.** | `NULL` |
| 3rd | `read` → `0` (end), the join gives `""`, which is empty. **Returns `NULL`.** | `NULL` |

The empty-stash test of step 5 is what stops the caller's loop. Without it, `""` (which is not `NULL`) would be returned forever.

### Cases handled

| Case | Behaviour |
|------|-----------|
| invalid `fd` (`-1`, closed, a directory) | `NULL`, no leak |
| `BUFFER_SIZE <= 0` | `NULL` |
| empty file | `NULL` at the first call |
| last line without `\n` | returned without `\n`, then `NULL` |
| `BUFFER_SIZE` very small (1) or very large (10000000) | works |
| standard input / pipe (partial reads) | works, only `read() == 0` means end |
| `read()` error (`-1`) | stash freed, `NULL` |
| `malloc` failure | everything allocated is freed, `NULL` |

### Why this algorithm

The stash approach was chosen over a linked list of chunks for three reasons:

- **Simplicity**: one `static` pointer to follow, and each helper function is short and has one job.
- **Safer memory**: a single block to track means fewer `malloc`/`free` pairs, so fewer chances of a leak.
- **Extension**: the bonus (several file descriptors at once) only needs one stash per `fd`.

Its weak point is speed in extreme cases: `ft_strjoin_gnl` copies the whole stash at each `read()`, so the time grows with the square of the line length. On the development machine, `BUFFER_SIZE=1` with a line of 50000 characters took about 4 seconds, and 20000 characters about 0.6 second. With a normal `BUFFER_SIZE` this is not an issue. A linked list avoids these copies at the price of more allocations and more code, so it could be a later improvement.

## Bonus

The bonus lets `get_next_line` read from **several file descriptors at the same time**, switching between them without losing the reading position of each one. For example, one call on `fd1`, one on `fd2`, then back to `fd1` continues exactly where `fd1` stopped.

Files delivered: `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h`.

Rule of the bonus: only **one** `static` variable is allowed.

### What changes compared to the mandatory part

The mandatory part has one stash for the whole program. The bonus needs **one stash per file descriptor**, so the single pointer becomes an array indexed by the `fd`:

```c
static char	*stash[FD_MAX];
```

`stash[fd]` holds the text already read but not yet returned for that `fd`. Everything else (`read_stash`, `ft_extract_line`, `ft_keep_rest`, the utils) works the same way, but receives `stash[fd]` instead of `stash`. The helper functions did not need any change.

### Size of the array: `FD_MAX`

`FD_MAX` is defined in `get_next_line_bonus.h` with `#ifndef` (default `1024`, the usual limit of open file descriptors on the school computers), so it can be changed with `-D FD_MAX=n`. The array takes about 8 KB with the default value.

### Protection of the array

Before any access, the function checks:

```c
if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FD_MAX)
	return (NULL);
```

`fd >= FD_MAX` (and not `>`) is needed because the valid indexes go from `0` to `FD_MAX - 1`. Without it, a large `fd` would write outside the array.

### Bonus cases handled

| Case | Behaviour |
|------|-----------|
| alternating calls on 2 or more `fd` | each `fd` keeps its own position |
| one `fd` reaches the end, the others continue | only `stash[fd]` is freed and set to `NULL` |
| `fd < 0` or `fd >= FD_MAX` | `NULL`, no access to the array |
| standard input mixed with files | works, `fd` 0 has its own slot |

### Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
	get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

## Technical choices

- The default `BUFFER_SIZE` (42) is defined in `get_next_line.h` with `#ifndef`, so a `-D BUFFER_SIZE=n` on the command line always wins.
- `buf` is allocated with `malloc` and not declared as a local array, to survive large values of `BUFFER_SIZE`.
- Every `free` is followed by the pointer being set to `NULL` when the pointer stays alive (the `static` stash).
- Memory errors (`NULL` returned by `malloc`) never crash the function: it frees what it owns and returns `NULL`.

## Testing

Cases to check:

- compile with and without `-D BUFFER_SIZE`;
- `BUFFER_SIZE` = 1, 2, 3, 4, 7, 42, 1000, 9999, 10000000;
- normal file, empty file, file without final `\n`, file with only `\n`, one-character file, blank lines, very long line;
- standard input (`./gnl < file` and a piped input);
- invalid `fd` (`-1`, closed `fd`, a directory);
- bonus: several files read in alternation, and `fd` values at the limit of `FD_MAX`;
- memory: compile with `-fsanitize=address` (and run `valgrind` when available), and compare the program output with the original file using `cmp`.

## Resources

- Linux man pages: `open(2)`, `read(2)`, `close(2)`, `malloc(3)`, `free(3)`.
- GNU C Library manual, chapter on memory allocation.
- *The C Programming Language*, B. Kernighan and D. Ritchie: pointers, strings, storage classes (`static`).
- Documentation of AddressSanitizer and Valgrind, to find memory errors and leaks.
- The 42 subject of `get_next_line`.

### How AI was used

I used an AI assistant during this project to help me understand some concepts, as well as to get examples and explanations whenever I had difficulties understanding certain parts of the project.