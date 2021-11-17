#ifndef PONG_H_
#define PONG_H_

#include <stdlib.h>

struct bola_t {
	__uint32_t x, y; /* position on the screen */
	__uint32_t dx, dy; /* movement vector */
	__uint32_t m, h;
};

struct jogador_t {
	__uint32_t x, y, h, score;
};

void intro();
void espera_botao();
void init_game(struct bola_t *ball, struct jogador_t *jogadores, __uint32_t score1, __uint32_t score2);
void check_score(struct jogador_t *jogador);
__uint32_t check_colisao(struct bola_t *a, struct jogador_t *b);
void move_bola(struct bola_t *ball, struct jogador_t *jogador);
void move_jogador0 (struct jogador_t *jogador, __uint32_t adc);
void move_jogador1 (struct jogador_t *jogador, struct bola_t *ball);

#endif /* PONG_H_ */
