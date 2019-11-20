#include "algorithm_procedures.h"
#include "algorithm_configuration_calib.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

// --------------------------------------------------------------------------------------

_UINT16 crc16(_UINT8 *pcBlock, _UINT16 length)
{
	_UINT16 crc = 0xFFFF;
	_UINT8 i;
	
	while(length--)
	{
			crc ^= *pcBlock++ << 8;
			
			for(i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
	}
	return crc;
}

void parse_data(void)
{
	//parse data from packet
	pos = ftell(infile);
	
	//memset(&InPutData, 0, sizeof(TPacket));
	//if (BlockRead(&InPutData, sizeof(InPutData), infile))
	if(fread(&InPutData, sizeof(TPacket), 1, infile))
	{			
		check_sum = crc16((_UINT8*)&InPutData, sizeof(InPutData)-sizeof(InPutData.Check_Sum));
		
		if (InPutData.Check_Sum == check_sum)
		{
			//memset(&Data, 0, sizeof(Data));
			pos = ftell(infile);
			for (int i = 0; i < 3; i++)
			{
				Data.fB_m_s_s[i] = InPutData.fB_m_s_s[i];
				Data.OmegaB_deg_s[i] = InPutData.OmegaB_deg_s[i];
			}	
			Data.Temperature_C = InPutData.Temperature_C;	
		}
		else
		{
			printf("Lost packet. CRC16 != packet check sum: %d & %d\n", check_sum, InPutData.Check_Sum);
			printf("Packet index: %d\n", InPutData.Index);
			memset(&InPutData, 0, sizeof(InPutData));
			pos++;
			fseek(infile, pos, SEEK_SET);
			tmp = 0;
			
			while(tmp != 0xABCD)
			{
				fread(&tmp, sizeof(_SINT16), 1, infile);
			
				if (tmp == 0xABCD)
					fseek(infile, -2, SEEK_CUR);

				else
					fseek(infile, -1, SEEK_CUR);	
			}
			parse_data();
		}
	}
	else
	{
		pos = ftell(infile);
		reading = 0;
	}
}

void sensor_alinement(void)
{
	if(alinement_set)
	{
		_SINT8 i;
		for(i = 0; i < 3; i++)
			Data.OmegaB_deg_s[i] -= bias_gyro[i]/500.0;		
	}
	else
	{
		_SINT8 i;
		for(i = 0; i < 3; i++)
			bias_gyro[i] += Data.OmegaB_deg_s[i];
			
		if(bias_counter > 499)
			alinement_set = 1;
	}

}

void matrix_set(TCalibratedSensorsData* CalibData)
{
	_FLOAT32 gamma = CalibData->fB_m_s_s[0]/G; //+ AX/G; 
	_FLOAT32 theta = -CalibData->fB_m_s_s[1]/G; //+ AY/G; 
	_FLOAT32 eta   = 0; //CalibData->OmegaB_deg_s[0]/U/cos(PHI); 
	
	cosine_matrix[0][0] = cos(gamma)*cos(eta) + sin(theta)*sin(gamma)*sin(eta);
	cosine_matrix[0][1] = -cos(gamma)*sin(eta) + sin(theta)*sin(gamma)*cos(eta);
	cosine_matrix[0][2] = -cos(theta)*sin(gamma);
	cosine_matrix[1][0] = cos(theta)*sin(eta);
	cosine_matrix[1][1] = cos(theta)*cos(eta);
	cosine_matrix[1][2] = sin(theta);
	cosine_matrix[2][0] = sin(gamma)*cos(eta) - sin(theta)*cos(gamma)*sin(eta);
	cosine_matrix[2][1] = -sin(gamma)*sin(eta) - sin(theta)*cos(gamma)*cos(eta);
	cosine_matrix[2][2] = cos(eta)*cos(gamma);
	
	isSet = 1;
}

void coef_application(void)
{
	for (int i = 0; i != 3; i ++)
	{
		CalibData.fB_m_s_s[i] -= acc_bias[i];
		CalibData.OmegaB_deg_s[i] -= gyro_bias[i];
		
		for (int j = 0; j != 3; j++)
		{
			CalibData.fB_m_s_s[i]  += acc_scale_coefs[i][j] * Data.fB_m_s_s[j];
			CalibData.OmegaB_deg_s[i] -= gyro_scale_coefs[i][j] * Data.OmegaB_deg_s[j] - orth_matrix[i][j] * Data.fB_m_s_s[j];
		}
	}
	
	gyro_matrix[0][0] = 0;
	gyro_matrix[0][1] = -CalibData.OmegaB_deg_s[2] * DEG_TO_RAD;
	gyro_matrix[0][2] = CalibData.OmegaB_deg_s[1] * DEG_TO_RAD;
	gyro_matrix[1][0] = -gyro_matrix[0][1] * DEG_TO_RAD;
	gyro_matrix[1][1] = 0;
	gyro_matrix[1][2] = -CalibData.OmegaB_deg_s[0] * DEG_TO_RAD;
	gyro_matrix[2][0] = -gyro_matrix[0][1] * DEG_TO_RAD;
	gyro_matrix[2][1] = gyro_matrix[1][2] * DEG_TO_RAD;
	gyro_matrix[2][2] = 0;
}

void puasson_equation(void)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				cosine_matrix_new[i][j] += cosine_matrix[i][k]*gyro_matrix[k][j]*dt - gyro_corr[i][k]*cosine_matrix[k][j]*dt;	
}

