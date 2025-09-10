const StrictProLang = require('../src/index.js');
const fs = require('fs');
const path = require('path');

function runTests() {
    console.log('Running Strict-ProLang Tests...\n');
    
    let passed = 0;
    let failed = 0;
    
    // Test 1: Basic instantiation
    try {
        const spl = new StrictProLang();
        console.log('✓ Test 1: Basic instantiation - PASSED');
        passed++;
    } catch (error) {
        console.log('✗ Test 1: Basic instantiation - FAILED:', error.message);
        failed++;
    }
    
    // Test 2: Base-12 conversion
    try {
        const spl = new StrictProLang();
        const result = spl.base12ToBase10('A');
        if (result === 10) {
            console.log('✓ Test 2: Base-12 conversion (A = 10) - PASSED');
            passed++;
        } else {
            throw new Error(`Expected 10, got ${result}`);
        }
    } catch (error) {
        console.log('✗ Test 2: Base-12 conversion - FAILED:', error.message);
        failed++;
    }
    
    // Test 3: Base-12 conversion advanced
    try {
        const spl = new StrictProLang();
        const result = spl.base12ToBase10('1B');
        if (result === 23) { // 1*12 + 11 = 23
            console.log('✓ Test 3: Base-12 conversion (1B = 23) - PASSED');
            passed++;
        } else {
            throw new Error(`Expected 23, got ${result}`);
        }
    } catch (error) {
        console.log('✗ Test 3: Base-12 conversion advanced - FAILED:', error.message);
        failed++;
    }
    
    // Test 4: Tokenization
    try {
        const spl = new StrictProLang();
        const tokens = spl.tokenize('let x = A');
        if (tokens.length === 4) {
            console.log('✓ Test 4: Basic tokenization - PASSED');
            passed++;
        } else {
            throw new Error(`Expected 4 tokens, got ${tokens.length}`);
        }
    } catch (error) {
        console.log('✗ Test 4: Basic tokenization - FAILED:', error.message);
        failed++;
    }
    
    // Test 5: Simple program execution
    try {
        const spl = new StrictProLang();
        const result = spl.run('print');
        console.log('✓ Test 5: Simple program execution - PASSED');
        passed++;
    } catch (error) {
        console.log('✗ Test 5: Simple program execution - FAILED:', error.message);
        failed++;
    }
    
    // Test 6: Compilation
    try {
        const spl = new StrictProLang();
        const ir = spl.compile('let x = 5');
        if (ir && ir.instructions) {
            console.log('✓ Test 6: Basic compilation - PASSED');
            passed++;
        } else {
            throw new Error('No intermediate representation generated');
        }
    } catch (error) {
        console.log('✗ Test 6: Basic compilation - FAILED:', error.message);
        failed++;
    }
    
    // Summary
    console.log(`\n--- Test Results ---`);
    console.log(`Passed: ${passed}`);
    console.log(`Failed: ${failed}`);
    console.log(`Total: ${passed + failed}`);
    
    if (failed === 0) {
        console.log('\n🎉 All tests passed!');
        process.exit(0);
    } else {
        console.log('\n❌ Some tests failed.');
        process.exit(1);
    }
}

if (require.main === module) {
    runTests();
}

module.exports = { runTests };