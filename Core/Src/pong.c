#include "pong.h"
#include "figuras.h"
#include "NOKIA5110.h"
#include "atraso.h"


void intro(){
		goto_XY(0, 0);
		string_LCD("   PONG   ");
		goto_XY(0, 2);
		string_LCD("Aperte para");
		goto_XY(0, 3);
		string_LCD("iniciar o jogo");
		imprime_LCD();
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) != GPIO_PIN_SET);
		HAL_Delay(300);
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) != GPIO_PIN_SET);
}

void espera_botao(){
	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) != GPIO_PIN_SET);
	HAL_Delay(300);
	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) != GPIO_PIN_SET);
}

void init_game(struct bola_t *ball, struct jogador_t *jogadores, __uint32_t score1, __uint32_t score2){

		ball->x = 42; // bola no centro
		ball->y = 24;
		ball->dy = 2; //função de numero aleatorio
		ball->dx = 2; //função de numero aleatorio
		ball->h = 2; // raio
		ball->m = 2; // raio

		(jogadores)->x = 4;  // 0 + 2
		(jogadores)->y = 24; // 48/2
		(jogadores)->h = 6;
		(jogadores)->score = (jogadores)->score + score1;

		(jogadores+1)->x = 80; // 81 + 2
		(jogadores+1)->y = 24; // 48/2
		(jogadores+1)->h = 6;
		(jogadores+1)->score = (jogadores+1)->score + score2;
}

void check_score(struct jogador_t *jogador){

	int i;

	//loop through player scores
	for(i = 0; i < 2; i++) {
		limpa_LCD();
		goto_XY(0, 0);
		string_LCD_Nr("Seu score: ", (jogador)->score, 1);// escreve uma mensagem com um número
		goto_XY(0, 2);
		string_LCD_Nr("Score adv: ", (jogador+1)->score, 1);
		imprime_LCD();
		atraso_ms(2500);
		//check if score is @ the score win limit
		if ((jogador+i)->score == 3 ) {

			//reset scores
			(jogador)->score = 0;
			(jogador+1)->score = 0;

			//return 1 if player 1 score @ limit
			if (i == 0) {
				escreve2fb((unsigned char*) game_won);
				imprime_LCD();
				atraso_ms(2500);
				espera_botao();

			//return 2 if player 2 score is @ limit
			} else {
				escreve2fb((unsigned char*) game_over);
				imprime_LCD();
				atraso_ms(2500);
				espera_botao();
			}
		}
	}
}

__uint32_t check_colisao(struct bola_t *a, struct jogador_t *b){

	__uint32_t  left_a, left_b;
	__uint32_t  right_a, right_b;
	__uint32_t  top_a, top_b;
	__uint32_t  bottom_a, bottom_b;

		left_a = a->x - (a->m);
		right_a = a->x + (a->m);
		top_a = a->y - (a->h);
		bottom_a = a->y + (a->h);

		left_b = b->x;
		right_b = b->x;
		top_b = b-> y - (b->h)/2;
		bottom_b = b->y + (b->h)/2;


		if (left_a > right_b) {
			return 0;
		}

		if (right_a < left_b) {
			return 0;
		}

		if (top_a > bottom_b) {
			return 0;
		}

		if (bottom_a < top_b) {
			return 0;
		}

		return 1;
}

void move_bola(struct bola_t *ball, struct jogador_t *jogador){
	/* Move the ball by its motion vector. */
		ball->x += ball->dx;
		ball->y += ball->dy;

		/* Turn the ball around if it hits the edge of the screen. */
		if (ball->x < 3) {

			init_game(ball, jogador, 0, 1);
			check_score(jogador);
		}

		if (ball->x > 80) {

			init_game(ball, jogador, 1, 0);
			check_score(jogador);
		}

		if (ball->y < 3 || ball->y > 43) {

			ball->dy = -ball->dy;
		}

		//check for collision with the paddle
		__uint32_t i;

		for (i = 0; i < 2; i++) {

			__uint32_t c = check_colisao(ball, (jogador+i));


			//collision detected
			if (c) {

				//ball moving left
				if (ball->dx < 0) ball->dx -= 1;

				//ball moving right
				else ball->dx += 1;

				//change ball direction
				ball->dx = -ball->dx;

				//change ball angle
				__uint32_t hitP = ((jogador+i)->y + (jogador+i)->h) - ball->y;

				if(hitP >= 1 && hitP < 8) ball->dy = 3;
				if(hitP>= 8 && hitP < 15) ball->dy = 2;
				if(hitP>=15 && hitP < 22) ball->dy = 1;
				if(hitP>=22 && hitP < 25) ball->dy = 0;
				if(hitP>=25 && hitP < 33) ball->dy = -1;
				if(hitP>=33 && hitP < 40) ball->dy = -2;
				if(hitP>=40 && hitP < 47) ball->dy = -3;

				//ball moving right
				if(ball->dx > 0){
					if(ball->x < 6) ball->x = 6;}
				else{
					if(ball->x > 82) ball->x = 82;}



			}

		}
}


void move_jogador0 (struct jogador_t *jogador, __uint32_t adc){

	if(adc >= 4000){
		if(jogador->y + 2 + 3 < 48)
		jogador->y = jogador->y + 2;
	}

	if(adc <= 800){
		if (jogador->y - 2 > 2)
		jogador->y = jogador->y - 2;
	}
}

void move_jogador1 (struct jogador_t *jogador, struct bola_t *ball){
	__uint32_t center = (jogador+1)->y;
	__uint32_t screen_center = 24;
	int ball_speed = ball->dy;

	if (ball_speed < 0) {

		ball_speed = -ball_speed;
	}

	//ball moving right
	if (ball->dx < 0) {

		//return to center position
		if (center < screen_center) {
			(jogador+1)->y += ball_speed;

		}

		if (center > screen_center)

			(jogador+1)->y -= ball_speed;
		}

	//ball moving left
	else {

		//ball moving down
		if (ball->dy > 0) {

			if (ball->y > center) {
				if((jogador+1)->y + 2 < 45)
				(jogador+1)->y += 2;

			} else {
				if ((jogador+1)->y - 2 > 2)
				(jogador+1)->y -= 2;
			}
		}

		//ball moving up
		if (ball->dy < 0) {

			if (ball->y < center) {
				if ((jogador+1)->y - 2 > 2)
				(jogador+1)->y -= 2;

			} else {
				if((jogador+1)->y + 2 < 45)
				(jogador+1)->y += 2;
			}
		}

		//ball moving stright across
		if (ball->dy == 0) {

			if (ball->y < center)
				if ((jogador+1)->y > 2)
					(jogador+1)->y -= 1;

			if (ball->y > center)
				if((jogador+1)->y < 45)
					(jogador+1)->y += 1;
			}
		}
}
