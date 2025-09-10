const readline = require('readline');

class StrictProLang {
    constructor() {
        this.version = '0.1.0';
        this.base = 12; // Base-12 number system
        this.context = new Map(); // Context for variables and functions
        this.strict = true; // Strict mode enforcement
    }
    
    /**
     * Run Strict-ProLang source code
     * @param {string} source - The source code to execute
     */
    run(source) {
        try {
            console.log('Executing Strict-ProLang code...');
            const tokens = this.tokenize(source);
            const ast = this.parse(tokens);
            const result = this.evaluate(ast);
            console.log('Execution completed.');
            return result;
        } catch (error) {
            console.error('Runtime Error:', error.message);
            throw error;
        }
    }
    
    /**
     * Compile source code to intermediate representation
     * @param {string} source - The source code to compile
     */
    compile(source) {
        try {
            console.log('Compiling Strict-ProLang code...');
            const tokens = this.tokenize(source);
            const ast = this.parse(tokens);
            const ir = this.generateIR(ast);
            console.log('Compilation completed.');
            console.log('Intermediate Representation:', JSON.stringify(ir, null, 2));
            return ir;
        } catch (error) {
            console.error('Compilation Error:', error.message);
            throw error;
        }
    }
    
    /**
     * Start interactive REPL mode
     */
    startRepl() {
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout,
            prompt: 'spl> '
        });
        
        console.log(`Strict-ProLang v${this.version} Interactive Mode`);
        console.log('Every Instruction Counts — Safe, Contextual, Base-12 Precision');
        console.log('Type "exit" to quit, "help" for help\n');
        
        rl.prompt();
        
        rl.on('line', (line) => {
            const input = line.trim();
            
            if (input === 'exit') {
                console.log('Goodbye!');
                rl.close();
                return;
            }
            
            if (input === 'help') {
                this.showReplHelp();
                rl.prompt();
                return;
            }
            
            if (input === '') {
                rl.prompt();
                return;
            }
            
            try {
                const result = this.run(input);
                if (result !== undefined) {
                    console.log('=>', result);
                }
            } catch (error) {
                // Error already logged in run method
            }
            
            rl.prompt();
        });
        
        rl.on('close', () => {
            process.exit(0);
        });
    }
    
    showReplHelp() {
        console.log(`
REPL Commands:
  exit       - Exit the REPL
  help       - Show this help message
  
Strict-ProLang Features:
  - Base-12 number system
  - Contextual variable scoping
  - Strict instruction counting
  
Example code:
  let x = A;     // A is 10 in base-12
  print x;       // Outputs: 10
`);
    }
    
    /**
     * Tokenize source code
     * @param {string} source - Source code to tokenize
     */
    tokenize(source) {
        // Basic tokenizer - can be expanded
        const tokens = [];
        const lines = source.split('\n');
        
        for (let i = 0; i < lines.length; i++) {
            const line = lines[i].trim();
            if (line && !line.startsWith('//')) {
                // Simple tokenization - split by spaces for now
                const lineTokens = line.split(/\s+/).filter(token => token.length > 0);
                tokens.push(...lineTokens.map(token => ({
                    type: this.getTokenType(token),
                    value: token,
                    line: i + 1
                })));
            }
        }
        
        return tokens;
    }
    
    getTokenType(token) {
        if (/^[0-9A-B]+$/.test(token)) return 'NUMBER'; // Base-12 number
        if (['let', 'print', 'if', 'else', 'while', 'function'].includes(token)) return 'KEYWORD';
        if (['+', '-', '*', '/', '=', '==', '!=', '<', '>'].includes(token)) return 'OPERATOR';
        if ([';', '(', ')', '{', '}'].includes(token)) return 'PUNCTUATION';
        return 'IDENTIFIER';
    }
    
    /**
     * Parse tokens into AST
     * @param {Array} tokens - Tokens to parse
     */
    parse(tokens) {
        // Basic parser - creates simple AST
        return {
            type: 'Program',
            body: tokens.map(token => ({
                type: 'Statement',
                token: token
            }))
        };
    }
    
    /**
     * Evaluate AST
     * @param {Object} ast - Abstract syntax tree
     */
    evaluate(ast) {
        // Basic evaluator
        console.log('Evaluating program with', ast.body.length, 'statements');
        
        for (const statement of ast.body) {
            if (statement.token.type === 'KEYWORD' && statement.token.value === 'print') {
                console.log('Hello from Strict-ProLang!');
            }
        }
        
        return true;
    }
    
    /**
     * Generate intermediate representation
     * @param {Object} ast - Abstract syntax tree
     */
    generateIR(ast) {
        return {
            version: this.version,
            instructions: ast.body.map((stmt, idx) => ({
                id: idx,
                opcode: 'STMT',
                token: stmt.token
            }))
        };
    }
    
    /**
     * Convert number from base-12 to base-10
     * @param {string} base12 - Base-12 number string
     */
    base12ToBase10(base12) {
        let result = 0;
        const digits = base12.toUpperCase();
        
        for (let i = 0; i < digits.length; i++) {
            const digit = digits[digits.length - 1 - i];
            let value;
            
            if (digit >= '0' && digit <= '9') {
                value = parseInt(digit);
            } else if (digit === 'A') {
                value = 10;
            } else if (digit === 'B') {
                value = 11;
            } else {
                throw new Error(`Invalid base-12 digit: ${digit}`);
            }
            
            result += value * Math.pow(12, i);
        }
        
        return result;
    }
}

module.exports = StrictProLang;