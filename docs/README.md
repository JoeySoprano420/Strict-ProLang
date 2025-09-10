# Strict-ProLang Documentation

## Overview
Strict-ProLang is a programming language featuring base-12 arithmetic, strict instruction counting, and contextual safety.

## Language Features

### Base-12 Number System
- Uses digits 0-9, A, B
- A represents 10 in decimal
- B represents 11 in decimal
- Natural for dozen-based calculations

### Strict Mode
- Every instruction is counted
- Enforces safe execution patterns
- Prevents resource abuse

### Contextual Variables
- Variables have contextual meaning
- Scoping is strictly managed
- Memory-safe operations

## Syntax Reference

### Variable Declaration
```strict-prolang
let variableName = value;
```

### Output
```strict-prolang
print expression;
```

### Comments
```strict-prolang
// Single line comment
```

## Examples

See the `examples/` directory for sample programs.

## API Reference

### Core Classes

#### StrictProLang
Main interpreter class with methods:
- `run(source)` - Execute source code
- `compile(source)` - Compile to IR
- `startRepl()` - Start interactive mode
- `base12ToBase10(value)` - Convert numbers

## Command Line Usage

```bash
strict-prolang [options] [file]
```

Options:
- `-h, --help` - Show help
- `-v, --version` - Show version
- `-i, --interactive` - Interactive mode
- `-c, --compile` - Compile mode