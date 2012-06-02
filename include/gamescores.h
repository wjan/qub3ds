#ifndef GAMESCORES_H
#define GAMESCORES_H

#define MAX_SCORES 5
#define SCORES_FILENAME "qub3scr"

typedef struct Score {
	char *name;
	int score;
	bool valid;
};

class GameScores
{
	public:
		GameScores();
		~GameScores();
		bool areAvailable();
		int getScoreToBoardIndex(int score);
		Score* getScores();
		void addScore(Score score);
	private:
		int fatInit;
		Score scores[MAX_SCORES];
		void readScores();
		void writeScores();

};

#endif
