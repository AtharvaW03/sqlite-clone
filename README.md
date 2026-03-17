# SQLite Clone

A simplified SQLite database implementation written in C, built from scratch to understand how relational databases work under the hood.

## Features

- **REPL Interface** — Interactive prompt for executing SQL commands
- **SQL Support** — `INSERT` and `SELECT` statements with input validation
- **B-Tree Storage Engine** — Data stored in a B-tree with both leaf and internal nodes
  - Binary search for key lookup
  - Leaf node splitting when full
  - Internal node splitting for multi-level trees
  - Sibling pointers for sequential leaf scanning
- **Persistence** — File-backed pager that reads/writes 4KB pages to disk
- **Row Storage** — Serialized row format with `id`, `username` (32 chars), and `email` (255 chars)

## Building

```bash
gcc -o db main.c
```

## Usage

```bash
./db mydb.db
```

### Commands

```
db > insert 1 alice alice@example.com
Executed.
db > select
(1, alice, alice@example.com)
Executed.
db > .btree
Tree:
- leaf (size 1)
  - 1
db > .exit
```

### Meta-commands

| Command    | Description                   |
|------------|-------------------------------|
| `.btree`   | Print the B-tree structure    |
| `.constants` | Print B-tree node constants |
| `.exit`    | Exit the REPL                 |

## How It Works

The database stores rows in a B-tree indexed by `id`. Each node occupies a single 4KB page. Leaf nodes hold the actual row data, while internal nodes store keys and child pointers to navigate the tree. When a leaf fills up, it splits and promotes a key to its parent internal node. Internal nodes split similarly when they reach capacity.

Data is persisted to a file via a pager that caches pages in memory and flushes them to disk on exit.
