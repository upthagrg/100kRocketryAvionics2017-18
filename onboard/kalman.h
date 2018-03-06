#ifndef KALMAN_H
#define KALMAN_H

#include <stdint.h>
#include "matrix.h"

#define NUM_INPUTS 7
#define NUM_VARS 9

enum sensor_vars {TIME, LATITUDE, LONGITUDE, ALTITUDE, PRESSURE, ACCELERATION, VELOCITY};

struct filter {
	Matrix state, measurement, prediction, prediction_model, observation_model, uncertainty, uncertainty_prediction, weights;
};

int8_t kalman_filter(void);

void kf_init(struct filter *);

void kf_predict_state(struct filter *);

void kf_predict_uncertainty(struct filter *);

void kf_measure(struct filter *);

void kf_calculate_weights(struct filter *);

void kf_update_state(struct filter *);

void kf_update_uncertainty(struct filter *);

#endif
