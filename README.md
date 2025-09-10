# Strict-ProLang

[![CI](https://github.com/JoeySoprano420/Strict-ProLang/workflows/CI/badge.svg)](https://github.com/JoeySoprano420/Strict-ProLang/actions)
[![npm version](https://badge.fury.io/js/strict-prolang.svg)](https://badge.fury.io/js/strict-prolang)

**"Every Instruction Counts — Safe, Contextual, Base-12 Precision."**

Strict-ProLang is a programming language designed with precision and safety in mind, featuring a base-12 number system and strict instruction counting for optimal performance and reliability.

## Features

- 🔢 **Base-12 Number System**: Native support for duodecimal (base-12) arithmetic
- 🛡️ **Strict Mode**: Every instruction is counted and validated
- 🎯 **Contextual Safety**: Advanced context management for variable scoping
- 🚀 **Fast Execution**: Lightweight interpreter with compilation support
- 📦 **Easy Installation**: Available via npm and GitHub releases

## Installation

### Via NPM (Recommended)
```bash
npm install -g strict-prolang
```

### Via GitHub Releases
1. Go to the [Releases page](https://github.com/JoeySoprano420/Strict-ProLang/releases)
2. Download the latest release archive
3. Extract and run the installation script

### From Source
```bash
git clone https://github.com/JoeySoprano420/Strict-ProLang.git
cd Strict-ProLang
npm install
npm run build
npm link
```

## Usage

### Command Line Interface
```bash
# Run a Strict-ProLang program
strict-prolang program.spl

# Start interactive mode
strict-prolang -i

# Compile to intermediate representation
strict-prolang -c program.spl

# Show version
strict-prolang --version

# Show help
strict-prolang --help
```

### Programming Examples

#### Basic Variables (Base-12)
```strict-prolang
// Variables in base-12
let x = A;      // A = 10 in base-10
let y = B;      // B = 11 in base-10  
let z = 10;     // 10 in base-12 = 12 in base-10

print x;        // Outputs: 10
print y;        // Outputs: 11
print z;        // Outputs: 12
```

#### Interactive Mode
```bash
$ strict-prolang -i
Strict-ProLang v0.1.0 Interactive Mode
Every Instruction Counts — Safe, Contextual, Base-12 Precision
Type "exit" to quit, "help" for help

spl> let x = A
spl> print x
Hello from Strict-ProLang!
spl> exit
Goodbye!
```

## API Reference

### Node.js Integration
```javascript
const StrictProLang = require('strict-prolang');

const spl = new StrictProLang();

// Run code
spl.run('let x = A; print x;');

// Compile to IR
const ir = spl.compile('let x = 5');

// Base-12 conversion utility
const decimal = spl.base12ToBase10('1B'); // Returns 23
```

## Development

### Building from Source
```bash
npm install
npm run build
```

### Running Tests
```bash
npm test
```

### Project Structure
```
Strict-ProLang/
├── src/           # Core interpreter implementation
├── bin/           # CLI executable
├── test/          # Test suite
├── examples/      # Example programs
├── docs/          # Documentation
└── dist/          # Built artifacts
```

## Base-12 Number System

Strict-ProLang uses base-12 (duodecimal) numbers:
- Digits: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B
- A = 10 (decimal)
- B = 11 (decimal)
- 10 (base-12) = 12 (decimal)

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Releases & Downloads

- **Latest Release**: [GitHub Releases](https://github.com/JoeySoprano420/Strict-ProLang/releases)
- **NPM Package**: [strict-prolang](https://www.npmjs.com/package/strict-prolang)
- **Development**: Clone this repository

## Support

- 📖 [Documentation](docs/)
- 🐛 [Issue Tracker](https://github.com/JoeySoprano420/Strict-ProLang/issues)
- 💬 [Discussions](https://github.com/JoeySoprano420/Strict-ProLang/discussions)