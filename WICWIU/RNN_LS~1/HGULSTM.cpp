#ifdef WORKING

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "HGUUtility.h"
#include "HGULongArray.h"
#include "HGUOptimizer.h"
#include "HGULayerConfig.h"
#include "HGUConnectionLayer.h"
#include "HGURNN.h"
#include "HGULSTM.h"


#include "HGUClassInfo.h"
#include "HGUNeuralNetConfig.h"
#include "HGUNeuralNet.h"

#ifdef __CUDA__
#include <cuda_runtime.h>
#endif // __CUDA__


int HGULSTM::Alloc(int inputDim, int hiddenDim, int outputDim, int actFun, int hiddenActFun)
{
	if (m_aWeight != NULL)
		Delete();

	hiddenDim = outputDim;								// in LSTM hiddenDim == outputDim

	m_inputWidth = inputDim;
	m_inputHeight = 1;
	m_inputSize = inputDim;
	m_noInputChannel = 1;
	m_inDim = inputDim;

	m_outputWidth = outputDim;
	m_outputHeight = 1;
	m_outputSize = outputDim;
	m_noOutputChannel = 1;
	m_outDim = outputDim;

	m_hiddenDim = hiddenDim;

	if (actFun >= 0)
		m_actFun = actFun;
	if (hiddenActFun >= 0)
		m_hiddenActFun = hiddenActFun;

	m_deltaDim = 0;

	memset(m_aWeightIndex, 0, sizeof(m_aWeightIndex));

	m_aWeightIndex[WT_InputToHidden] = 0;				// input to hidden	(no bias)
	m_parameterDim = inputDim  * hiddenDim;

	m_aWeightIndex[WT_OutputToHidden] = m_parameterDim;	// output to hidden
	m_parameterDim += (outputDim + 1) * hiddenDim;

//	m_aWeightIndex[WT_HiddenToOutput] = m_parameterDim;	// hidden to output
//	m_parameterDim += (hiddenDim + 1) * outputDim;

	m_gatePitch = inputDim + outputDim + 1;

	m_aWeightIndex[WT_InputGate] = m_parameterDim;			// inputGate
	m_parameterDim += m_gatePitch * hiddenDim;

	m_aWeightIndex[WT_ForgetGate] = m_parameterDim;		// forgetGate
	m_parameterDim += m_gatePitch * hiddenDim;

	m_aWeightIndex[WT_OutputGate] = m_parameterDim;		// outputGate
	m_parameterDim += m_gatePitch * outputDim;

	try {
		m_aWeight = new Float[m_parameterDim];
		m_aOutput.Alloc(m_outDim);
		m_aOutput.Fill(0.F);
		m_aHidden[0].Alloc(m_hiddenDim);
		m_aHidden[0].Fill(0);
		m_aHidden[1].Alloc(m_hiddenDim);
		// m_aHidden[1].Fill(0);					// not necessary

		m_hidden0.Alloc(m_hiddenDim);
		m_hidden2.Alloc(m_hiddenDim);

		m_aInputGate.Alloc(m_hiddenDim);
		m_aForgetGate.Alloc(m_hiddenDim);
		m_aOutputGate.Alloc(m_outDim);

		m_activeHidden = 0;
	} catch (...){
		printf("Failed to allocate memory in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		m_aWeight = NULL;
		return FALSE;
	}

	InitializeWeight();

	SetGateBias(m_inDim + m_outDim, m_hiddenDim, &GetWeight_InputGate(), 6.F);
	SetGateBias(m_inDim + m_outDim, m_hiddenDim, &GetWeight_ForgetGate(), 6.F);
	SetGateBias(m_inDim + m_outDim, m_hiddenDim, &GetWeight_OutputGate(), 6.F);

	return TRUE;
}

HGULongArray<Float>* HGULSTM::Step(HGULongArray<Float> *pInput)
{
	m_pInput = pInput;								// for training
	HGULongArray<Float> &prevHidden = m_aHidden[m_activeHidden];
	m_activeHidden = 1 - m_activeHidden;			// toggle current hidden and previous hidden
	HGULongArray<Float> &hidden = m_aHidden[m_activeHidden];

	// compute input, forget, hidden gates
	ComputeWeightedSum(m_inDim, m_hiddenDim, pInput->GetBlock(0), &GetWeight_InputGate(), m_aInputGate.GetBlock(0), FALSE, TRUE, m_gatePitch);
	ComputeWeightedSum(m_outDim, m_hiddenDim, m_aOutput.GetBlock(0), &GetWeight_InputGate() + m_inDim, m_aInputGate.GetBlock(0), TRUE, FALSE, m_gatePitch);
	ApplyActivationFunction(AF_Sigmoid, &m_aInputGate);

#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", TRUE, "Input gate: ");
	LogLongArray("Z:\\lstm.log", FALSE, &m_aInputGate);
#endif // _DEBUG

	ComputeWeightedSum(m_inDim, m_hiddenDim, pInput->GetBlock(0), &GetWeight_ForgetGate(), m_aForgetGate.GetBlock(0), FALSE, TRUE);
	ComputeWeightedSum(m_outDim, m_hiddenDim, m_aOutput.GetBlock(0), &GetWeight_ForgetGate() + m_inDim, m_aForgetGate.GetBlock(0), TRUE, FALSE, m_gatePitch);
	ApplyActivationFunction(AF_Sigmoid, &m_aForgetGate);

#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", FALSE, "Forget gate: ");
	LogLongArray("Z:\\lstm.log", FALSE, &m_aForgetGate);
#endif // _DEBUG

	ComputeWeightedSum(m_inDim, m_outDim, pInput->GetBlock(0), &GetWeight_OutputGate(), m_aOutputGate.GetBlock(0), FALSE, TRUE);
	ComputeWeightedSum(m_outDim, m_outDim, m_aOutput.GetBlock(0), &GetWeight_OutputGate() + m_inDim, m_aOutputGate.GetBlock(0), TRUE, FALSE, m_gatePitch);
	ApplyActivationFunction(AF_Sigmoid, &m_aOutputGate);

#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", FALSE, "Output gate: ");
	LogLongArray("Z:\\lstm.log", FALSE, &m_aOutputGate);
#endif // _DEBUG

	// compute hidden0
	//	input -> hidden0
	ComputeWeightedSum(m_inDim, m_hiddenDim, pInput->GetBlock(0), &GetWeight_InputToHidden(0, 0), m_hidden0.GetBlock(0), FALSE, TRUE);
	//	output -> hidden0
	ComputeWeightedSum(m_outDim, m_hiddenDim, m_aOutput.GetBlock(0), &GetWeight_OutputToHidden(0, 0), m_hidden0.GetBlock(0), TRUE, FALSE);
	ApplyActivationFunction(m_hiddenActFun, &m_hidden0);

#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", FALSE, "Hidden0: ");
	LogLongArray("Z:\\lstm.log", FALSE, &m_hidden0);
#endif // _DEBUG

	// compute hidden
	for (int h = 0; h < m_hiddenDim; h++)
		hidden[h] = prevHidden[h] * m_aForgetGate[h] + m_hidden0[h] * m_aInputGate[h];


#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", FALSE, "Hidden: ");
	LogLongArray("Z:\\lstm.log", FALSE, &hidden);
#endif // _DEBUG

	// compute output
	m_aOutput = hidden;
	ApplyActivationFunction(m_actFun, &m_aOutput);

#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", FALSE, "Hidden2: ");
	LogLongArray("Z:\\lstm.log", FALSE, &m_aOutput);		// currently, m_aOutput contains C2
#endif // _DEBUG

	if (m_maxTrainStep > 0)			// if training mode
		m_hidden2 = m_aOutput;

	for (int o = 0; o < m_outDim; o++)
		m_aOutput[o] *= m_aOutputGate[o];

#ifdef DEBUG
	LogMessageF("Z:\\lstm.log", FALSE, "Output: ");
	LogLongArray("Z:\\lstm.log", FALSE, &m_aOutput);
#endif // _DEBUG

	return &m_aOutput;
}

HGULongArray<Float>* HGULSTM::StepForTrain(HGULongArray<Float> *pInput, HGULongArray<Float> *pDesiredOutput, float *pAccError)
{
	if (m_maxTrainStep <= 0){
		printf("To run BPTT, input and hidden buffers must be allocated. %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}

	HGULongArray<Float> *pOutput = Step(pInput);
	if (pOutput == NULL)
		return NULL;

	if (pAccError){
		Float error = GetError(pDesiredOutput);
		*pAccError += error;
	}

	HGULongArray<Float> aGradientWrtPrevOutput(m_outDim);
	aGradientWrtPrevOutput.Fill(0.F);
	HGULongArray<Float> aGradientWrtPrevHidden(m_hiddenDim);
	aGradientWrtPrevHidden.Fill(0.F);

//printf("aGradientWrtPrevHidden: (%d %d)\n", aGradientWrtPrevHidden.GetSize(), aGradientWrtPrevHidden.GetBlockSize());

	SaveCurrentStep();

	if (pDesiredOutput != NULL){
		for (int s = 0; s < m_trainStep; s++){
			// gradient w.r.t. output
			if (s == 0)
				ComputeDelta0(pDesiredOutput, 1.F, &m_aOutput, &m_aDelta0);		// gradient w.r.t. output
			else
				m_aDelta0 = aGradientWrtPrevOutput;
			aGradientWrtPrevOutput.Fill(0.F);

			////// output gate
			// gradient w.r.t. output gate
			m_aDelta.Multiply(m_aDelta0, m_hidden2);

			// gradient w.r.t. NET_og
			HGUConnectionLayer::MultiplyDerivative(m_aDelta, AF_Sigmoid, m_aOutputGate);

			// gradient w.r.t. U_og, V_og, B_og
 			ComputeGradientFromDelta(m_inDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousInput(s).GetBlock(0), &GetGradient_OutputGate(), FALSE, m_gatePitch);
			if (s < m_trainStep - 1)
				ComputeGradientFromDelta(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousOutput(s + 1).GetBlock(0), &GetGradient_OutputGate() + m_inDim, TRUE, m_gatePitch);

			// accumulate gradient w.r.t. previous output
			BackPropagate(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), &GetWeight_OutputGate() + m_inDim, aGradientWrtPrevOutput.GetBlock(0), TRUE, m_gatePitch);


			// gradient w.r.t. C2
			m_aDelta0_hidden.Multiply(m_aDelta0, m_aOutputGate);

			// gradient w.r.t. C
			ComputeDeltaFromDelta0(m_actFun, m_outDim, m_hidden2.GetBlock(0), m_aDelta0_hidden.GetBlock(0), m_aDelta_hidden.GetBlock(0));

			if (s > 0)
				m_aDelta_hidden.Add(aGradientWrtPrevHidden, 1.F);

			if (s < m_trainStep - 1)
				aGradientWrtPrevHidden.Multiply(m_aDelta_hidden, m_aForgetGate);

			///// forget gate
			if (s < m_trainStep - 1){
				// gradient w.r.t. forget gate
				m_aDelta.Multiply(m_aDelta_hidden, GetPreviousHidden(s + 1));

				// gradient w.r.t. NET_fg
				HGUConnectionLayer::MultiplyDerivative(m_aDelta, AF_Sigmoid, m_aForgetGate);

				// gradient w.r.t. U_fg, V_fg, B_fg
				ComputeGradientFromDelta(m_inDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousInput(s).GetBlock(0), &GetGradient_ForgetGate(), FALSE, m_gatePitch);
				if (s < m_trainStep - 1)
					ComputeGradientFromDelta(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousOutput(s + 1).GetBlock(0), &GetGradient_ForgetGate() + m_inDim, TRUE, m_gatePitch);

				// accumulate gradient w.r.t. previous output
				BackPropagate(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), &GetWeight_ForgetGate() + m_inDim, aGradientWrtPrevOutput.GetBlock(0), TRUE, m_gatePitch);
			}


			///// input gate
			// gradient w.r.t. input gate
			m_aDelta.Multiply(m_aDelta_hidden, m_hidden0);

			// gradient w.r.t. NET_ig
			HGUConnectionLayer::MultiplyDerivative(m_aDelta, AF_Sigmoid, m_aInputGate);

			// gradient w.r.t. U_ig, V_ig, B_ig
			ComputeGradientFromDelta(m_inDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousInput(s).GetBlock(0), &GetGradient_InputGate(), FALSE, m_gatePitch);
			if (s < m_trainStep - 1)
				ComputeGradientFromDelta(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousOutput(s + 1).GetBlock(0), &GetGradient_InputGate() + m_inDim, TRUE, m_gatePitch);

			// accumulate gradient w.r.t. previous output
			BackPropagate(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), &GetWeight_InputGate() + m_inDim, aGradientWrtPrevOutput.GetBlock(0), TRUE, m_gatePitch);


			///// Input Weights
			// gradient w.r.t. C0
			m_aDelta.Multiply(m_aDelta_hidden, m_aInputGate);

			// gradient w.r.t. NET_C0
			HGUConnectionLayer::MultiplyDerivative(m_aDelta, m_hiddenActFun, m_hidden0);

			// gradient w.r.t S
			ComputeGradientFromDelta(m_inDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousInput(s).GetBlock(0), &GetGradient_InputToHidden(0, 0), FALSE);
			// gradient w.r.t T
			if (s < m_trainStep - 1)
				ComputeGradientFromDelta(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), GetPreviousOutput(s + 1).GetBlock(0), &GetGradient_OutputToHidden(0, 0), TRUE);

			// accumulate gradient w.r.t. previous output
			BackPropagate(m_outDim, m_hiddenDim, m_aDelta.GetBlock(0), &GetWeight_OutputToHidden(0, 0), aGradientWrtPrevOutput.GetBlock(0), TRUE);
		}
	}

	return pOutput;
}

