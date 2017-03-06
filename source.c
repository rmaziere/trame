#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

unsigned char data[] = {0xb5, 0x62, 0x01, 0x21,
0x14, 0x00,
0x60, 0x12, 0xe7, 0x1e, 0x0d, 0x00, 0x00, 0x00, 0x6c, 0x77, 0x00, 0x00, 0xe0, 0x07, 0x07, 0x17, 0x00, 0x00, 0x2b, 0x07,
0xd4, 0x01};

struct gpsTrameHeader{
	unsigned char sync1, sync2, class, id, length;
	unsigned long iTOW, tAcc, tAcc2, year, month, day, hour, min, sec, valid;
	long nano;
};

unsigned long trameReader(unsigned char trame[], int start_indice, int length){
	if(start_indice >= 0 && length > 0){
		int i;
		unsigned long data = 0;
		for(i = 0; i < length; i++){
			data = data | (trame[i + start_indice] << (8 * i));
		}
		return data;
	}
	else{
		return 0;
	}
}

bool checkTrame(unsigned char data[]){
	int i;
	uint8_t ckA = 0, ckB = 0, CK_A, CK_B;

	CK_A = data[26];
	CK_B = data[27];

	for(i = 2; i < 26; i++){
		ckA = ckA + data[i];
		ckB = ckB + ckA;
	}

	printf("ckA : %x - CK_A : %x\n", ckA, CK_A);
	printf("ckB : %x - CK_B : %x\n", ckB, CK_B);

	if(ckA == CK_A && ckB == CK_B){
		return true;
	}
	return false;
}

int main(int argc, char **argv){
	struct gpsTrameHeader trame;
	int start_data_indice = 6;

	trame.sync1 = data[0];
	trame.sync2 = data[1];
	trame.class = data[2];
	trame.id = data[3];
	trame.length = data[4] | (data[5] << 8);
	trame.iTOW = trameReader(data, start_data_indice, 4);

	trame.tAcc = trameReader(data, start_data_indice + 4, 4);

	trame.nano = trameReader(data, start_data_indice + 8, 4);

	trame.year = trameReader(data, start_data_indice + 12, 2);
	trame.month = data[start_data_indice + 14];
	trame.day 	= data[start_data_indice + 15];
	trame.hour 	= data[start_data_indice + 16];
	trame.min 	= data[start_data_indice + 17];
	trame.sec 	= data[start_data_indice + 18];
	trame.valid = data[start_data_indice + 19];

	printf("Sync1 = 0x%x\n", trame.sync1);
	printf("Sync2 = 0x%x\n", trame.sync2);
	printf("Class = %x\n", trame.class);
	printf("id = %x\n", trame.id);
	printf("Length = %d\n", trame.length);

	printf("CheckTrame : %d\n", checkTrame(data));

	printf("Nano = %d ns\n", trame.nano);
	printf("Time accuracy = %d\n", trame.tAcc);

	printf("Time (yy-mm-dd hh:mm:ss) : %d-%d-%d %d:%d:%d\n", trame.year, trame.month, trame.day, trame.hour, trame.min, trame.sec);

	printf("iTOW = %lx\n", trame.iTOW);
}
