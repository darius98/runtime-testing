#! /usr/bin/env python3.7

import subprocess

proc = None
try:
    proc = subprocess.run("./core_single_test_timeout",
                          timeout=10, capture_output=True)
except TimeoutError:
    print("Test did not finish in 10 seconds")
    exit(1)

if proc.returncode != 1:
    print("Test did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[F] TestCase::test" not in proc.stdout.decode() or "\tTest execution timed out." not in proc.stdout.decode():
    print("Test did not correctly report exit of timeout test")
    print("Output:")
    print(proc.stdout.decode())
    exit(1)
