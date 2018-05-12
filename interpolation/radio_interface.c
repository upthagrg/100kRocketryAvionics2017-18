#include <stdlib.h>
#include <Python.h>
#include "transmit.h"
#include "conversion.h"

FILE *address;
#define FIFO ""

static PyObject * seed_random(PyObject *self, PyObject *args)
{
	srand((unsigned)time(NULL));
	address = fopen(FIFO, "r");
	return Py_BuildValue("");
}

Packet * td_convert_packet(struct telem_data td)
{
	Packet *packet = malloc(sizeof(Packet));
	packet->time = td.time;
	packet->latitude = td.lat;
	packet->longitude = td.lon;
	packet->altitude = td.alt;
	packet->speed = td.vel;
	return packet;
}

Packet * retrieve_packet(void *address)
{
	struct telem_data td;
	size_t buff_size = 64;
	char *buff = NULL;
	if (getline(&buff, &buff_size, (FILE *)address) == -1) exit(0);
	td = structure(&buff);
	free(buff);
	
	return td_convert_packet(td);
}

static PyObject * extract_packet(PyObject *self, PyObject *args)
{
	Packet *packet;

	do {
		packet = retrieve_packet(address);
	} while (packet == NULL);
	
	PyObject *ret = Py_BuildValue("(i,f,f,f,f)", packet->time, packet->latitude, packet->longitude, packet->altitude, packet->speed);

	free(packet);
	return ret;
}

static PyMethodDef interface_functions[] = {
    {"extract_packet", (PyCFunction)extract_packet, METH_VARARGS, ""},
    {"seed_random", (PyCFunction)seed_random, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initradio_interface(void){
    Py_InitModule("radio_interface", interface_functions);
}
