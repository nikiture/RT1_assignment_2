# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import subprocess
import sys
sys.path.insert(0, os.path.abspath ('../'))

subprocess.call ('doxygen Doxyfile.in', shell = True)
show_authors = True

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information



project = 'py_RS1_assignment2'
copyright = '2024, Torre Nicolò'
author = 'Torre Nicolò'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinx.ext.autodoc', 'breathe']

highlight_language = 'c++'
source_suffix = '.rst'
master_doc = 'index'
#html_theme = 'sphinx_rtd_theme'

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

#--- --
breathe_projects = {
	"py_RS1_assignment2": "../build/xml"
}
breathe_default_project = "py_RS1_assignment2"
breathe_default_members = ('members', 'undoc-members')
# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']
