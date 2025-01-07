import os

project = 'Media Player'
copyright = '2025 - BaoL2, HuyDN34, GiauPVT'
author = 'BHG'

# -- General configuration ---------------------------------------------------

extensions = [
    'breathe',
    'sphinxcontrib.plantuml', 
]

breathe_projects = {
    "Media Player": "../../docs/xml"
}
breathe_default_project = "Media Player"


templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------

html_theme = "sphinx_rtd_theme"
html_static_path = ['_static']
html_css_files = [
    'custom.css',  # Add your custom CSS file here
]

plantuml = f'java -jar {os.path.join(os.path.dirname(__file__), "plantuml.jar")}'
