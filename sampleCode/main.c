/* ******************************************************/
/* SETR 23/24, Paulo Pedreiras                          */
/* 		Sample code for Assignment 2					*/
/*   	A few tests to the cmdProcessor to illustrate	*/
/*      how the the tests can be carried out.         	*/
/*														*/
/* Note that I'm not using Unity! That is part of your 	*/
/*		work. 											*/
/*                                                      */
/* Compile with: gcc cmdproc.c main.c -o main           */
/*	Feel free to use flags such as -Wall -Wpedantic ...	*/
/*	and it is a good idea to create a makefile			*/
/*                                                      */
/* ******************************************************/
#include <stdio.h>
#include <string.h>
#include "cmdproc.h"


int main(void) 
{
	int i,len, err;
	unsigned char ans[30]; 
	unsigned char ansTest1[]={'#','p','t', '+', '2', '1', '1', '1', '4','!'};
	
	printf("\n Smart Sensor interface emulation \n");
	printf(" \t - simple illustration of interface and use \n\n\r");
	
	/* Init UART RX and TX buffers */
	resetTxBuffer();
	resetRxBuffer();
	
	/* Test 1 */
	
	printf("Test1 - check the answer to a valid A command\n");
	
	/* 1 - send the command 
	rxChar('#');
	rxChar('A'); //65
	rxChar(64);
	rxChar('!');
	
	
	cmdProcessor();*/
	for (int i = 0; i < 20; i++) {
	rxChar('#');
	rxChar('A');
	rxChar(65);
	rxChar('!');
	
	
	cmdProcessor();
	resetTxBuffer();
	}
	
	printf("Test2 - check the answer to a valid L command\n");
	
	
	rxChar('#');
	rxChar('L'); 
	rxChar(76);
	rxChar('!');
	cmdProcessor();
	resetTxBuffer();
	
			
	/* 2 - vetores */
	
	
	/*
	for (int i = 0; i < 30; i++) {
        printf("Leitura %d:\n", i + 1);
        printf("  Temperatura: %d°C\n", readTemperature());
        printf("  Umidade: %d%%\n", readHumidity());
        printf("  CO2: %d ppm\n", readCO2());
        printf("--------------------------\n");
    }
	
	*/
	
	
	return 0;
}
