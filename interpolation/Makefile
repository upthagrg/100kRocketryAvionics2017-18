TARGET	= radio_interface

.PHONY: clean

all: $(TARGET).so build lib

$(TARGET).so: $(TARGET).c
	gcc -shared -Wl,-soname,$(TARGET) -o $(TARGET).so -fPIC $(TARGET).c conversion.c -I/usr/include/python2.7
	python setup.py install --prefix=.

clean:
	rm -r *.so build/* lib/*
