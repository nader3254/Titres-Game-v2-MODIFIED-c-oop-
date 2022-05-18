#include "engine.h"

engine::engine()
{
    buf_relics            = 0 ;
	game_speed            = 20;
	speed_count           = 0 ;
	score                 = 0 ;
	num_pieces            = 0 ;
    tetro_x               = work->CONSOLE_W / 2 - TETRO_W / 2;
	tetro_y               = 2 ;
	cur_tetro_type        = 0 ;
    vk_up_down            = FALSE;
	cur_tetro_orientation = 0;


    InitGameStat();
    

    
	while (1)
    {
		UpdateTopInfo();
		UpdateBottomInfo();
		// When the new tetromino immedately collides,
		// the screen is full -->> quit game loop Game Over
		if (!NoCollision(0, 0, 0)) {
			DrawTetromino();
			RenderDidplay();
			GameOver();
			InitGameStat();
			continue;
		}
		HandleKeyPress();
		DrawTetromino();
		RenderDidplay();

		ClearDisplay();
		TryLowerTetromino();

		Sleep(SLEEP_TIME);
    }
   
};

engine::~engine()
{
}

void engine::UpdateTopInfo() {
	stringstream msg_score;
	msg_score << "Score: " << score;
	msg_score << "      ";
	msg_score << "Pieces: " << num_pieces;
	msg_score << "      ";
	int gs = 41 - game_speed / 0.5;
	msg_score << "Speed: " << gs << "  ";
	ShowMsg(msg_score, 2);
}
void engine::ShowMsg(stringstream const& t, int pos ) 
{
   
     pos = (work->CONSOLE_H - 1)*work->CONSOLE_W + 5;
    
	memcpy(&work->buf_display[pos], t.str().c_str(), t.str().length());
}

void engine::UpdateBottomInfo() {
	stringstream msg_score;
	msg_score << "[ESC]: Exit";
	ShowMsg(msg_score, (work->CONSOLE_H - 1) * work->CONSOLE_W + 2);
}

BOOL engine::NoCollision(int dx, int dy, int dr) {
	int cur_x = tetro_x + dx;
	int cur_y = tetro_y + dy;
	int cur_orit = cur_tetro_orientation + dr;
	cur_orit = cur_orit >= 4 ? 0 : cur_orit;

	for (int y = 0; y < TETRO_H; y++) {
		for (int x = 0; x < TETRO_W; x++) {
			string tetro = work->tetromino_repo[cur_tetro_type][cur_orit];
			if (tetro[y * TETRO_W + x] == '\xDB')
				if (work->buf_display[(cur_y + y) * work->CONSOLE_W + (cur_x + x)] != ' ') {
					return FALSE;
				}
		}
	}
	return TRUE;
}

// Draw the falling tetromino
void engine::DrawTetromino() {
	string tetromino = work->tetromino_repo[cur_tetro_type][cur_tetro_orientation];
	for (int y = 0; y < TETRO_H; y++) {
		for (int x = 0; x < TETRO_W; x++) {
			char pixel = tetromino[y * TETRO_W + x];
			// pixel = RotateTetromino(tetromino, x, y, 90);
			if (pixel != '.') {
				work->buf_display[(tetro_y + y) * work->CONSOLE_W + (tetro_x + x)] = pixel;
			}
		}
	}
}

void engine::RenderDidplay() {
	WriteConsoleOutputCharacterA(work->console_screen, work->buf_display,
		work->CONSOLE_W * work->CONSOLE_H, COORD({ 0, 0 }),
		&work->chars_written);
}
void engine::GameOver() {
	const int PROMPT_W = work->CONSOLE_W / 2 + 12;
	const int PROMPT_H = 10;
	const int PROMPT_POS_X = work->CONSOLE_W / 2 - PROMPT_W / 2;
	const int PROMPT_POS_Y = work->CONSOLE_H / 2 - PROMPT_H / 2;

	string t(PROMPT_H * PROMPT_W, ' ');
	for (int y = 0; y < PROMPT_H; y++) {
		for (int x = 0; x < PROMPT_W; x++) {
			if (x == 0 || x == PROMPT_W - 1 || y == 0 || y == PROMPT_H - 1)
				t[y * PROMPT_W + x] = 178;
			else
				t[y * PROMPT_W + x] = ' ';
		}
	}
	stringstream str1;
	str1 << "GAME OVER!";
	stringstream str2;
	str2 << "Your Score: ";
	str2 << score;
	stringstream str3;
	str3 << "Press SPACE to continue...";
	
	
	t.replace(2 * PROMPT_W + (PROMPT_W / 2 - str1.str().length() / 2),
		str1.str().length(), str1.str());
	t.replace(5 * PROMPT_W + 3, str2.str().length(), str2.str());
	t.replace(7 * PROMPT_W + 3, str3.str().length(), str3.str());
	

	const char* msg = t.c_str();
	for (int y = 0; y < PROMPT_H; y++) {
		int pos = (y + PROMPT_POS_Y) * work->CONSOLE_W + PROMPT_POS_X;
		memcpy(&work->buf_display[pos], &msg[y * PROMPT_W], PROMPT_W);
	}
	RenderDidplay();

	while (1) {
        
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			WorkArea * W = new WorkArea();
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_RETURN) & 0x8000 ))
			return;
	}
}