int HGULSTM::PrepareToTrain(Float *pSharedGradient)
{
	HGURNN::PrepareToTrain(pSharedGradient);

	m_aDelta0_hidden.Alloc(m_hiddenDim);		// gradient w.r.t. hidden
	m_aDelta_hidden.Alloc(m_hiddenDim);		// gradient w.r.t. NET_hidden

	return TRUE;
}

void HGULSTM::CloseTraining()
{
	HGURNN::CloseTraining();

	m_aDelta0_hidden.Delete();
	m_aDelta_hidden.Delete();
}

void HGULSTM::SetGateBias(int inDim, int outDim, Float *pWeight, Float v)
{
	Float *w = pWeight + inDim;
	for (int o = 0; o < outDim; o++, w += inDim + 1)
		*w = v;
}

/*
void LSTMTest()
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

	HGULSTM lstm;
	lstm.Alloc(5, 4, 5);
	lstm.PrepareToTrain();
	lstm.SetOptimizer(OT_SGD, 0.9F);
//	lstm.SetMomentum(OT_SGC);
	if (lstm.GetActivationFunction() != AF_SoftMax)
		lstm.SetTrueClassGain((float)sqrt(float(lstm.GetOutputDim() - 1)));

	int ret = lstm.LoadWeight("lstm.wgt");
	if (ret == FALSE)
		lstm.SaveWeight("lstm.wgt");

	printf("##### Training lstm\n");
	for (int epoch = 0; epoch < 100000000; epoch++){
		Float error = 0.F;
		lstm.ResetTrainStep();
		lstm.ResetHidden();
		lstm.ResetOutput();
		for (int i = 0; i < seqLen; i++){
			input.MakeOneHotVector(inputSeq[i], outDim);
			desired.MakeOneHotVector(desiredSeq[i], outDim);

			lstm.StepForTrain(&input, &desired, &error);
		}

		lstm.UpdateWeightsWithGradient(0.001F);

		if ((epoch + 1) % 10000 == 0)
		{
			printf("epoch %5d, Average error = %f\n", epoch + 1, error / seqLen);
			fflush(stdout);
		}

		if ((epoch + 1) % 100000 == 0)
		{
			printf("##### Testing lstm\n");
			lstm.ResetHidden();
			lstm.ResetOutput();
			for (int i = 0; i < seqLen; i++){
				input.MakeOneHotVector(inputSeq[i], outDim);
				HGULongArray<Float> *pOutput = lstm.Step(&input);
				int intResult = pOutput->GetMaxIndex();
				printf("%c --> %c\n", inputStr[i], alphabet[intResult]);
			}

//			lstm.SaveWeight("lstm.wgt");
		}

		if (error / seqLen < 0.001F){
			printf("Training done, epoch = %d, error = %f\n", epoch + 1, error / seqLen);
			break;
		}
	}
	printf("##### Testing lstm\n");
	lstm.ResetHidden();
	lstm.ResetOutput();
	for (int i = 0; i < seqLen; i++){
		input.MakeOneHotVector(inputSeq[i], outDim);
		HGULongArray<Float> *pOutput = lstm.Step(&input);
		int intResult = pOutput->GetMaxIndex();
		printf("%c --> %c\n", inputStr[i], alphabet[intResult]);
	}

	MyPause();

	lstm.CloseTraining();
	lstm.Delete();
}

*/

