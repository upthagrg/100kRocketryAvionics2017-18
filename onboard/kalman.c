#include <stdint.h>
#include "matrix.h"
#include "kalman.h"
#include "hwint.h"
#ifdef DEBUG
#include "debug.h"
#endif

struct filter kf;

int8_t kalman_filter(void)
{
	kf_init();

	do kf_main_loop(); while (1);

	return 0;
}

int8_t kf_init(void)
{
	float s_m [NUM_VARS]  [1];			/* state matrix */
	float m_m [NUM_INPUTS][1];			/* measurement matrix */
	float p_m [NUM_VARS]  [1];			/* prediction matrix */
	float u_m [NUM_VARS]  [NUM_VARS];	/* uncertainty matrix */
	float up_m[NUM_VARS]  [NUM_VARS];	/* uncertainty prediction matrix */
	float w_m [NUM_VARS]  [NUM_INPUTS];	/* weights matrix */
	float pc_m[NUM_VARS]  [NUM_VARS];	/* process covariance matrix */
	float oc_m[NUM_INPUTS][NUM_INPUTS];	/* observation covariance matrix */

	float pm_m[NUM_VARS][NUM_VARS] =	/* prediction model matrix */
		/* Populate with values from simulation */
		{
			{1,0,0,0,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0,0,0},
			{0,0,1,0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,0,0,1,0,0,0},
			{0,0,0,0,0,0,0,1,0,0},
			{0,0,0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,0,0,0,1}
		};
	float om_m[NUM_INPUTS][NUM_VARS] =	/* observation model matrix */
	/* Populate with values from simulation */
		{
			{1,0,0,0,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0,0,0},
			{0,0,1,0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0}
		};

	new_matrix(&kf.state,					NUM_VARS,   1,          s_m);
	new_matrix(&kf.measurement,				NUM_INPUTS, 1,          m_m);
	new_matrix(&kf.prediction,				NUM_VARS,   1,          p_m);
	new_matrix(&kf.prediction_model,		NUM_VARS,   NUM_VARS,   pm_m);
	new_matrix(&kf.observation_model,		NUM_INPUTS, NUM_VARS,   om_m);
	new_matrix(&kf.uncertainty,				NUM_VARS,   NUM_VARS,   u_m);
	new_matrix(&kf.uncertainty_prediction,	NUM_VARS,   NUM_VARS,   up_m);
	new_matrix(&kf.weights,					NUM_VARS,   NUM_INPUTS, w_m);
	new_matrix(&kf.process_covariance,		NUM_VARS,   NUM_VARS,   pc_m);
	new_matrix(&kf.observation_covariance,	NUM_INPUTS, NUM_INPUTS, oc_m);

	kf_initial_state();

	return 0;
}

int8_t kf_initial_state(void)
{
	uint32_t i = 0;

	kf_measure();

	for (; i < NUM_INPUTS; i++) kfstate(i) = kfmeasurement(i);
	for (; i < NUM_VARS;   i++) kfstate(i) = 0;

	return 0;
}

int8_t kf_main_loop(void)
{
	kf_measure();
	kf_predict_state();
	kf_predict_uncertainty();
	kf_calculate_weights();
	kf_update_state();
	kf_update_uncertainty();
	return 0;
}

int8_t kf_measure(void)
{
#ifdef DEBUG
	size_t buff_size = 64;
	char *buff = NULL;
	if (getline(&buff, &buff_size, debug_input) == -1) exit(0);
	_hwsim_td = structure(&buff);
	free(buff);
	printf("{%f, %f, %f, %f}\n", _hwsim_td.time, _hwsim_td.lat, _hwsim_td.lon, _hwsim_td.alt);

#endif
	kfmeasurement(TIME)			= cpu_time();
	kfmeasurement(LATITUDE)		= gps_latitude();
	kfmeasurement(LONGITUDE)	= gps_longitude();
	kfmeasurement(ALTITUDE)		= gps_altitude();
	kfmeasurement(PRESSURE)		= baro_pressure();
	kfmeasurement(ACCELERATION)	= accel_acceleration();

	return 0;
}

/*
  prediction = prediction_model * state
 */
