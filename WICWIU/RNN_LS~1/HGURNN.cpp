#ifdef WORKING

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "HGUUtility.h"
#include "HGULongArray.h"
#include "HGULayerConfig.h"
#include "HGUConnectionLayer.h"
#include "HGURNN.h"


#include "HGUClassInfo.h"
#include "HGUNeuralNetConfig.h"
#include "HGUNeuralNet.h"

#ifdef __CUDA__
#include <cuda_runtime.h>
#endif // __CUDA__

int HGURNN::Alloc(int inputDim, int hiddenDim, int outputDim, int actFun, int hiddenActFun)
{
	if (m_aWeight != NULL)
		Delete();

	m_inputWidth = inputDim;
	m_inputHeight = 1;
	m_inputSize = inputDim;
	m_inDim = inputDim;

	m_outputWidth = outputDim;
	m_outputHeight = 1;
	m_outputSize = outputDim;
	m_outDim = outputDim;

	m_hiddenDim = hiddenDim;

	m_actFun = actFun;
	m_hiddenActFun = hiddenActFun;

	m_deltaDim = 0;

	m_aWeightIndex[WT_InputToHidden] = 0;

	m_weightDim = inputDim  * hiddenDim;			// input to hidden	(no bias)
	m_aWeightIndex[WT_HiddenToHidden] = m_weightDim;

	m_weightDim += (hiddenDim + 1) * hiddenDim;		// hidden to hidden
	m_aWeightIndex[WT_HiddenToOutput] = m_weightDim;

	m_weightDim += (hiddenDim + 1) * outputDim;		// hidden to output

	if (actFun == AF_Linear)
		m_coefficientDim = m_noInputChannel * 2;

	m_parameterDim = m_weightDim + m_biasDim + m_coefficientDim;

	try {
		m_aWeight = new Float[m_parameterDim];
		m_aOutput.Alloc(m_outDim);
		m_aHidden[0].Alloc(m_hiddenDim);
		m_aHidden[0].Fill(0);
		m_aHidden[1].Alloc(m_hiddenDim);
		// m_aHidden[1].Fill(0);					// not necessary
		m_activeHidden = 0;
	} catch (...){
		printf("Failed to allocate memory in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		m_aWeight = NULL;
		return FALSE;
	}

	InitializeWeight();

	return TRUE;
}

HGULongArray<Float>* HGURNN::Step(HGULongArray<Float> *pInput)
{
	m_pInput = pInput;			// for training

	HGULongArray<Float> &prevHidden = m_aHidden[m_activeHidden];
	m_activeHidden = 1 - m_activeHidden;			// toggle current hidden and previous hidden
	HGULongArray<Float> &hidden = m_aHidden[m_activeHidden];

	// Compute hidden
	// input -> hidden
	ComputeWeightedSum(m_inDim, m_hiddenDim, pInput->GetBlock(0), &GetWeight_InputToHidden(0, 0), hidden.GetBlock(0), FALSE, TRUE);
	// hidden -> hidden
	ComputeWeightedSum(m_hiddenDim, m_hiddenDim, prevHidden.GetBlock(0), &GetWeight_HiddenToHidden(0, 0), hidden.GetBlock(0), TRUE, FALSE);
	ApplyActivationFunction(m_hiddenActFun, &hidden);

	// Compute output
	ComputeWeightedSum(m_hiddenDim, m_outDim, hidden.GetBlock(0), &GetWeight_HiddenToOutput(0, 0), m_aOutput.GetBlock(0), TRUE, TRUE);
	ApplyActivationFunction(m_actFun);

	return &m_aOutput;
}

int HGURNN::PrepareToTrain(Float *pSharedGradient)
{
	if (m_hiddenDim == 0){
		printf("RNN was not allocated! %s(%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	}
	if (m_deltaDim > 0)
		CloseTraining();

	m_deltaDim = Max(m_hiddenDim, m_outDim);		// reuse delta0 and delta for hidden and output

	try{
		m_aAccGradient = new Float[m_parameterDim];
		memset(m_aAccGradient, 0, m_parameterDim * sizeof(m_aAccGradient[0]));

		m_aDelta0.Alloc(m_deltaDim);
		m_aDelta.Alloc(m_deltaDim);
	} catch (...){
		printf("Failed to allocate memory in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		CloseTraining();
		return FALSE;
	}

	if (m_maxTrainStep == 0)
		SetMaxTrainStep(DefaultMaxTrainStep);

#ifdef OLD_CODE
	if (m_actFun == AF_SoftMax){
		if (m_aExpNet.Alloc(m_outDim, m_outputSize) == FALSE){
			printf("Failed to allocate memory in %s, (%s, %d)\n", __FUNCTION__, __FILE__, __LINE__);
			CloseTraining();

			return FALSE;
		}
	}
#endif // OLD_CODE

	return TRUE;
}

int HGURNN::SetMaxTrainStep(int maxTrainStep)
{
	if (maxTrainStep == 0){
		DeleteTrainBuffer();
		return TRUE;
	}

	if (m_maxTrainStep >= maxTrainStep){
		m_maxTrainStep = maxTrainStep;
		return TRUE;
	} else if (m_maxTrainStep > 0)
		DeleteTrainBuffer();

	try{
		m_aInputBuffer = new HGULongArray<Float>[maxTrainStep];
		m_aHiddenBuffer = new HGULongArray<Float>[maxTrainStep];
		m_aOutputBuffer = new HGULongArray<Float>[maxTrainStep];
		for (int i = 0; i < maxTrainStep; i++){
			m_aInputBuffer[i].Alloc(m_inDim);
			m_aHiddenBuffer[i].Alloc(m_hiddenDim);
			m_aOutputBuffer[i].Alloc(m_outDim);
		}
		m_maxTrainStep = maxTrainStep;
		m_trainStep = 0;
		m_stepTail = 0;
	} catch (...){
		printf("Failed to allocate memory in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		DeleteTrainBuffer();
		return FALSE;
	}

	return TRUE;
}

void HGURNN::DeleteTrainBuffer()
{
	if (m_maxTrainStep <= 0)
		return;

	if (m_aInputBuffer){
		delete[] m_aInputBuffer;
		m_aInputBuffer = NULL;
	}

	if (m_aHiddenBuffer){
		delete[] m_aHiddenBuffer;
		m_aHiddenBuffer = NULL;
	}

	if (m_aOutputBuffer){
		delete[] m_aOutputBuffer;
		m_aOutputBuffer = NULL;
	}

	m_maxTrainStep = 0;
}

void HGURNN::CloseTraining()
{
	if (m_aAccGradient){
		delete[] m_aAccGradient;
		m_aAccGradient = NULL;
	}

	m_deltaDim = 0;

	if (m_maxTrainStep > 0)
		DeleteTrainBuffer();
}

HGULongArray<Float>* HGURNN::StepForTrain(HGULongArray<Float> *pInput, HGULongArray<Float> *pDesiredOutput, float *pAccError)
{
	if (m_maxTrainStep <= 0){
		printf("To run BPTT, input and hidden buffers must be allocated. %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}

	HGULongArray<Float>* ret = Step(pInput);
	if (ret == NULL)
		return NULL;

	if (pAccError)
		*pAccError += GetError(pDesiredOutput);

	SaveCurrentStep();

	if (pDesiredOutput != NULL){
		// compute gradient from hidden to output
		ComputeDelta0(pDesiredOutput, 1.F, &m_aOutput, &m_aDelta0);		// gradient w.r.t. output
		ComputeDeltaFromDelta0(m_actFun, m_outDim, m_aOutput.GetBlock(0), m_aDelta0.GetBlock(0), m_aDelta.GetBlock(0));
		ComputeGradientFromDelta(m_hiddenDim, m_outDim, m_aDelta.GetBlock(0), GetActiveHidden()->GetBlock(0), &GetGradient_HiddenToOutput(0, 0), TRUE);

		for(int s = 0; s < m_trainStep; s++){
			if (s == 0)		// from output layer at t
				BackPropagate(m_hiddenDim, m_outDim, m_aDelta.GetBlock(0), &GetWeight_HiddenToOutput(0, 0), m_aDelta0.GetBlock(0), TRUE);
			else			// from hidden at time t+1
				BackPropagate(m_hiddenDim, m_hiddenDim, m_aDelta.GetBlock(0), &GetWeight_HiddenToHidden(0, 0), m_aDelta0.GetBlock(0), TRUE);

			ComputeDeltaFromDelta0(m_hiddenActFun, m_hiddenDim, GetPreviousHidden(s).GetBlock(0), m_aDelta0.GetBlock(0), m_aDelta.GetBlock(0));

			// gradient from input to hidden
			ComputeGradientFromDelta(m_inDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousInput(s).GetBlock(0), &GetGradient_InputToHidden(0, 0), FALSE);

			// gradient from hidden to hidden
			if (s < m_trainStep - 1)
				ComputeGradientFromDelta(m_hiddenDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousHidden(s + 1).GetBlock(0), &GetGradient_HiddenToHidden(0, 0), TRUE);
		}
	}

	return ret;
}

void HGURNN::SaveCurrentStep()
{
	if (m_maxTrainStep == 0){
		printf("Training Buffer was not allocated in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		return;
	}

	m_aInputBuffer[m_stepTail] = *m_pInput;

	m_aHiddenBuffer[m_stepTail] = *GetActiveHidden();
	if (m_aOutputBuffer != NULL)
		m_aOutputBuffer[m_stepTail] = GetOutput();

	m_stepTail = (m_stepTail + 1) % m_maxTrainStep;
	
	if (m_trainStep < m_maxTrainStep)
		m_trainStep++;
}

int HGURNN::ComputeWeightedSum(int inDim, int outDim, Float *pInput, Float *pWeight, Float *pOutput, int bBias, int bClear, int pitch)
{
	int skip = 0;
	if (pitch != 0)
		skip = pitch - inDim - (bBias ? 1 : 0);

	if (bClear)
		memset(pOutput, 0, sizeof(pOutput[0]) * outDim);

	Float *w = pWeight;
	for (int o = 0; o < outDim; o++){
		Float net = 0.F;
		for (int i = 0; i < inDim; i++)
			net += *(w++) * pInput[i];

		if (bBias)
			net += *(w++);

		w += skip;

		pOutput[o] += net;
	}

	return TRUE;
}

int HGURNN::ComputeDeltaFromDelta0(int actFun, int outDim, Float *pOutput, Float *pDelta0, Float *pDelta)
{
	if (actFun == AF_SoftMax){
#ifndef OLD_CODE
		for (int i = 0; i < m_outDim; i++){
			Float delta = 0.F;
			for (int j = 0; j < m_outDim; j++){
				if (pDelta0[j] != 0.F)
					delta += m_aOutput[j] * ((i == j ? 1.F : 0.F) - m_aOutput[i]) * pDelta0[j];
			}
			m_aDelta[i] = delta;
		}

#else // OLD_CODE
		memset(pDelta, 0, outDim * sizeof(pDelta[0]));

		for (int i = 0; i < m_outDim; i++){
			if (pDelta0[i] != 0.F){
				Float denom = Max(m_sumOfExpNet - m_aExpNet[i], 0.0001F);
				Float o = pOutput[i];
				Float delta = pDelta0[i] * o * (1 - o);		// dE/dY * dY/dA
				for (int j = 0; j < m_outDim; j++){
					if (i != j){
//						if(denom != 0)
						pDelta[j] += delta * (-m_aExpNet[j]) / denom;			// dA / dnet
					}
					else
						pDelta[j] += delta;
				}
			}
		}
#endif // OLD_CODE
	} else {
		Float *po = pOutput;
		Float *delta0 = pDelta0;
		Float *delta = pDelta;
		Float *dLimit = delta + outDim;
		for (; delta < dLimit; po++, delta0++, delta++)
			*delta = Derv_ActFunc(0, actFun, *po) * *delta0;
	}

	return TRUE;
}

int HGURNN::ComputeGradientFromDelta(int inDim, int outDim, Float *pDelta, Float *pInput, Float *pGradient, int bBias, int pitch)
{
	int skip = 0;
	if (pitch > 0)
		skip = pitch - inDim - (bBias ? 1 : 0);

	Float *g = pGradient;
	for (int o = 0; o < outDim; o++){
		for (int i = 0; i < inDim; i++)
			*(g++) += pDelta[o] * pInput[i];

		if (bBias)
			*(g++) += pDelta[o];
		g += skip;
	}

	return TRUE;
}

int HGURNN::BackPropagate(int inDim, int outDim, Float *pDelta, Float *pWeight, Float *pPrevDelta0, int bBias, int pitch)
{
	if (pitch == 0)
		pitch = inDim + (bBias ? 1 : 0);

	for (int i = 0; i < inDim; i++){
		Float delta0 = 0.F;
		Float *w = pWeight + i;
		for (int o = 0; o < outDim; o++, w += pitch)
			delta0 += *w * pDelta[o];

		pPrevDelta0[i] = delta0;
	}

	return TRUE;
}

void RNNTest()
{
	char alphabet[] = "helo$";
	int outDim = 5;

	char inputStr[] = "hello";
	char desiredStr[] = "ello$";

	char seqLen = 5;
	int inputSeq[] = { 0, 1, 2, 2, 3 };
	int desiredSeq[] = { 1, 2, 2, 3, 4 };

	HGULongArray<Float> input;
	HGULongArray<Float> desired;

	HGURNN rnn;
	rnn.Alloc(5, 4, 5);
	rnn.PrepareToTrain();

	printf("##### Training RNN\n");
	for (int epoch = 0; epoch < 1000000; epoch++){
		Float error = 0.F;
		rnn.ResetTrainStep();
		for (int i = 0; i < seqLen; i++){
			input.MakeOneHotVector(inputSeq[i], outDim);
			desired.MakeOneHotVector(desiredSeq[i], outDim);

			rnn.StepForTrain(&input, &desired, &error);
		}

		rnn.UpdateWeightsWithGradient(0.01F);
		if ((epoch + 1) % 10000 == 0){
			printf("epoch %5d, Average error = %f\n", epoch + 1, error / seqLen);
			fflush(stdout);
		}
		if (error / seqLen < 0.00001F){
			printf("Training done, epoch = %d, error = %f\n", epoch + 1, error / seqLen);
			break;
		}
	}
	rnn.CloseTraining();

	printf("##### Testing RNN\n");
	rnn.ResetHidden();
	for (int i = 0; i < seqLen; i++){
		input.MakeOneHotVector(inputSeq[i], outDim);
		HGULongArray<Float> *pOutput = rnn.Step(&input);
		int intResult = pOutput->GetMaxIndex();
		printf("%c --> %c\n", inputStr[i], alphabet[intResult]);
	}

	rnn.CloseTraining();
	rnn.Delete();

	MyPause();
}


#endif	// WORKING