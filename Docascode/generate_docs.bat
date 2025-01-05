@echo off

doxygen Doxyfile
cd sphinx-docs
sphinx-build -b html source build

echo Documentation generated in %cd%\docs

pause
