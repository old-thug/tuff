# **Tuff**

**Tuff** is a **systems programming language** that lives between **C** and **Rust**.
It offers the **performance and semantics of C** with **modern ergonomics** inspired by Rust — concise syntax, expressive types, and control that never hides from you.

> **No runtime. No GC. No hand-holding. Just control.**

---

## 🚀 Overview

Tuff is built for developers who want:

* C’s **predictability** and **low-level access**
* Rust-like **ergonomics**, **pattern matching**, and **option types**
* **LLVM backend** for real, optimized machine code
* A toolchain written in **C**, designed to be **self-hosting**

---

## 🧩 Language Example

```tuff
func add(a: i32, b: i32) i32 {
    a + b
}

func div(a: i32, b: i32) ?i32 {
    if b == 0 then nil else then a / b
}

func factorial(n: i32) i32 {
    a := n;
    b := n - 1;
    while b >= 2 {
        a *= b;
        b -= 1;
    }
    a
}

extern func printf(fmt: *char, ...) i32;
extern func scanf(fmt: *char, ...) i32;

func main() i32 {
    use ffi; # for to_cstr(&str)
    printf("Hello World".to_cstr());
    n: i32 = 0;
    scanf("%d".to_cstr(), &n);
    printf("N = %d".to_cstr(), n);
}
```

### Structs, Unions, and Pattern Matching

```tuff
type Foo = struct {
    a: i32,
    b: &str,
};

type Bar = union {
    foo: Foo,
    baz: int,
};

impl Foo {
    init(a: i32, b: &str) {
        self.a = a;
        self.b = b;
    }

    func name(&self) &str { self.b }
    func age(&self) i32 { self.a }
}

func main() void {
    use io; # for println
    bar: Bar = '{ .baz = 3 };

    match bar {
        baz |x| -> { println("x = %d", x); }
        foo |f| -> { println("f = %v", f); }
    }
}
```

---

## ⚙️ Design Principles

* **C semantics** — explicit memory model, predictable execution
* **LLVM backend** — portable, efficient machine code
* **Optional safety** — nullables, matches, and type inference without runtime cost
* **No hidden allocations** — what you write is what runs
* **Direct C interop** — headers in, functions out

---

## 🧠 Compiler Architecture

Written entirely in **C**, targeting **LLVM IR** for code generation.

### Completeds Components

* **Lexer** – UTF-8 aware tokenizer [ ]
* **Parser** – recursive-descent style, produces an AST [ ]
* **Semantic analyzer** – name resolution, type checking [ ]
* **LLVM codegen** – IR builder emitting modules and functions [ ]
* **Runtime stubs** – minimal, opt-in FFI utilities [ ]

### Planned Features

* Self-hosting compiler
* Lightweight module system
* Optional borrow-checking layer
* Inline assembly support
* Standard library (IO, FFI, memory utilities)

---

## 🧰 Build

```bash
git clone https://github.com/old-thug/tuff.git
cd tuff
mkdir bin
cmake -S . -B bin/
cmake --build bin/
```

By default, the build produces `tuffc`, the compiler driver.
LLVM (≥ 15.0) and a C99 compiler are required.

---

## 🤝 Contributing

Contributions are welcome — whether you’re fixing a parser edge case, designing syntax, or extending LLVM codegen.

1. Fork the repo
2. Create a feature branch (`git checkout -b feature/new-thing`)
3. Commit your changes (`git commit -m "feat: add new thing"`)
4. Push to your fork and open a PR

---


