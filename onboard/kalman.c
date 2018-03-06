#include <stdint.h>
#include "matrix.h"
#include "kalman.h"
#include "hwint.h"

int8_t kalman_filter(void)
{
	struct filter kf;

	float s_m [NUM_VARS]  [1];		    /* state matrix */
	float m_m [NUM_INPUTS][1];			/* measurement matrix */
	float p_m [NUM_VARS]  [1];			/* prediction matrix */
	float pm_m[NUM_VARS]  [NUM_VARS];	/* prediction model matrix */
	float om_m[NUM_INPUTS][NUM_VARS];	/* observation model matrix */
	float u_m [NUM_VARS]  [NUM_VARS];	/* uncertainty matrix */
	float up_m[NUM_VARS]  [NUM_VARS];   /* uncertainty prediction matrix */
	float w_m [NUM_VARS]  [NUM_INPUTS];	/* weights matrix */

	new_matrix(&kf.state,					NUM_VARS,   1,          s_m);
	new_matrix(&kf.measurement,				NUM_INPUTS, 1,          m_m);
	new_matrix(&kf.prediction,				NUM_VARS,   1,          p_m);
	new_matrix(&kf.prediction_model,		NUM_VARS,   NUM_VARS,   pm_m);
	new_matrix(&kf.observation_model,		NUM_INPUTS, NUM_VARS,   om_m);
	new_matrix(&kf.uncertainty,				NUM_VARS,   NUM_VARS,   u_m);
	new_matrix(&kf.uncertainty_prediction,	NUM_VARS,   NUM_VARS,   up_m);
	new_matrix(&kf.weights,					NUM_VARS,   NUM_INPUTS, w_m);
	
	kf_init(&kf);

	do {
		kf_predict_state(&kf);
		kf_predict_uncertainty(&kf);
		kf_measure(&kf);
		kf_calculate_weights(&kf);
		kf_update_state(&kf);
		kf_update_uncertainty(&kf);
	} while (1);
	
	return 0;
}

void kf_init(struct filter *kf)
{
	float **s = kf->state.matrix;
	float **m = kf->measurement.matrix;

	kf_measure(kf);

	s[LATITUDE][0] = m[LATITUDE][0];
	s[LONGITUDE][0] = m[LONGITUDE][0];
	s[ALTITUDE][0] = m[ALTITUDE][0];
	s[VELOCITY][0] = 0;
	s[ACCELERATION][0] = 0;
}

void kf_predict_state(struct filter *kf)
{
	matrix_multiply(kf->prediction_model, kf->state, &kf->prediction);
}

void kf_predict_uncertainty(struct filter *kf)
{
	float t_m[kf->prediction_model.cols][kf->prediction_model.rows];
	Matrix trans, Q;
	new_matrix(&trans, kf->prediction_model.cols, kf->prediction_model.rows,
	           t_m);
	
	matrix_transpose(kf->prediction_model, &trans);
	matrix_multiply(kf->prediction_model, kf->uncertainty,
	                &kf->uncertainty_prediction);
	matrix_multiply(kf->uncertainty_prediction, trans,
	                &kf->uncertainty_prediction);
	matrix_add(kf->uncertainty_prediction, Q, &kf->uncertainty_prediction); /* TODO: Calculate Q */
}

void kf_measure(struct filter *kf)
{
	float **m = kf->measurement.matrix;

	m[LATITUDE][0] = gps_latitude();
}

void kf_calculate_weights(struct filter *kf)
{
	float tr_m[kf->observation_model.cols][kf->observation_model.rows];
	float tmp_m[kf->uncertainty_prediction.rows][kf->observation_model.rows];
	Matrix trans, temp, R;
	new_matrix(&trans, kf->observation_model.cols, kf->observation_model.rows,
	           tr_m);
	new_matrix(&temp, kf->uncertainty_prediction.rows,
	           kf->observation_model.rows, tmp_m);
	
	matrix_transpose(kf->observation_model, &trans);
	matrix_multiply(kf->uncertainty_prediction, trans, &temp);
	matrix_multiply(kf->observation_model, temp, &kf->weights);
	matrix_add(kf->weights, R, &kf->weights); /* TODO: Calculate R */
	matrix_inverse(kf->weights, &kf->weights);
	matrix_multiply(temp, kf->weights, &kf->weights);
}

void kf_update_state(struct filter *kf)
{
	float t_m[kf->measurement.rows][kf->measurement.cols];
	Matrix temp;
	new_matrix(&temp, kf->measurement.rows, kf->measurement.cols, t_m);

	matrix_multiply(kf->observation_model, kf->prediction, &temp);
	matrix_scale(temp, -1, &temp);
	matrix_add(kf->measurement, temp, &temp);
	matrix_multiply(kf->weights, temp, &kf->state);
	matrix_add(kf->prediction, kf->state, &kf->state);
}

void kf_update_uncertainty(struct filter *kf)
{
	float i_m[kf->weights.rows][kf->observation_model.cols];
	float t_m[kf->weights.rows][kf->observation_model.cols];
	Matrix identity, temp;
	new_matrix(&identity, kf->weights.rows, kf->observation_model.cols, i_m);
	new_matrix(&temp, kf->weights.rows, kf->observation_model.cols, t_m);
	identity_matrix(&identity);

	matrix_multiply(kf->weights, kf->observation_model, &temp);
	matrix_scale(temp, -1, &temp);
	matrix_add(identity, temp, &temp);
	matrix_multiply(temp, kf->uncertainty_prediction, &kf->uncertainty);
}
