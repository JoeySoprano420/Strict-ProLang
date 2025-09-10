#!/usr/bin/env node

const StrictProLang = require('../src/index.js');
const fs = require('fs');
const path = require('path');

function showHelp() {
    console.log(`
Strict-ProLang Interpreter
Usage: strict-prolang [options] [file]

Options:
  -h, --help     Show this help message
  -v, --version  Show version information
  -i, --interactive  Start interactive mode
  -c, --compile  Compile to intermediate representation
  
Examples:
  strict-prolang program.spl        # Run a Strict-ProLang file
  strict-prolang -i                 # Start interactive mode
  strict-prolang -c program.spl     # Compile program
`);
}

function showVersion() {
    const packageJson = require('../package.json');
    console.log(`Strict-ProLang v${packageJson.version}`);
    console.log('Every Instruction Counts — Safe, Contextual, Base-12 Precision');
}

function main() {
    const args = process.argv.slice(2);
    
    if (args.length === 0 || args.includes('-h') || args.includes('--help')) {
        showHelp();
        return;
    }
    
    if (args.includes('-v') || args.includes('--version')) {
        showVersion();
        return;
    }
    
    if (args.includes('-i') || args.includes('--interactive')) {
        console.log('Starting Strict-ProLang interactive mode...');
        const interpreter = new StrictProLang();
        interpreter.startRepl();
        return;
    }
    
    const compileMode = args.includes('-c') || args.includes('--compile');
    const filename = args.find(arg => !arg.startsWith('-'));
    
    if (filename) {
        if (!fs.existsSync(filename)) {
            console.error(`Error: File '${filename}' not found`);
            process.exit(1);
        }
        
        const source = fs.readFileSync(filename, 'utf8');
        const interpreter = new StrictProLang();
        
        if (compileMode) {
            console.log('Compiling...', filename);
            interpreter.compile(source);
        } else {
            console.log('Running...', filename);
            interpreter.run(source);
        }
    } else {
        console.error('Error: No input file specified');
        showHelp();
        process.exit(1);
    }
}

if (require.main === module) {
    main();
}

module.exports = { main, showHelp, showVersion };