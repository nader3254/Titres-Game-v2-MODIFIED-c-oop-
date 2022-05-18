#include "configscreen.h"

WorkArea :: WorkArea()
{
	HandleNormalExit();
}

WorkArea::WorkArea(string s) 
{
    chars_written = 0;
    buf_display   = 0;
    buf_relics    = 0;
	buf_display   = 0;
	chars_written = 0;
	buf_relics    = 0;
    CmdSettings();
    Tetrominoes();
    CreatCmdBuffer();
    CreatCmdBuffer();
    DisableResize();
    FixedSize();
    DisableCursorBlinking();
    StartScreen();


}

WorkArea::~WorkArea()
{
}
void WorkArea::StartScreen()
{
    const int SIGNATURE_W = 26;
	const int SIGNATURE_H = 4;
	string signature = "";
	int signature_x = CONSOLE_W / 2 - SIGNATURE_W / 2;
	// signature_x = 5;
	int signature_y = 15;
	int dx = 2;
	int dy = 1;

	stringstream m;
	for (int i = 0; i < 5; i++) {
		m << "                                            ";
	}
	m << "     _____          _            _          ";
	m << "    |_   _|        | |          (_)         ";
	m << "      | |     ___  | |_   _ __   _   ___    ";
	m << "      | |    / _ \\ | __| | ___| | | / __\\    ";
	m << "     | |   |  __/ | |_  | |    | | \\__ |   ";
	m << "      \\_/    \\___|  \\__| |_|    |_| |___/  ";
	m << "                                     _      ";
	m << "                            \\  / /| / \\      ";
	m << "                            \\/   |o\\_/      ";
	for (int i = 0; i < 25; i++) {
		m << "                                            ";
	}
	m << "        Press SPACE to continue...          ";
/*
	signature += "                __        ";
	signature += "|\\/| _  _ | _  |  \\ _ |_ _";
	signature += "|  |(_||_)|(-  |__/(_||_(-";
	signature += "       |                  ";
*/
    signature += "              _   _  _    ";
	signature += R"(|\  / /\   | | |_ |_|   )";
	signature += R"(| \/  /  \  |_| |_ | \   )";
	signature += R"(                          )";

	// Splash screen loop
	while (1) {
		memcpy(buf_display, m.str().c_str(), m.str().length());

		// Draw signature
		for (int y = 0; y < SIGNATURE_H; y++) {
			int pos = (signature_y + y) * CONSOLE_W + signature_x;
			memcpy(&buf_display[pos], &signature[y * SIGNATURE_W], SIGNATURE_W);
		}
		signature_x += dx;
		signature_y += dy;
		if (signature_x + SIGNATURE_W >= CONSOLE_W - 2 ||
			signature_x <= 1) {
			dx *= -1;
		}
		if (signature_y + SIGNATURE_H >= 37 ||
			signature_y <= 14) {
			dy *= -1;
		}
		// Display and key handling
		RenderDidplay();
		ClearDisplayBuf();
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			HandleNormalExit();
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_RETURN) & 0x8000))
			break;
		Sleep(100);
	}
}
void WorkArea::HandleNormalExit() {
    if (buf_display) delete[] buf_display;
	if (buf_relics) delete[] buf_relics;
	exit(0);
}
void WorkArea::ClearDisplayBuf() {
	for (int i = 0; i < CONSOLE_W * CONSOLE_H; i++) {
		buf_display[i] = ' ';
	}
}
void WorkArea::RenderDidplay() {
	WriteConsoleOutputCharacterA(console_screen, buf_display,
		CONSOLE_W * CONSOLE_H, COORD({ 0, 0 }),
		&chars_written);
}
void WorkArea:: CmdSettings()
{
    
   // Init empty display buf and relic buf
	buf_display = new char[CONSOLE_W * CONSOLE_H];
	buf_relics = new BOOL[CONSOLE_W * CONSOLE_H];
    for (int i = 0; i < CONSOLE_W * CONSOLE_H; i++) {
		buf_display[i] = ' ';
		buf_relics[i] = FALSE;
	}

}
void WorkArea::Tetrominoes()
{
    vector<string> Is(4);
	vector<string> Os(4);
	vector<string> Zs(4);
	vector<string> Ts(4);
	vector<string> Ls(4);
	vector<string> Ss(4);
	vector<string> Js(4);
	vector<string> Saviors(4);

	// tmp var for making tetromino blocks
	string tmp = "";

	// I 0 2 
	tmp += "....\xDB\xDB....";
	tmp += "....\xDB\xDB....";
	tmp += "....\xDB\xDB....";
	tmp += "....\xDB\xDB....";
	Is[0] = tmp;
	Is[2] = tmp;
	tmp = "";

	// I 1 3
	tmp += "..........";
	tmp += ".\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB.";
	tmp += "..........";
	tmp += "..........";
	Is[1] = tmp;
	Is[3] = tmp;
	tmp = "";

	// O 0 1 2 3
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "..........";
	tmp += "..........";
	Os[0] = tmp;
	Os[1] = tmp;
	Os[2] = tmp;
	Os[3] = tmp;
	tmp = "";

	// Z 0 2 
	tmp += "..\xDB\xDB\xDB\xDB....";
	tmp += "....\xDB\xDB\xDB\xDB..";
	tmp += "..........";
	tmp += "..........";
	Zs[0] = tmp;
	Zs[2] = tmp;
	tmp = "";
	// Z 1 3 
	tmp += ".....\xDB\xDB...";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "...\xDB\xDB.....";
	tmp += "..........";
	Zs[1] = tmp;
	Zs[3] = tmp;
	tmp = "";

	// T0
	tmp += "..\xDB\xDB\xDB\xDB\xDB\xDB..";
	tmp += "....\xDB\xDB....";
	tmp += "..........";
	tmp += "..........";
	Ts[0] = tmp;
	tmp = "";
	// T1
	tmp += ".....\xDB\xDB...";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += ".....\xDB\xDB...";
	tmp += "..........";
	Ts[1] = tmp;
	tmp = "";
	// T2
	tmp += "....\xDB\xDB....";
	tmp += "..\xDB\xDB\xDB\xDB\xDB\xDB..";
	tmp += "..........";
	tmp += "..........";
	Ts[2] = tmp;
	tmp = "";
	// T3
	tmp += "...\xDB\xDB.....";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "...\xDB\xDB.....";
	tmp += "..........";
	Ts[3] = tmp;
	tmp = "";

	// L0
	tmp += "..\xDB\xDB\xDB\xDB\xDB\xDB..";
	tmp += "..\xDB\xDB......";
	tmp += "..........";
	tmp += "..........";
	Ls[0] = tmp;
	tmp = "";
	// L1
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += ".....\xDB\xDB...";
	tmp += ".....\xDB\xDB...";
	tmp += "..........";
	Ls[1] = tmp;
	tmp = "";
	// L2
	tmp += "......\xDB\xDB..";
	tmp += "..\xDB\xDB\xDB\xDB\xDB\xDB..";
	tmp += "..........";
	tmp += "..........";
	Ls[2] = tmp;
	tmp = "";
	// L3
	tmp += "...\xDB\xDB.....";
	tmp += "...\xDB\xDB.....";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "..........";
	Ls[3] = tmp;
	tmp = "";

	// S 0 2 
	tmp += "....\xDB\xDB\xDB\xDB..";
	tmp += "..\xDB\xDB\xDB\xDB....";
	tmp += "..........";
	tmp += "..........";
	Ss[0] = tmp;
	Ss[2] = tmp;
	tmp = "";
	// S 1 3 
	tmp += "...\xDB\xDB.....";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += ".....\xDB\xDB...";
	tmp += "..........";
	Ss[1] = tmp;
	Ss[3] = tmp;
	tmp = "";

	// J0
	tmp += "..\xDB\xDB\xDB\xDB\xDB\xDB..";
	tmp += "......\xDB\xDB..";
	tmp += "..........";
	tmp += "..........";
	Js[0] = tmp;
	tmp = "";
	// J1
	tmp += ".....\xDB\xDB...";
	tmp += ".....\xDB\xDB...";
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "..........";
	Js[1] = tmp;
	tmp = "";
	// J2
	tmp += "..\xDB\xDB......";
	tmp += "..\xDB\xDB\xDB\xDB\xDB\xDB..";
	tmp += "..........";
	tmp += "..........";
	Js[2] = tmp;
	tmp = "";
	// J3
	tmp += "...\xDB\xDB\xDB\xDB...";
	tmp += "...\xDB\xDB.....";
	tmp += "...\xDB\xDB.....";
	tmp += "..........";
	Js[3] = tmp;
	tmp = "";

	// Savior Blocks
	tmp += ".....\xDB....";
	tmp += "..........";
	tmp += "..........";
	tmp += "..........";
	Saviors[0] = tmp;
	Saviors[1] = tmp;
	Saviors[2] = tmp;
	Saviors[3] = tmp;
	tmp = "";


	tetromino_repo[TetroType::I] = Is;
	tetromino_repo[TetroType::O] = Os;
	tetromino_repo[TetroType::Z] = Zs;
	tetromino_repo[TetroType::T] = Ts;
	tetromino_repo[TetroType::L] = Ls;
	tetromino_repo[TetroType::S] = Ss;
	tetromino_repo[TetroType::J] = Js;
	tetromino_repo[TetroType::Savior] = Saviors;

}
void WorkArea::CreatCmdBuffer()
{
    console_screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleTitleA(window_title.c_str());

}
void WorkArea::DisableResize()
{
    
    if (!SetConsoleWindowInfo(console_screen, TRUE, &min_window_size))
		cout<<"Failed resizing window(1st)", GetLastError();
	if (!SetConsoleScreenBufferSize(console_screen,
		{ (short)CONSOLE_W, (short)CONSOLE_H }))
		cout<<"Failed resizing screen buffer", GetLastError();
	if (!SetConsoleWindowInfo(console_screen, TRUE, &window_size))
		cout<<"Failed resizing window(2nd)", GetLastError();


}
void WorkArea::FixedSize()
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    MoveWindow(console, 450, 80 ,380, 750, TRUE); // 380 width, 750 height
    

}
void WorkArea::DisableCursorBlinking()
{
    CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(console_screen, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(console_screen, &cci);
	// Activate new scrren
	SetConsoleActiveScreenBuffer(console_screen);


}