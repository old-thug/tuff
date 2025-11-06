# Contributing to Tuff

Thanks for checking out **Tuff**!
Whether you're fixing a bug or suggesting syntax changes, your help is appreciated.

## 🧰 Build Instructions
1. Clone the repo and initialize:
   ```bash
   git clone https://github.com/old-thug/tuff.git
   cd tuff
   mkdir bin && cd bin
   cmake ..
   cmake --build .
````

2. Make sure you have **LLVM 15+** and a **C99 compiler** installed.

## 🧩 Structure

| Folder     | Description                                     |
| ---------- | ----------------------------------------------- |
| `libtuff`  | Core language (lexer, parser, AST, type system) |
| `tuffc`    | Compiler (LLVM backend)                         |
| `tuffy`    | Interpreter / REPL                              |
| `examples` | Example programs                                |

## 🧠 Contributing Workflow

1. Fork this repository
2. Create a new branch (`git checkout -b feature/your-idea`)
3. Make your changes
4. Run builds and tests
5. Commit (`git commit -m "feat: short description"`)
6. Push and open a Pull Request

## 💬 Communication

Join or start a [Discussion](https://github.com/old-thug/tuff/discussions) for:

* Language design ideas
* Compiler architecture debates
* Bug reports or weird behavior

## ✨ Guidelines

* Follow the existing C coding style (consistent braces, spacing).
* Write small commits — it helps with review.
* Explain *why* in commit messages, not just *what*.