int8_t kf_predict_state(void)
{
	matrix_multiply(kf.prediction_model, kf.state, &kf.prediction);

	return 0;
}

/*
  uncertainty_prediction = prediction_model * uncertainty * prediction_model^T
	+ process_covariance
 */
int8_t kf_predict_uncertainty(void)
{
	float t_m[kf.prediction_model.cols][kf.prediction_model.rows];
	Matrix trans;
	new_matrix(&trans, kf.prediction_model.cols, kf.prediction_model.rows, t_m);

	matrix_transpose(kf.prediction_model, &trans);
	matrix_multiply (kf.prediction_model, kf.uncertainty, &kf.uncertainty_prediction);
	matrix_multiply (kf.uncertainty_prediction, trans, &kf.uncertainty_prediction);
	matrix_add      (kf.uncertainty_prediction, kf.process_covariance, &kf.uncertainty_prediction);

	return 0;
}

/*
  weights = uncertainty_prediction * observation_model^T * (observation_model
	* uncertainty_prediction * observation_model^T + observation_covariance)^-1
 */
int8_t kf_calculate_weights(void)
{
	float tr_m [kf.observation_model.cols]     [kf.observation_model.rows];
	float r_m  [kf.uncertainty_prediction.rows][kf.observation_model.rows];
	float tmp_m[kf.observation_model.rows]     [kf.observation_model.rows];
	Matrix trans, result, temp;
	new_matrix(&trans,	kf.observation_model.cols,		kf.observation_model.rows, tr_m);
	new_matrix(&result,	kf.uncertainty_prediction.rows,	kf.observation_model.rows, r_m);
	new_matrix(&temp,	kf.observation_model.rows,		kf.observation_model.rows, tmp_m);
	
	matrix_transpose(kf.observation_model, &trans);
	matrix_multiply (kf.uncertainty_prediction, trans, &result);
	matrix_multiply (kf.observation_model, result, &temp);
	matrix_add      (temp, kf.observation_covariance, &temp);
	matrix_inverse  (temp, &temp);
	matrix_multiply (result, temp, &kf.weights);

	return 0;
}

/*
  state = prediction + weights * (measurement - observation_model * prediction)
 */
int8_t kf_update_state(void)
{
	float t_m[kf.measurement.rows][kf.measurement.cols];
	Matrix temp;
	new_matrix(&temp, kf.measurement.rows, kf.measurement.cols, t_m);

	matrix_multiply(kf.observation_model, kf.prediction, &temp);
	matrix_scale   (&temp, -1);
	matrix_add     (kf.measurement, temp, &temp);
	matrix_multiply(kf.weights, temp, &kf.state);
	matrix_add     (kf.prediction, kf.state, &kf.state);
#ifdef DEBUG
	kf_print_state();
#endif

	return 0;
}

/*
  uncertainty = uncertainty_prediction - weights * observation_model
	* uncertainty_prediction
 */
int8_t kf_update_uncertainty(void)
{
	float i_m[kf.weights.rows][kf.observation_model.cols];
	float t_m[kf.weights.rows][kf.observation_model.cols];
	Matrix identity, temp;
	new_matrix(&identity,	kf.weights.rows, kf.observation_model.cols, i_m);
	new_matrix(&temp,		kf.weights.rows, kf.observation_model.cols, t_m);
	identity_matrix(&identity);

	matrix_multiply(kf.weights, kf.observation_model, &temp);
	matrix_scale   (&temp, -1);
	matrix_add     (identity, temp, &temp);
	matrix_multiply(temp, kf.uncertainty_prediction, &kf.uncertainty);

	return 0;
}

float kf_get_value(enum sensor_var index)
{
	return kfstate(index);
}

#ifdef DEBUG
void kf_print_state(void)
{
	printf("TIME:\t\t%f\n", kf_get_value(TIME));
	printf("LATITUDE:\t%f\n", kf_get_value(LATITUDE));
	printf("LONGITUDE:\t%f\n", kf_get_value(LONGITUDE));
	printf("ALTITUDE:\t%f\n", kf_get_value(ALTITUDE));
	printf("ACCELERATION:\t%f\n", kf_get_value(ACCELERATION));
}
#endif