void normalization(void)
{
	// row normalization
	if (cycle_counter % 2 == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			_FLOAT32 row = 0.0;
			for (int j = 0; j < 3; j++)
				row += pow(cosine_matrix_new[i][j], 2);
				
			row = sqrt(row);
			
			for (int j = 0; j < 3; j++)
				cosine_matrix_new[i][j] = cosine_matrix_new[i][j]/row;
		}	
	}
	// column normalization
	else
	{
		for (int j = 0; j < 3; j++)
		{
			_FLOAT32 col = 0.0;
			for (int i = 0; i < 3; i++)
				col += pow(cosine_matrix_new[i][j], 2);
				
			col = sqrt(col);
			
			for (int i = 0; i < 3; i++)
				cosine_matrix_new[i][j] = cosine_matrix_new[i][j]/col;
		}	
	}
}

void calculation(void)
{
	_FLOAT32 c0 = sqrt(pow(cosine_matrix_new[0][2], 2) + pow(cosine_matrix_new[2][2], 2));
	
	_FLOAT32 pitch, roll, heading;
	// Pitch angle calc
	if (c0 == 0)
		if (cosine_matrix_new[1][2] > 0)
			pitch = PI / 2;
		else
			pitch = -PI / 2;
	else 
		pitch = atan(cosine_matrix_new[1][2]/c0);
	
	// Roll angle calc
	if (cosine_matrix_new[2][2] == 0)
		if (cosine_matrix_new[0][2] > 0)
			roll = PI / 2;
		else 
			roll = -PI / 2;
	else
		if (cosine_matrix_new[2][2] > 0)
			roll = -atan(cosine_matrix_new[0][2]/cosine_matrix_new[2][2]);
		else
			if (cosine_matrix_new[0][2] > 0)
				roll = PI + -atan(cosine_matrix_new[0][2]/cosine_matrix_new[2][2]);
			else
				roll = -PI + -atan(cosine_matrix_new[0][2]/cosine_matrix_new[2][2]);
	
	// Heading angle
	if (cosine_matrix_new[1][1] == 0)
		if (cosine_matrix_new[1][0] > 0)
			heading = PI / 2;
		else
			heading = -PI / 2;
	else
		if (cosine_matrix_new[1][1] > 0)
			heading = atan(cosine_matrix_new[1][0]/cosine_matrix_new[1][1]);
		else
			if (cosine_matrix_new[1][0] > 0)
				heading = PI + atan(cosine_matrix_new[1][0]/cosine_matrix_new[1][1]);
			else 
				heading = -PI + atan(cosine_matrix_new[1][0]/cosine_matrix_new[1][1]);
		if (heading < 0)
			heading += 2 * PI;
			
	OutData.Roll_deg     =  roll * RAD_TO_DEG;
	OutData.Pitch_deg    =  pitch * RAD_TO_DEG;
	OutData.Heading_deg  =  heading * RAD_TO_DEG;
	
	t = cycle_counter*dt;
	
	for(int i = 0; i < 3; i++)
		fN_m_s_s[i] = 0.0;
}

void cosine_matrix_application(void)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			fN_m_s_s[i] += cosine_matrix_new[i][j] * CalibData.fB_m_s_s[j];
			
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			cosine_matrix[i][j] = cosine_matrix_new[i][j];
}

void radial_correction(void)
{
	if (cycle_counter > 1000)
		if ((fabs(fN_m_s_s[0]) > 0.5) || (fabs(fN_m_s_s[1]) > 0.5)) 
			K = 0.05;
		else
			K = 0.1;
	else //(cycle_counter < 10.0 * FREQ)
		K = 10.0;

	_FLOAT32 e_corr = -K * fN_m_s_s[1];
	_FLOAT32 n_corr = K * fN_m_s_s[0];
	
	gyro_corr[1][2] = -e_corr;
	gyro_corr[2][1] = e_corr;
		
        gyro_corr[0][2] = n_corr;
	gyro_corr[2][0] = -n_corr;
}

// --------------------------------------------------------------------------------------
