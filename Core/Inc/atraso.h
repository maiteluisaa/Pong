#ifndef ATRASO_H_
#define ATRASO_H_


/* O nr de ciclos gastos nas instruções depende da forma da implementação do hardware (acesso a memória)
   já registrei o dobro do tempo esperado quando usei frequencias menores que 36 MHz no STM32 (sendo 72 MHz a máxima).

 CUIDADO no uso de BAIXAS frequencias, abaixo de 3MHz o atraso de us sera errado!

 Para precisão, usar aferição com um osciloscópio.

 */

#include "main.h"

#define FCPU 8000000							//define a frequencia da CPU - ALTERAR TB NA LINHA DE BAIXO!
asm (".equ fcpu, 8000000 \n\t");				//define a frequencia para o uso em código assembly

asm (".equ const_ms, fcpu/3000		 \n\t");	//constante para o atraso_ms
asm (".equ const_us, fcpu/3000000	 \n\t");	//constante para o atraso_us

/*	Colocar o argumento igual a ZERO resultará no máximo atraso possível para a função.
	Não existe teste contra zero, visto que é um valor ilógico para a função	*/


//Valores máximos considerando uma máxima frequencia de trabalho de 84 MHz
//Valores fora da faixa permitida gerarão valores irregulares de atrasos.

//VALOR MÁXIMO DE 153s

void atraso_us(uint32_t valor);		//valor máximo de 153.000.000
void atraso_ms(uint32_t valor);		//valor máximo de 153.000
void atraso_loops(uint32_t valor);  //valor <= 4.294.967.296  (2^32) // repetição de 3 ciclos de clock por unidade de valor

#endif /* ATRASO_H_ */