void engine::InitGameStat() {
	score = 0;
	game_speed = 20;
	num_pieces = 0;
	ClearRelicsBuf();
	ClearDisplay();
	GenerateNewTetromino();  // init the first tetromino
	DrawBoundaries();
}
void engine::HandleKeyPress() {
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		WorkArea * W = new WorkArea();
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000 && NoCollision(-1, 0, 0)) {
		tetro_x -= 1;
		return;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && NoCollision(1, 0, 0)) {
		tetro_x += 1;
		return;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && NoCollision(0, 1, 0)) {
		tetro_y += 1;
		return;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000 && NoCollision(0, 0, 1) && !vk_up_down) {
		vk_up_down = TRUE;
		cur_tetro_orientation++;
		if (cur_tetro_orientation == 4) {
			cur_tetro_orientation = 0;
		}
		return;
	}
	else if (!(GetAsyncKeyState(VK_UP) & 0x8000)) {
		vk_up_down = FALSE;
	}
}

void engine::TryLowerTetromino() {
	speed_count++;
	if (speed_count >= game_speed) {
		speed_count = 0;
		if (!NoCollision(0, 1, 0)) {  // if collision
			SaveTetrominoRelics();
			TryClearFullLineRelics();
			GenerateNewTetromino();
			GameSpeedAdjuster();

		}
		else {  // if no collision
			tetro_y++;
		}
	}
}
/*****************************/
void engine::SaveTetrominoRelics() {
	for (int y = 0; y < TETRO_H; y++) {
		for (int x = 0; x < TETRO_W; x++) {
			string tetro = work->tetromino_repo[cur_tetro_type][cur_tetro_orientation];
			if (tetro[y * TETRO_W + x] == '\xDB') {
				work->buf_relics[(tetro_y + y) * work->CONSOLE_W + (tetro_x + x)] = TRUE;
				work->buf_display[(tetro_y + y) * work->CONSOLE_W + (tetro_x + x)] = '\xDB';
			}
		}
	}
}
void engine::TryClearFullLineRelics() {
	for (int y = work->CONSOLE_H - 3; y > 1; y--) {
		BOOL is_full = TRUE;
		for (int x = work->CONSOLE_W - 3; x > 1; x--) {
			// If there is an empty slot, is_full becomes FALSE
			is_full &= work->buf_relics[y * work->CONSOLE_W + x];
		}
		if (is_full) {
			RelicsRowClearAnimation(y);
			RelicsRowShift(y);
			score += int(25 - game_speed);
			y++;  //last line lowers by 1 row
		}
	}
}
void engine::RelicsRowShift(int row) {
	for (int y = row; y > 1; y--) {
		for (int x = work->CONSOLE_W - 3; x > 1; x--) {
			if (y == 2) {  // top-most row is always empty
				work->buf_relics[y * work->CONSOLE_W + x] = FALSE;
				work->buf_display[y * work->CONSOLE_W + x] = ' ';
			}
			else {
				work->buf_relics[y * work->CONSOLE_W + x] =
					work->buf_relics[(y - 1) * work->CONSOLE_W + x];
				work->buf_display[y * work->CONSOLE_W + x] =
					work->buf_display[(y - 1) * work->CONSOLE_W + x];
			}
		}
	}
}
void engine:: RelicsRowClearAnimation(int y) {
	// flicker three times
	for (int i = 0; i< 3; i++) {
		for (int x = work->CONSOLE_W - 3; x > 1; x--) {
			work->buf_display[y * work->CONSOLE_W + x] = ' ';
		}
		RenderDidplay();
		Sleep(10);
		for (int x = work->CONSOLE_W - 3; x > 1; x--) {
			work->buf_display[y * work->CONSOLE_W + x] = '\xDB';
		}
		RenderDidplay();
		Sleep(10);
	}
}
void engine::GameSpeedAdjuster() {
	if (num_pieces % SPEED_UP_NUM == 0)
		game_speed = game_speed - 0.5 > 2 ? game_speed - 0.5 : 2;
}




void engine::ClearRelicsBuf() {
	for (int i = 0; i < work->CONSOLE_W * work->CONSOLE_H; i++) {
		work->buf_relics[i] = FALSE;
	}
}
void engine::ClearDisplay() {
	for (int y = 2; y < work->CONSOLE_H - 2; y++) {
		for (int x = 2; x < work->CONSOLE_W - 2; x++) {
			if (work->buf_relics[y * work->CONSOLE_W + x] != TRUE) {
				work->buf_display[y * work->CONSOLE_W + x] = ' ';
			}
		}
	}
	
}

void engine::GenerateNewTetromino() {
	tetro_x = work->CONSOLE_W / 2 - TETRO_W / 2;  // init xpos at the middle
	tetro_y = 2;                            // init y pos on the top
	srand(time(NULL));
	cur_tetro_type = rand() % NUM_TETRO_TYPE;  // random type
											   // cur_tetro_type = 0;
	cur_tetro_orientation = rand() % 4;        // random oritentation
	num_pieces++;
}
void engine::DrawBoundaries() {
	for (int y = 0; y < work->CONSOLE_H; y++) {
		for (int x = 0; x < work->CONSOLE_W; x++) {
			if (x > 0 && x < work->CONSOLE_W - 1 && y > 0 && y < work->CONSOLE_H - 1) {
				if (x == 1 || x == work->CONSOLE_W - 2)
					work->buf_display[y * work->CONSOLE_W + x] = 186;  // left,right
				if (y == work->CONSOLE_H - 2)
					work->buf_display[y * work->CONSOLE_W + x] = 207;  // bottom 205
				if (y == 1)
					work->buf_display[y * work->CONSOLE_W + x] = '_';  // upper 196
			}
		}
	}
	work->buf_display[1 * work->CONSOLE_W + 1] = '_';  //Upper left 214
	work->buf_display[1 * work->CONSOLE_W + (work->CONSOLE_W - 2)] = '_';  //upper right 183
	work->buf_display[(work->CONSOLE_H - 2) * work->CONSOLE_W + 1] = 200;  //bottom left
	work->buf_display[(work->CONSOLE_H - 2) * work->CONSOLE_W + (work->CONSOLE_W - 2)] = 188; //bottom right
}