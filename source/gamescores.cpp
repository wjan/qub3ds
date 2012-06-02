#include <gamescores.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

GameScores::GameScores()
{
	fatInit = fatInitDefault();
	if ( !areAvailable() ) {
		return;
	}
	for (int i = 0; i < MAX_SCORES; i++) {
		scores[i].name = (char *)malloc(sizeof(char) * 50);
	}
	readScores();
}

void GameScores::readScores() {
	if ( !areAvailable() ) {
		return;
	}
	int entriesRead = 0;
	FILE *file = fopen(SCORES_FILENAME, "rb");
	if (file != NULL) {
		char buff[50];
		while ( entriesRead < MAX_SCORES && fgets(buff, 50, file) != NULL ) {
			int sserr = sscanf(buff, "%s %d", scores[entriesRead].name, &scores[entriesRead].score);
			scores[entriesRead].valid = true;
			if ( sserr >= 0 || strlen(scores[entriesRead].name) > 0 ) {
				entriesRead++;
			} else {
				//this record is not ok
			}
		}
		fclose(file);
	}
	while (entriesRead < MAX_SCORES) {
		scores[entriesRead].valid = false;
		entriesRead++;
	}
}

void GameScores::writeScores() {
	if ( !areAvailable() ) {
		return;
	}
	FILE* file = fopen (SCORES_FILENAME, "wb"); //wb = create/truncate & write 
	if (file != NULL) {
		for (int i = 0; i < MAX_SCORES; i++) {
			if ( scores[i].valid ) {
				char buff[50];
				sprintf(buff, "%s %d\n", scores[i].name, scores[i].score);
				fwrite(buff, strlen(buff), 1, file);
			}
		}
		fclose(file);
	}
}

bool GameScores::areAvailable() {
	return fatInit != 0;
}


void GameScores::addScore(Score score) {
	for (int i = 0; i < MAX_SCORES; i++) {
		if ( !scores[i].valid || scores[i].score < score.score) {
			for (int j = MAX_SCORES - 1; j > i; j--) {
				memcpy(scores[j].name, scores[j - 1].name, strlen(scores[j - 1].name));
				scores[j].score = scores[j - 1].score;
				scores[j].valid = scores[j - 1].valid;
			}
			memcpy(scores[i].name, score.name, strlen(score.name));
			scores[i].score = score.score;
			scores[i].valid = true;
			break;
		}
	}
	writeScores();
}

int GameScores::getScoreToBoardIndex(int score) {
	//return -1;
	for (int i = 0; i < MAX_SCORES; i++) {
		if ( !scores[i].valid || scores[i].score < score) {
			return i;
		}
	}
	return -1;
}

Score* GameScores::getScores() {
	return scores;
}

GameScores::~GameScores()
{
}
