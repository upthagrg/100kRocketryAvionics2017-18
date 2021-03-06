#ifndef KALMAN_H
#define KALMAN_H

#include <stdint.h>
#include "matrix.h"

#define NUM_INPUTS 6
#define NUM_VARS 10

enum sensor_var { TIME, LATITUDE, LONGITUDE, ALTITUDE, PRESSURE, ACCELERATION, YAW, PITCH, ROLL, SPEED };

struct filter {
	Matrix state, measurement, prediction, prediction_model, observation_model, uncertainty, uncertainty_prediction, weights, process_covariance, observation_covariance;
};

/*
  Macros to access the state or measurement variable `v'
  Can be used to get or set the value
 */
#define kfstate(v) ((float (*)[kf.state.cols])kf.state.matrix)[v][0]
#define kfmeasurement(v) ((float (*)[kf.measurement.cols])kf.measurement.matrix)[v][0]

int8_t kalman_filter(void);

int8_t kf_init(void);

int8_t kf_main_loop(void);

int8_t kf_predict_state(void);

int8_t kf_predict_uncertainty(void);

int8_t kf_measure(void);

int8_t kf_calculate_weights(void);

int8_t kf_update_state(void);

int8_t kf_update_uncertainty(void);

float kf_get_value(enum sensor_var);

#ifdef DEBUG
void kf_print_state(void);
#endif

#endif
