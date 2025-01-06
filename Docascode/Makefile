# Define paths
DOXYGEN = doxygen
DOXYFILE = Doxyfile
DOXYGEN_OUTPUT = docs/xml
SPHINX_BUILD = sphinx-build
SPHINX_SOURCE = sphinx-docs/source
SPHINX_BUILD_DIR = ../docs

# Default target
all: sphinx

# Doxygen target: Run Doxygen to generate XML (only rebuild if Doxyfile changes)
doxygen: $(DOXYFILE)
	$(DOXYGEN) $(DOXYFILE)
# echo "Doxygen documentation generated in $(DOXYGEN_OUTPUT)"

# Sphinx target: Run Sphinx to build HTML docs (only rebuild if XML or .rst files change)
sphinx: doxygen $(SPHINX_SOURCE)/index.rst $(DOXYGEN_OUTPUT)/index.xml
	$(SPHINX_BUILD) -b html $(SPHINX_SOURCE) $(SPHINX_BUILD_DIR) && start $(SPHINX_BUILD_DIR)/index.html
# echo "Sphinx documentation generated in $(SPHINX_BUILD_DIR)"

# Clean up generated files
clean:
	del /f /q "$(DOXYGEN_OUTPUT)\*.*"
	del /f /q "$(SPHINX_BUILD_DIR)\*.*"
# echo "Cleaned up generated files."

# Rebuild everything (clean + doxygen + sphinx)
rebuild: clean all
