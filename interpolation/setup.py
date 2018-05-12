#!/usr/bin/python

from distutils.core import setup, Extension

setup(name="radio_interface", version="1.0", ext_modules=[Extension("extract_packet", ["radio_interface.c"])])
