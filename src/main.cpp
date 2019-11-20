/*  
	Обработка данных инерциального модуля с применением паспорта устройства и алгоритма ориентации
*/
#include "algorithm_procedures.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(void)
{
	infile = fopen("D:\\data2.bin", "rb");
	if (infile == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}
	else
		printf("Reading file...\n");
	
	output = fopen("D:\\data_calib2.txt", "wt");
	if(output == NULL)
	{
		printf("Cannot write data\n");
		exit(1);
	}
	else
		fprintf(output, "Time\tRoll grad\tPitch grad\tYaw grad\tOmega_x\tOmega_y\tOmega_z\tAcc_x\tAcc_y\tAcc_z\tTemp\n");
	
	while(!reading)
	{
		fread(&tmp, sizeof(_SINT16), 1, infile);
		pos = ftell(infile);
		
		if (tmp == 0xABCD) // Заголовок пакета
		{
			reading = 1;
			fseek(infile, -2, SEEK_CUR);
		}
		else
		{
			fseek(infile, -1, SEEK_CUR);	
		}		
	}	
	
	while (reading)
	{
		parse_data();										  
		
		if(!alinement_set)
		{
			bias_counter++;
			sensor_alinement(); // Выставка
		}
	
		if(alinement_set)
		{
			cycle_counter++; 
			
			sensor_alinement();	
			coef_application();
			if(!isSet)
				matrix_set(&CalibData);
			puasson_equation();
			normalization();		
			cosine_matrix_application();
			radial_correction();
			calculation();
			
			// Формат записи в файле:
			// время[с] | Крен[град] | Тангаж[град] | Курс[град] | \
			// Угл.скор_x[град/с] | Угл.скор_y[град/с] | Угл.скор_z[град/с] | \
			// Ускорение_x[м/с/с] | Ускорение_y[м/с/с] | Ускорение_z[м/с/с] | \
			// Температура[град.Цельсия]
			fprintf(output, "%5.2f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\t%9.4f\n",
			t, OutData.Roll_deg, OutData.Pitch_deg, OutData.Heading_deg,
			CalibData.OmegaB_deg_s[0], CalibData.OmegaB_deg_s[1], CalibData.OmegaB_deg_s[2],
			CalibData.fB_m_s_s[0], CalibData.fB_m_s_s[1], CalibData.fB_m_s_s[2], Data.Temperature_C);
			memset(&CalibData, 0, sizeof(CalibData));
		}
	}
	fclose(output);
	fclose(infile);	
	system("PAUSE");
	return 0;
}
