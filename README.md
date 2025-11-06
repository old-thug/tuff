# 🧱 Tuff

**Tuff** is a **systems programming language** that lives between **C** and **Rust**.  
It combines **C’s semantics and control** with **modern ergonomics** — concise syntax, expressive types, and zero runtime hand-holding.

> **No GC. No safety nets. Just control.**

---

## 🚀 Overview

Tuff consists of **three core components**:

| Project | Description |
|----------|--------------|
| **libtuff** | Core library: lexer, parser, AST, type system, and shared utilities. |
| **tuffc** | The Tuff compiler. Builds LLVM IR and emits native binaries. |
| **tuffy** | The interpreter (and REPL). Executes Tuff code directly. |

---

## 🧩 Language Example

```tuff
func add(a: i32, b: i32) i32 {
    a + b
}

func div(a: i32, b: i32) ?i32 {
    if b == 0 then nil else a / b
}

func factorial(n: i32) i32 {
    var a := n;
    var b := n - 1;
    while b >= 2 {
        a *= b;
        b -= 1;
    }
    a
}

func main() void {
    use io; // hypothetical stdlib
    println("Factorial of 5 = %d", factorial(5));
}
````

---

## ⚙️ Design Principles

* **Predictable semantics** — no hidden allocations, no implicit conversions
* **LLVM backend** — real machine code, no VM tax
* **Optional safety** — nullables, pattern matching, and type inference without cost
* **First-class C interop** — headers in, functions out
* **No borrow checker** — ownership via move semantics, simple and explicit

---

## 🧠 Architecture

Tuff is structured as a **multi-target toolchain**:

```
libtuff/
├── lexer/
├── parser/
├── ast/
└── ir/
tuffc/  → compiler binary (LLVM backend)
tuffy/  → interpreter (bytecode / AST execution)
```

Both the compiler and interpreter depend on `libtuff` for core language logic,
but each maintains its own symbol resolution and execution model.

---

## 🧰 Build

```bash
git clone https://github.com/old-thug/tuff.git
cd tuff
mkdir bin && cd bin
cmake ..
cmake --build .
```

By default this builds:

* `tuffc` → the compiler
* `tuffy` → the interpreter
* `libtuff` → static library used by both

> Requires LLVM 15+ and a C99-compatible compiler.

---

## 🧪 Example Usage

Compile:

```bash
./tuffc examples/hello.tuff -o hello
./hello
```

Interpret:

```bash
./tuffy examples/hello.tuff
```

---

## 🧭 Roadmap

* [ ] Self-hosting compiler
* [ ] Standard library (I/O, FFI, memory)
* [ ] Optional borrow-checking pass
* [ ] Inline assembly
* [ ] Module and package system

---

## 🤝 Contributing

All contributions are welcome — code, design input, or docs.

1. Fork the repo
2. Create a feature branch
3. Commit and push your changes
4. Open a pull request

Check the [Issues](https://github.com/old-thug/tuff/issues) and [Discussions](https://github.com/old-thug/tuff/discussions) for active topics.
---
