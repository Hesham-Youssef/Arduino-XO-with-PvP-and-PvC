#include <IRremote.h>

bool isMovesLeft(char game[3][3]) {
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			if (game[i][j] == '-')
				return true;
		}
	}
	return false;
}

int checkWinner(char game[3][3]) {
	if ((game[0][0] == game[1][1]) && (game[1][1] == game[2][2])) {
		if (game[1][1] == 'x')
			return 10;
		else if (game[1][1] == 'o')
			return -10;
	}

	if ((game[0][2] == game[1][1]) && (game[1][1] == game[2][0])) {
		if (game[1][1] == 'x')
			return 10;
		else if (game[1][1] == 'o')
			return -10;
	}


	for (int i = 0;i < 3;i++) {
		if ((game[i][0] == game[i][1]) && (game[i][1] == game[i][2])) {
			if (game[i][0] == 'x')
				return 10;
			else if (game[i][0] == 'o')
				return -10;
		}

		if ((game[0][i] == game[1][i]) && (game[1][i] == game[2][i])) {
			if (game[0][i] == 'x')
				return 10;
			else if (game[0][i] == 'o')
				return -10;
		}

	}

	return 0;
}


int minimax(char game[3][3], int depth, bool isMax){
	int state = checkWinner(game);

	if (state != 0)
		return state;

	if (isMovesLeft(game) == false)
		return 0;

  if(depth == 3)
    return 0;

	int best = -1000;
	if (isMax) {
		for (int i = 0;i < 3;i++) {
			for (int j = 0;j < 3;j++) {
				if (game[i][j] == '-') {

					game[i][j] = 'x';

					best = max(best, minimax(game, depth + 1, !isMax));

					game[i][j] = '-';
				}
			}
		}
		return best;
	}
	else {
		int best = 1000;

		// Traverse all cells
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				// Check if cell is empty
				if (game[i][j] == '-')
				{
					// Make the move
					game[i][j] = 'o';

					best = min(best, minimax(game, depth + 1, !isMax));

					game[i][j] = '-';
				}
			}
		}
		return best;
	}
}

byte bestMove[2];

void findBestMove(char game[3][3])
{
	int bestVal = -1000;

	bestMove[0] = -1;
	bestMove[1] = -1;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Check if cell is empty
			if (game[i][j] == '-')
			{
				// Make the move
				game[i][j] = 'x';

				int moveVal = minimax(game, 0, false);

				game[i][j] = '-';

				if (moveVal > bestVal)
				{
					bestMove[0] = i;
					bestMove[1] = j;
					bestVal = moveVal;
				}
			}
		}
	}

	return bestMove;
}

const byte IRR = 11;
const byte LED = 13;
String currCode = "";
int delayTime = 5;
char board[3][3] = {{'-','-','-'},
                    {'-','-','-'},
                    {'-','-','-'}};

bool playerTurn = true;
bool aganistPlayer = true;
bool gameDone = true;


void makeAMove(byte i, byte j){
  if(gameDone){
    return;
  }

  byte move[2];
  if(board[i][j] == '-'){
    if(playerTurn){
      board[i][j] = 'o';
      if(!aganistPlayer){
        findBestMove(board);
        Serial.println("computer played");
        if(bestMove[0] != -1)
          board[bestMove[0]][bestMove[1]] = 'x';
      }else{
        playerTurn = !playerTurn;
      }
    }else{
      board[i][j] = 'x';
      playerTurn = !playerTurn;
    }

    byte winner = checkWinner(board);
    if(checkWinner(board) == 10){
      Serial.println("Player 2 Won!!");
      gameDone = true;
    }else if(checkWinner(board) == -10){
      Serial.println("Player 1 Won!!");
      gameDone = true;
    }else{
      if(!isMovesLeft(board)){
        Serial.println("Draw");
        gameDone = true;
      }
    }
    byte res = checkWinner(board);
    Serial.println(String(board[0][0]) + " " + String(board[0][1]) + " " + String(board[0][2]));
    Serial.println(String(board[1][0]) + " " + String(board[1][1]) + " " + String(board[1][2]));
    Serial.println(String(board[2][0]) + " " + String(board[2][1]) + " " + String(board[2][2]));
  }
}




void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IRR);
  pinMode(LED, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

}

void loop() {
  if(!aganistPlayer){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }

  if(!gameDone){
    if(playerTurn)
      digitalWrite(12, HIGH);
    else
      digitalWrite(12, LOW);
  }else{
    digitalWrite(12, HIGH);
    delay(500);
    digitalWrite(12, LOW);
    delay(500);
  }

  bool ledToBlink[9] = {0};
  
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      if(board[i][j] == '-'){
        digitalWrite((i*3+j+2), LOW);
      }else{
        if(board[i][j] == 'o'){
          digitalWrite((i*3+j+2), HIGH);
        }else{
          ledToBlink[(i*3+j)] = true;
        }
      }
    }
  }

  for(int i=0;i<9;i++){
    if(ledToBlink[i])
      digitalWrite(i+2, HIGH);
  }
  if(!gameDone)
    delay(200);
  else
    delay(600);
  for(int i=0;i<9;i++){
    if(ledToBlink[i])
      digitalWrite(i+2, LOW);
  }
  delay(200);


  if(IrReceiver.decode()){
    String ir_code = String(IrReceiver.decodedIRData.command, HEX);
    if(ir_code != currCode){
      currCode = ir_code;
      if(currCode == "10"){
        aganistPlayer = !aganistPlayer;
        Serial.println("aganist computer");
      }else if(currCode == "11"){
        makeAMove(0, 0);
      }else if(currCode == "12"){
        makeAMove(0, 1);
      }else if(currCode == "13"){
        makeAMove(0, 2);
      }else if(currCode == "14"){
        makeAMove(1, 0);
      }else if(currCode == "15"){
        makeAMove(1, 1);
      }else if(currCode == "16"){
        makeAMove(1, 2);
      }else if(currCode == "17"){
        makeAMove(2, 0);
      }else if(currCode == "18"){
        makeAMove(2, 1);
      }else if(currCode == "19"){
        makeAMove(2, 2);
      }else if(currCode == "a"){
        for(int i=0;i<3;i++){
          for(int j=0;j<3;j++){
            board[i][j] = '-';
          }
        }
        playerTurn = true;
        gameDone = false;
      }
      Serial.println(currCode);
    }
    IrReceiver.resume();
  }

}