void Char2Vector(char c, HGULongArray<Float> &vector);
char *dataFile = "Z:\\Hello.c";
//char *dataFile = "Z:\\Hello.txt";
char *modelFile = "Z:\\lstm_text.wgt";

void LSTMTest()
{
	// read text file
	long fileLen = GetFileLength(dataFile);
	if (fileLen == 0){
		printf("No file %s in %s (%s %d)\n", dataFile, __FUNCTION__, __FILE__, __LINE__);
		return;
	}
	char *aBuffer = new char[fileLen];
	if (aBuffer == NULL){
		printf("Failed to allocated %ld bytes in %s (%s %d)\n", fileLen, __FUNCTION__, __FILE__, __LINE__);
		return;
	}

	FILE *fp = fopen(dataFile, "r");
	if (fp == NULL){
		printf("Failed to open %s in %s (%s %d)\n", dataFile, __FUNCTION__, __FILE__, __LINE__);
		return;	
	}

	fread(aBuffer, 1, fileLen, fp);

	fclose(fp);

	int outDim = 128;
	float learningRate = 0.00001F;

	HGULongArray<Float> input;
	HGULongArray<Float> desired;

	HGULSTM lstm;
	lstm.Alloc(128, 100, 128);

	lstm.PrepareToTrain();
	lstm.SetOptimizer(OT_SGD, 0.9F);
//	lstm.SetOptimizer(OT_RMSProp, 0.9F);
//	lstm.SetOptimizer(OT_Adam, 0.9F);

	if (lstm.GetActivationFunction() != AF_SoftMax)
		lstm.SetTrueClassGain((float)sqrt(float(lstm.GetOutputDim() - 1)));

	int ret = lstm.LoadWeight(modelFile);
	if (ret == FALSE)
		lstm.SaveWeight(modelFile);
	
	printf("##### Training lstm\n");
	for (int epoch = 0; epoch < 100000000; epoch++){
		Float error = 0.F;
		lstm.ResetTrainStep();
		lstm.ResetHidden();
		lstm.ResetOutput();
		for (int i = 0; i < fileLen - 1; i++){
			input.MakeOneHotVector(aBuffer[i], 128);
			desired.MakeOneHotVector(aBuffer[i + 1], 128);

			lstm.StepForTrain(&input, &desired, &error);

			if ((i + 1) % 32 == 0)
				lstm.UpdateWeightsWithGradient(learningRate);
		}

		lstm.UpdateWeightsWithGradient(learningRate);

//		if ((epoch + 1) % 10000 == 0)
//		if ((epoch + 1) % 10 == 0)
		{
			printf("epoch %5d, average error = %f\n", epoch + 1, error / fileLen);
			fflush(stdout);
		}

		if ((epoch + 1) % 100 == 0)	{
			printf("##### Testing lstm\n");
			lstm.ResetHidden();
			lstm.ResetOutput();
			for (int i = 0; i < fileLen; i++){
				input.MakeOneHotVector(aBuffer[i], outDim);
				HGULongArray<Float> *pOutput = lstm.Step(&input);
				int intResult = pOutput->GetMaxIndex();
				printf("%c --> %c\n", aBuffer[i], intResult);
			}

			lstm.SaveWeight(modelFile);
		}

		if (error / fileLen < 0.001F){
			printf("Training done, epoch = %d, error = %f\n", epoch + 1, error / fileLen);
			break;
		}
	}
	printf("##### Testing lstm\n");
	lstm.ResetHidden();
	lstm.ResetOutput();
	for (int i = 0; i < fileLen; i++){
		input.MakeOneHotVector(aBuffer[i], outDim);
		HGULongArray<Float> *pOutput = lstm.Step(&input);
		int intResult = pOutput->GetMaxIndex();
		printf("%c --> %c\n", aBuffer[i], intResult);
	}

	MyPause();

	lstm.CloseTraining();
	lstm.Delete();

	delete[] aBuffer;
}

void Char2Vector(char c, HGULongArray<Float> &vector)
{
	if (vector.GetSize() != 128)
		vector.Alloc(128);
	vector.Fill(0.F);
	if (c & 0x80)
		vector[0] = 1.F;
	else
		vector[c] = 1.F;
}

#endif // WORKING