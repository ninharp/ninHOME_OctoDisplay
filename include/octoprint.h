/**
 * Project: ninHOME_OctoDisplay
 * @file octoprint.h
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 18.04.2018
 *
 * Structures for Octoprint Values
 *
 */
#ifndef INCLUDE_OCTOPRINT_H_
#define INCLUDE_OCTOPRINT_H_

/* Octoprint Values structures */
struct octoprint_temperature_t {
	float actual;	// Actual Temperature
	float target;	// Target Temperature
};

struct octoprint_progress_printing_t {
	String location;
	String path;
};

struct octoprint_progress_slicing_t {
	String source_location;
	String source_path;
	String dest_location;
	String dest_path;
	String slicer;
};

struct octoprint_progress_t {
	uint8_t progress;
	octoprint_progress_printing_t printing;
	octoprint_progress_slicing_t slicing;
};

struct octoprint_event_printstarted_t {
#ifdef OCTOPRINT_VER13
	String name;
	String path;
#else
	String file;
	String filename;
#endif
	String origin;
};

struct octoprint_event_connected_t {
	String port;
	String baudrate;
};

struct octoprint_events_t {
	octoprint_event_printstarted_t PrintStarted; // Print started event
	octoprint_event_connected_t Connected; // Printer Connected event
	String lastEvent; // Last Event received
};

struct octoprint_t {
	uint8_t num_extruders = OCTOPRINT_NUM_EXTRUDERS;
	uint32_t _timestamp; // Last Timestamp of message received from Octoprint
	octoprint_temperature_t temperature[OCTOPRINT_NUM_EXTRUDERS+1]; // Temperatures Nozzles + Bed
	octoprint_progress_t progress; // Printing progress
	octoprint_events_t events; // Events from octoprint
	bool connected; // Octoprint Connected? (LWT)
} octoprintValues;

//struct octoprint_display_t {
//} octoprintDisplay;

#endif /* INCLUDE_OCTOPRINT_H_ */
