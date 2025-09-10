# strictc/tests/run_tests.sh
#!/bin/bash
set -e
../strictc ../examples/hello.strict
grep "echo" out.dgm
