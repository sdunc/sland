#include "stage.h"

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doFighters(void);
static void doBullets(void);
static void spawnEnemies(void);
static void drawFighters(void);
static void drawBullets(void);
static int bulletHitFighter(Entity* b);

static Entity* player;
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;
static int enemySpawnTimer;

void initStage(void) {
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	/* pre cache the texture so we dont lond on each bullet init */
	bulletTexture = loadTexture("gfx/bullet.png");
	enemyTexture = loadTexture("gfx/enemy.png");

	enemySpawnTimer = 0;
}

static void initPlayer(void) {
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));
	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->x = 100;
	player->y = 100;
	player->side = SIDE_PLAYER;
	player->texture = loadTexture("gfx/player.png");
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void) {
	doPlayer();

	doFighters();

	doBullets();

	spawnEnemies();
}

static void doPlayer(void) {
	player->dx = player->dy = 0;

	if (player->reload > 0) {
		player->reload--;
	}

	if (app.keyboard[SDL_SCANCODE_UP]) {
		player->dy = -PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_DOWN]) {
		player->dy = PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_RIGHT]) {
		player->dx = PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_LEFT]) {
		player->dx = -PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload == 0) {
		fireBullet();
	}
}

static void doFighters(void) {
	Entity *e;
	Entity *prev;

	prev = &stage.fighterHead;

	for (e = stage.fighterHead.next; e != NULL; e = e->next) {
		e->x += e->dx;
		e->y += e->dy;

		// not the player and fully offscreen
		if (e != player && (e->x < -e->w || e->health == 0)) {
			if (e == stage.fighterTail) {
				stage.fighterTail = prev;
			}
			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}

static void spawnEnemies(void) {
	Entity* enemy;

	if (--enemySpawnTimer <= 0) {
		enemy = malloc(sizeof(Entity));
		memset(enemy, 0, sizeof(Entity));
		stage.fighterTail->next = enemy;
		stage.fighterTail = enemy;

		enemy->x = SCREEN_WIDTH;
		enemy->y = rand() % SCREEN_HEIGHT;
		enemy->health = 1;
		enemy->side = SIDE_ENEMY;
		enemy->texture = enemyTexture;
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
		
		enemy->dx = -(2 + (rand() % 4));

		enemySpawnTimer = 30 + (rand() % 60);
	}
}

static void drawFighters(void) {
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next) {
		blit(e->texture, e->x, e->y);
	}
}

static void fireBullet(void) {
	Entity* bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));

	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->side = SIDE_PLAYER;

	bullet->dx = PLAYER_BULLET_SPEED;

	bullet->health = 1;
	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

	player->reload = 8;
}

static void doBullets(void) {
	Entity* b;
	Entity* prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next) {
		b->x += b->dx;
		b->y += b->dy;

		if (bulletHitFighter(b) || b->x > SCREEN_WIDTH) {
			if (b == stage.bulletTail) {
				stage.bulletTail = prev;
			}
			prev->next = b->next;
			free(b);
			b = prev;
		}
		prev = b;
	}
}

static int bulletHitFighter(Entity* b) {
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next) {
		if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h)) {
			b->health = 0;
			e->health = 0;

			return 1;
		}
	}
	return 0;
}

static void draw(void) {
	drawFighters();
	drawBullets();
}

static void drawBullets(void) {
	Entity* b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next) {
		blit(b->texture, b->x, b->y);
	}
}