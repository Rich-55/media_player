#!/bin/bash

rm -rf coverage.info coverage_filtered.info coverage_report
find build -name "*.gcda" -delete
./unit_tests
lcov --rc lcov_branch_coverage=1 --capture --directory build --output-file coverage.info
lcov --rc lcov_branch_coverage=1 --extract coverage.info \
"*/src/Controller/*" \
"*/src/utils/*" \
"*/test/*" \
"*/test/utils/*" \
--output-file coverage_filtered.info
genhtml --rc lcov_branch_coverage=1 coverage_filtered.info --branch-coverage --output-directory coverage_report
firefox coverage_report/index.html
