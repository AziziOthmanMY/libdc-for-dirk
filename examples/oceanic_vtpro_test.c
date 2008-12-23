/* 
 * libdivecomputer
 * 
 * Copyright (C) 2008 Jef Driesen
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <stdio.h>	// fopen, fwrite, fclose

#include "oceanic_vtpro.h"
#include "utils.h"

#define WARNING(expr) \
{ \
	message ("%s:%d: %s\n", __FILE__, __LINE__, expr); \
}

device_status_t
test_dump_memory (const char* name, const char* filename)
{
	unsigned char data[OCEANIC_VTPRO_MEMORY_SIZE] = {0};
	device_t *device = NULL;

	message ("oceanic_vtpro_device_open\n");
	device_status_t rc = oceanic_vtpro_device_open (&device, name);
	if (rc != DEVICE_STATUS_SUCCESS) {
		WARNING ("Error opening serial port.");
		return rc;
	}

	message ("device_version\n");
	unsigned char version[OCEANIC_VTPRO_PACKET_SIZE] = {0};
	rc = device_version (device, version, sizeof (version));
	if (rc != DEVICE_STATUS_SUCCESS) {
		WARNING ("Cannot identify computer.");
		device_close (device);
		return rc;
	}

	message ("oceanic_vtpro_device_calibrate\n");
	unsigned char calibration = 0;
	rc = oceanic_vtpro_device_calibrate (device, &calibration, 1);
	if (rc != DEVICE_STATUS_SUCCESS) {
		WARNING ("Cannot calibrate computer.");
		device_close (device);
		return rc;
	}

	message ("device_read\n");
	rc = device_read (device, 0x00, data, sizeof (data));
	if (rc != DEVICE_STATUS_SUCCESS) {
		WARNING ("Cannot read memory.");
		device_close (device);
		return rc;
	}

	message ("Dumping data\n");
	FILE* fp = fopen (filename, "wb");
	if (fp != NULL) {
		fwrite (data, sizeof (unsigned char), sizeof (data), fp);
		fclose (fp);
	}

	message ("device_close\n");
	rc = device_close (device);
	if (rc != DEVICE_STATUS_SUCCESS) {
		WARNING ("Cannot close device.");
		return rc;
	}

	return DEVICE_STATUS_SUCCESS;
}


const char*
errmsg (device_status_t rc)
{
	switch (rc) {
	case DEVICE_STATUS_SUCCESS:
		return "Success";
	case DEVICE_STATUS_UNSUPPORTED:
		return "Unsupported operation";
	case DEVICE_STATUS_TYPE_MISMATCH:
		return "Device type mismatch";
	case DEVICE_STATUS_ERROR:
		return "Generic error";
	case DEVICE_STATUS_IO:
		return "Input/output error";
	case DEVICE_STATUS_MEMORY:
		return "Memory error";
	case DEVICE_STATUS_PROTOCOL:
		return "Protocol error";
	case DEVICE_STATUS_TIMEOUT:
		return "Timeout";
	default:
		return "Unknown error";
	}
}

int main(int argc, char *argv[])
{
	message_set_logfile ("VTPRO.LOG");

#ifdef _WIN32
	const char* name = "COM1";
#else
	const char* name = "/dev/ttyS0";
#endif

	if (argc > 1) {
		name = argv[1];
	}

	message ("DEVICE=%s\n", name);

	device_status_t a = test_dump_memory (name, "VTPRO.DMP");

	message ("\nSUMMARY\n");
	message ("-------\n");
	message ("test_dump_memory: %s\n", errmsg (a));

	message_set_logfile (NULL);

	return 0;
}