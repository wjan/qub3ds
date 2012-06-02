#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <game.h>
#include <board.h>
#include <soundmanager.h>
#include <debug.h>
#include <PA9.h>
#include <gamescores.h>

#define LOGO_LENGTH 35
#define LOGO "\
 QQQ   U   U BBBB  333  DDD   SSS  \
Q   Q  U   U B   B    3 D  D S     \
Q   Q  U   U BBBB   33  D  D  SSS  \
Q  QQ  U   U B   B    3 D  D     S \
 QQQQ   UUU  BBBB  333  DDD  SSSS  \
     Q                             \
"

#define HLOGO_LENGTH 38
#define HLOGO "\
H  H III   SSS   CCC  OOO  RRRR  EEEE \
H  H  I   S     C    O   O R   R E    \
HHHH  I    SSS  C    O   O RRRR  EEE  \
H  H  I       S C    O   O R R   E    \
H  H III  SSSS   CCC  OOO  R  RR EEEE \
"

GameScores gs;

void filledQuad(float x, float y, float z, float length, float r, float g, float b) {
	glColor3f ( r, g, b );
	glBegin(GL_QUADS);
		glVertex3f ( x, y, z);
		glVertex3f ( x + length, y, z);
		glVertex3f ( x + length, y + length, z );
		glVertex3f ( x, y + length, z);
	glEnd();
}

void drawLogo(char *logoChars, int lineLength,  float divider, float yOffset, float z) {
	Drawer drawer;
	drawer.rx = 5;
	drawer.z += 3;
	drawer.y += 3;
	drawer.x -= .4;
	drawer.draw();
	float x = 0;
	float y = 8;

	for (int i = 0; i < strlen(logoChars); i++) {
		if ( x == lineLength ) {
			y--;
			x = 0;
		}
		if (x < lineLength && logoChars[i] != ' ') {
			float r = 0;
			float g = 0;
			float b = 0;
			while (r + g + b == 0) {
				r = rand() % 2;
				g = rand() % 2;
				b = rand() % 2;
			}
			filledQuad(x / divider, y / divider - yOffset, z, 0.2, r, g, b);
			
		}
		x++;
	}
	glFlush(0);
}

void runMenu() {
	consoleClear();
	bool hscores = gs.areAvailable();
	Score *scores = gs.getScores();
	int logoRedraw = 0;
	int scorePos = 0;
	printf("\x1b[21;2HControls: D-PAD, A and START");
	do {
		scanKeys();
		if ( keysDown() & KEY_A )
		{
			SoundManager::getInstance().switchMusicHearable();
		}
		if ( logoRedraw % 6 == 0 ) {
			drawLogo(LOGO, LOGO_LENGTH, 5, 3.5, 0);
		}
		if ( logoRedraw % 80 < 40 ) {
			printf("\x1b[17;6HPress START to begin");
		} else {
			printf("\x1b[17;6H                    ");
		}
		if ( hscores && logoRedraw % 240 > 229 && logoRedraw % 240 < 239 ) {
			printf ( "\x1b[14;5H                             ");
		}
		if ( hscores && logoRedraw % 240 == 0 ) {
			printf ( "\x1b[14;5HScore %d:", scorePos + 1);
			if ( scores[scorePos].valid )
			{
				printf ( "\x1b[14;14H%d by %c%c%c", scores[scorePos].score,  scores[scorePos].name[0],scores[scorePos].name[1],scores[scorePos].name[2]);
			} else {
				printf ( "\x1b[14;14HNo score yet" );
			}
			scorePos++;
			if ( scorePos >= MAX_SCORES ) {
				scorePos = 0;
			}
		} else if ( !hscores ) {
			printf ( "\x1b[13;5HDLDI patching enables", scorePos + 1);
			printf ( "\x1b[14;5H  saving highscores", scorePos + 1);
		}
		logoRedraw++;
		swiWaitForVBlank();
	} while (!(keysDown() & KEY_START));
}

int runGame(Game *game) {
	game->reset();
	game->start();
	while (!game->isOver()) {
		game->tick();
	}
	return game->getScore();
}

#define CHARS "ABCDEFGHIJKLMNOPQRSTUVWYXZ"
#define NAME_LENGTH 3

void runScoreboard(int score) {
	int position = gs.getScoreToBoardIndex(score);
	if ( !gs.areAvailable() || position < 0) {
		return;	
	}
	glFlush(0);
	consoleClear();
	char scoreName[NAME_LENGTH];
	scoreName[0] = 0;
	scoreName[1] = 0;
	scoreName[2] = 0;
	int scoreNameIndex = 0;

	Score *scores = gs.getScores();
	
	printf("\x1b[9;10HYour rank: %d", position + 1);
	printf("\x1b[11;2HUse D-PAD to enter your name");
	printf("\x1b[13;4HPress START to continue\n");
	int logoRedraw = 0;
	do {
		if (logoRedraw % 6 == 0) {
			drawLogo(HLOGO, HLOGO_LENGTH, 5.2, 1.8, -1);
		}
		logoRedraw++;
		scanKeys();
		printf ( "\x1b[16;11HName:");
		printf ( "\x1b[16;17H%c%c%c\n", CHARS[scoreName[0]],CHARS[scoreName[1]],CHARS[scoreName[2]]);
		printf ( "\x1b[17;17H   ");
		printf ( "\x1b[17;%dH^", 17 + scoreNameIndex);
		if ( keysDown() & KEY_A )
		{
			SoundManager::getInstance().switchMusicHearable();
		}
		if ( keysDown() & KEY_LEFT )
		{
			if ( scoreNameIndex > 0 )
			{
				scoreNameIndex--;
			}
		}
		else if ( keysDown() & KEY_RIGHT )
		{
			if ( scoreNameIndex < NAME_LENGTH - 1 )
			{
				scoreNameIndex++;
			}
		}
		else if ( keysDown() & KEY_DOWN )
		{
			if ( scoreName[scoreNameIndex] > 0 )
			{
				scoreName[scoreNameIndex]--;
			}
			else
			{
				scoreName[scoreNameIndex] = strlen ( CHARS ) - 1;
			}
		}
		else if ( keysDown() & KEY_UP )
		{
			if ( scoreName[scoreNameIndex] < strlen ( CHARS ) - 1 )
			{
				scoreName[scoreNameIndex]++;
			}
			else
			{
				scoreName[scoreNameIndex] = 0;	
			}
		}
		swiWaitForVBlank();
	} while (!(keysDown() & KEY_START));
	Score addScore;
	addScore.name = (char *) malloc(sizeof(char) * 4);
	addScore.name[0] = CHARS[scoreName[0]];
	addScore.name[1] = CHARS[scoreName[1]];
	addScore.name[2] = CHARS[scoreName[2]];
	addScore.name[3] = 0;
	addScore.score = score;
	gs.addScore(addScore);
}

int main() {
	Debug::getInstance().setEnabled(false);
	PA_Init();
	SoundManager& sm = SoundManager::getInstance();
	Game game;
	PA_SwitchScreens();
	sm.playMod(GAME_STATE_MENU);
	while (1) {
		runMenu();	
		sm.playMod(GAME_STATE_GAME);
		game.highscore = gs.getScores()[0].score;
		int score = runGame(&game);
		sm.playMod(GAME_STATE_MENU);
		runScoreboard(score);
	}
	return 0;
}
