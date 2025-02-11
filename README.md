MCU module’s mock project - Media Player App

Visit docascode for this project: https://media-player-git-docascode-rich-55s-projects.vercel.app/
Please connect board KL46Z when start application, (baud 115200)
To build the app on Linux in first time:
+ cd to build directory: PlayerMediaApp
+ mkdir build
+ cmake -B build
+ cmake --build build -j$(nproc)
+ make For best experience, you should maximize terminal and set scale to 80% or 90%. To run app
+ cd to PlayerMediaApp/
+ ./PlayerMediaApp
To build after change code:
+ rm -rf build/
+ cmake -B build
+ cmake --build build -j$(nproc)
+ make For best experience, you should maximize terminal and set scale to 80% or 90%. To run app
+ cd to PlayerMediaApp/
+ ./PlayerMediaApp
Run unit test:
+ ./unit_tests
Build lcov:
+ rm -rf coverage.info coverage_filtered.info coverage_report
+ find build -name "*.gcda" -delete
+ ./unit_tests
+ lcov --rc lcov_branch_coverage=1 --capture --directory build --output-file coverage.info
+ lcov --rc lcov_branch_coverage=1 --extract coverage.info \
"*/src/Controller/*" \
"*/src/utils/*" \
"*/test/*" \
"*/test/utils/*" \
+ --output-file coverage_filtered.info
+ genhtml --rc lcov_branch_coverage=1 coverage_filtered.info --branch-coverage --output-directory coverage_report
+ firefox coverage_report/index.html
To run unit test, you must connect board KL46Z and fix path of USB in MediaScannerControllerTest and MediaPlaylistControllerTest

