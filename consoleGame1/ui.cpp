#include "stdafx.h"
#include "ui.h"

#define TITLE_LOCATION 40

const char* cursor = "▶";

void cursor_changer()
{
	static int delay = 0;

	if (delay++ == 0)
	{
		cursor = "▶";
	}
	else if (delay == 50)
	{
		cursor = "▷";
	}
	else if (delay == 100)
	{
		delay = 0;
	}
}

void TitleDraw()
{
	static int color = 1;
	static int delay = 0;
	int location_x = (cols - 120) / 2;
	WORD title_color[128];

	if (delay++ > 49)
	{
		if (color++ == 15)
		{
			color = 1;
		}

		delay = 0;
	}

	for (int i = 0; i < 128; i++)
	{
		if (0 < i && i < 82)
		{
			title_color[i] = color;

		}
		else
		{
			title_color[i] = white * 16 + black;
		}
	}

	LHPutAttrCharEx(location_x, 0, ":::::-::-::::-:::::::::-:::-:::--:::::::::::::::::::::::::::::::--:-----======+++++******#######%%%%%%@%@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharExLine(location_x, 1, ": ######    #####   ##   ##  ######   #######  ######   ##   ##    ##     ##   ## *****#*####%%#%%@%%@@@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharExLine(location_x, 2, ":  ##  ##  ##   ##  ### ###   ##  ##   ##   #   ##  ##  ### ###   ####    ###  ## *****####%#%%%%%%%%@%@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharExLine(location_x, 3, ":  ##  ##  ##   ##  #######   ##  ##   ## #     ##  ##  #######  ##  ##   #### ## ***#######%%%%%%%%%@@@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharExLine(location_x, 4, ":  #####   ##   ##  #######   #####    ####     #####   #######  ##  ##   ## #### *****#####%%%%%%%%%@@@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharExLine(location_x, 5, ":  ##  ##  ##   ##  ## # ##   ##  ##   ## #     ## ##   ## # ##  ######   ##  ### ***##%##%%%%%%%%%%@@@@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharExLine(location_x, 6, ":  ##  ##  ##   ##  ##   ##   ##  ##   ##   #   ##  ##  ##   ##  ##  ##   ##   ## *####%#%%%%%%%%%@@@@@@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharExLine(location_x, 7, ": ######    #####   ##   ##  ######   #######  #### ##  ##   ##  ##  ##   ##   ## #####%#%%%%%%%%@@@@@@@@@@@@@@@@@@@@@", title_color);
	LHPutAttrCharEx(location_x, 8, "++++++=++++++*++++++++*****+=++++++*+*****+*+++++++=++***#********+::-----+***##%#%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 9, ":::::=+++++++++++++++-------*+++++++------=*++++++++++**++**#****#--------+***%#*+++=-----=+**%%@@@@@@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 10, ":::-:++++++++++++++++--*----++++++++--*=--=+++++++++++*==++=*****#*==--=+*++=:.             .::--=*%@@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 11, "-::-:++++++++++++++++--=-+--++++++++--=:+--=+++++++++***+++*******#*****=-.                    .:---=*%@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 12, "******+++++++++++++++====*==++++++++====+==+++++*++**+**#+#*******####=.                        .:-----+%@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 13, ":::::++++++++=::::::++*=:==#:::::::*+++++++=::::-:********-======###=.                            :------*@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 14, "-::::++++++++=::::::++*+:--*:::-:::*+++++++=------********=====++%#.                              .-----:-+%@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 15, "--:-:++**++*+=-:::::++*-=++#:::::::*+++++++=------********====+++#.                               .:-------+@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 16, "++++++**===+*++++++++++*=+%*=+++*++*++++++*+*****+**********#####.                 ................--------:#@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 17, ":::::++---==++++++++++**#+++++++++++======+*++****************##+             :----------===================+@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 18, ":::::+**++=**++++++++#=+@@@#++++++++==*++=+++****************##*.            =--:::::::::::-------=++====--++@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 19, "--:::+++*+#++++++++++##%@@@%++++++++==+=*==+**************#####=             *::-:-=-%%%=--:-:::::-++=+##*==+@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 20, "*+**++++++++++++++++++*%@@#+++++++++----=--+*************#####%-             =::::--=%#*+--:...:::--=++%%+-=+@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 21, "::-:-++==--==+:-::::+=====-=-::::::********=------******##++++#-             .--:::------::::::...:=====+===+@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 22, ":::::+--=-----:-::::+--+----::-::::*++++***=---===#****###++**#:                  .......         .---:-----+@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 23, ":::::+--=-=--:-:-:::+--=-=--.:-::-:*+++****=-=====#**#####+***#+                                  .:-------:*@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 24, "++++++----#---+++++=+----*--:++++++*+******+++***+#*######*#####:                                  :-------:#@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 25, "=--=-++=====+#*******************************######*#######%%%%%*                                  :-------:%@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 26, "--:::+--=---=*+++++++++++++++++++******************###########%%%:                                .---------%@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 27, "--:::+--=-=--=++++++++++++++++++*****************###########%%%%%#:                               .--------=%@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 28, "++++=+----#--=+++++++++++++++*****************#############%%%%%%%#                               :-------:*@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 29, "----:+*******=::::::=++++++*-------********+====+=#######%#######%%+.   :                        .:-------+%@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 30, "::-::++++++++=::::::+++++++*-------********+++++++#**####%######%@%%+==---                       .:------+@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 31, "-::::++++++++=-:-:::+++++++*-------********+++++++###%#########%%@%@%++**#=.                     .------*@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 32, "=====++++++++=======+++++***-++++++*******#*******#****%###%%%%%%@@@%+++++*#*=:.                 .---::#@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 33, "-----+++++++++=+++++*=======#++++++#*****##########%%%%%%%%%%%%%@@%@%++++++###%#*=:..        .:--:----#@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 34, "-:-::++++++++=--=---=--+------=====****##############%%%%%%%%%%%%@@@#+++++++*###%%%*=::..  .....=:---%@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 35, "-::::++++++++=---------+-=====+-===+**#############%%%%%%%%%%%%%@@@@#++++++++######%%##+-::-----=:--#@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 36, "=====++++++++=:::=-::::::*------+=-=**############%%%%%%%%%%%%@@@@@@#++++**+++*######%%%%#+++++*+++#%@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 37, "=====++++++++=======+*******+++++++#******#******#%###%%%%%%%%%%%%#*++++++*#*++*########%%%%%%%%%%##%@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 38, "-::-:++++++++=------+*******-======#**#***#*****##%#%%%%%%%%%%%#*+++++++++++###++###############%###%@@@@@@@@@@@@@@@@@", white, black);
	LHPutAttrCharEx(location_x, 39, "-::-:++++++++=------+*******-====++#**#*********##%%%%%%%%%%%*+++++++++**#*++*#####%##############%##%@@@@@@@@@@@@@@@@", white, black);
}

void MenuDraw(const int x, const int y, const int select)
{
	LHPutAttrCharEx(x, y, "StartGame", black, white);
	LHPutAttrCharEx(x, y + 1, "GameInfo", black, white);
	LHPutAttrCharEx(x, y + 2, "Option", black, white);
	LHPutAttrCharEx(x, y + 3, "EndGame", black, white);
	LHPutAttrCharEx(x - 3, y + select, cursor, black, white);
}

void ModeListDraw(const int x, const int y, const int select)
{
	static int delay = 0;
	WORD color[64];
	int spark;

	if (delay++ < 50)
	{
		spark = red;
	}
	else
	{
		spark = lightred;

		if (delay == 100)
		{
			delay = 0;
		}
	}

	for (int i = 0; i < 64; i++)
	{
		if (i < 16)
		{
			color[i] = spark;
		}
		else
		{
			color[i] = white;
		}
	}

	LHPutAttrCharEx((cols - 80) / 2, 3, "   ####                                       ##   ##              ###", black, white);
	LHPutAttrCharEx((cols - 80) / 2, 4, "  ##  ##                                      ### ###               ##", black, white);
	LHPutAttrCharEx((cols - 80) / 2, 5, " ##        ####    ##  ##    ####             #######   ####        ##    ####", black, white);
	LHPutAttrCharEx((cols - 80) / 2, 6, " ##           ##   #######  ##  ##            #######  ##  ##    #####   ##  ##", black, white);
	LHPutAttrCharEx((cols - 80) / 2, 7, " ##  ###   #####   ## # ##  ######            ## # ##  ##  ##   ##  ##   ######", black, white);
	LHPutAttrCharEx((cols - 80) / 2, 8, "  ##  ##  ##  ##   ##   ##  ##                ##   ##  ##  ##   ##  ##   ##", black, white);
	LHPutAttrCharEx((cols - 80) / 2, 9, "   #####   #####   ##   ##   #####            ##   ##   ####     #####    #####", black, white);

	LHPutAttrCharEx(30, 8, "    . ", black, spark);
	LHPutAttrCharEx(30, 9, "    .:.", black, spark);
	LHPutAttrCharEx(30, 10, "     .-:        ..", black, spark);
	LHPutAttrCharEx(30, 11, "       --. .: .-:", black, spark);
	LHPutAttrCharEx(30, 12, "        :=:=--=.", black, spark);
	LHPutAttrCharEx(30, 13, "      .:===::-:::::...", black, spark);
	LHPutAttrCharEx(30, 14, "        :==::=-::::....", black, spark);
	LHPutAttrCharExLine(30, 15, "     :--::+-=-=-.         .-=+*######*+=:.", color);
	LHPutAttrCharExLine(30, 16, "    ..   -- =: .--:=-. :=*%%%%%%%%%%%%%%%%*=.", color);
	LHPutAttrCharExLine(30, 17, "        .-  --   ++##*+%%%%%%%%%%%%%%%%%%%%%%*:", color);
	LHPutAttrCharExLine(30, 18, "        .   .-  .+%##############%%%%%%%%%%%%%%*.", color);
	LHPutAttrCharExLine(30, 19, "             :    +%%%#############%%%%%%%%%%%%%%-", color);
	LHPutAttrCharExLine(30, 20, "             .   =###%%##############%%%%%%%%%%%%%=", color);
	LHPutAttrCharEx(30, 21, "                =#####################%%%%%%%%%%%%%-", black, white);
	LHPutAttrCharEx(30, 22, "               :########******########%%%%%%%%%%%%%%.", black, white);
	LHPutAttrCharEx(30, 23, "               +#######********########%%%%%%%%%%%%%+", black, white);
	LHPutAttrCharEx(30, 24, "              .########********########%%%%%%%%%%%%%#", black, white);
	LHPutAttrCharEx(30, 25, "              .%#######********########%%%%%%%%%%%%%%.", black, white);
	LHPutAttrCharEx(30, 26, "              .%#########****#########%%%%%%%%%%%%%%%.", black, white);
	LHPutAttrCharEx(30, 27, "              .#%####################%%%%%%%%%%%%%%%#", black, white);
	LHPutAttrCharEx(30, 28, "               +%%##################%%%%%%%%%%%%%%%%+", black, white);
	LHPutAttrCharEx(30, 29, "               :%%%################%%%%%%%%%%%%%%%%%.", black, white);
	LHPutAttrCharEx(30, 30, "                =%%%%%##########%%%%%%%%%%%%%%%%%%%-", black, white);
	LHPutAttrCharEx(30, 31, "                 +%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%=", black, white);
	LHPutAttrCharEx(30, 32, "                  =%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%-", black, white);
	LHPutAttrCharEx(30, 33, "                   :*%%%%%%%%%%%%%%%%%%%%%%%%%%*.", black, white);
	LHPutAttrCharEx(30, 34, "                     -*%%%%%%%%%%%%%%%%%%%%%%*:", black, white);
	LHPutAttrCharEx(30, 35, "                       :=*%%%%%%%%%%%%%%%%*=.", black, white);
	LHPutAttrCharEx(30, 36, "                          .-=+*######*+=:.", black, white);

	LHPutAttrCharEx(x, y, "멀티모드", black, white);
	LHPutAttrCharEx(x, y + 1, "싱글모드", black, white);
	LHPutAttrCharEx(x, y + 2, "Back", black, white);
	LHPutAttrCharEx(x - 3, y + select, cursor, black, white);
}

bool OptionDraw(const int x, const int y, const int select, bool flag)
{
	LHPutAttrCharEx((cols - 54) / 2, 3, "   #####              ##       ##", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 4, "  ##   ##             ##", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 5, "  ##   ##  ######    #####    ###      ####    #####", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 6, "  ##   ##   ##  ##    ##       ##     ##  ##   ##  ##", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 7, "  ##   ##   ##  ##    ##       ##     ##  ##   ##  ##", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 8, "  ##   ##   #####     ## ##    ##     ##  ##   ##  ##", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 9, "   #####    ##         ###    ####     ####    ##  ##", black, white);
	LHPutAttrCharEx((cols - 54) / 2, 10, "           ####", black, white);

	LHPutAttrCharEx((cols - 61) / 2, 15, "1P 키보드 / 상 : W / 하 : S / 좌 : A / 우 : D / 확인 : SPACE", black, white);
	LHPutAttrCharEx((cols - 65) / 2, 17, "2P 키보드 / 상 : ↑ / 하 : ↓ / 좌 : ← / 우 : → / 확인 : ENTER", black, white);
	LHPutAttrCharEx((cols - 47) / 2, 19, "게임패드 / 상하좌우 : DPAD ←↑↓→ / 확인 : A", black, white);
	LHPutAttrCharEx(x, y, "1P 게임패드", black, white);
	LHPutAttrCharEx(x + 16, y, "1P 키보드", black, white);
	LHPutAttrCharEx(x + 32, y, "2P 게임패드", black, white);
	LHPutAttrCharEx(x + 48, y, "2P 키보드", black, white);
	LHPutAttrCharEx(x + 64, y, "Back", black, white);
	LHPutAttrCharEx(x - 3 + 16 * select, y, cursor, black, white);

	if (flag == true)
	{
		static int delay = 0;
		LHPutAttrCharEx(x + 15, 18, "처리되었습니다.", black, white);

		if (delay++ < 50)
		{
			return true;
		}

		delay = 0;
	}

	return false;
}

void InfoDraw(const int x, const int y)
{
	LHPutAttrCharEx((cols - 9) / 2, 5, "GameInfo", black, white);

	LHPutAttrCharEx((cols - 9) / 2 - 3, y, "<컨트롤러 설정>", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 13, y + 2, "기본은 w,s,a,d space / 방향키 enter", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 22, y + 4, "option에서 xbox컨트롤러를 사용하도록 설정할 수 있습니다", black, white);

	LHPutAttrCharEx((cols - 9) / 2, y + 7, "<플레이>", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 23, y + 9, "자신의 색 블럭을 파괴하면 일정확률로 능력치가 상승합니다", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 23, y + 11, "능력치에는 체력, 폭탄레벨, 최대설치 가능 개수가 있습니다", black, white);

	LHPutAttrCharEx((cols - 9) / 2 - 23, y + 13, "상대를 맞추면 폭탄레벨X5의 데미지를 입힙니다", black, white);
	//LHPutAttrCharEx((cols - 9) / 2, y + 11, "자신의 색을 파괴하면 +7, 상대의 색은-5", black, white);
	//LHPutAttrCharEx((cols - 9) / 2, y + 12, "매초 체력이 줄어들며 시간이 갈수록 증가폭이 커진다.", black, white);
	LHPutAttrCharEx((cols - 9) / 2, y + 16, "<licence>", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 12, y + 17, "Song : HEAVYgiant - Purification", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 14, y + 18, "Follow Artist : https://bit.ly/3kSgBGO", black, white);
	LHPutAttrCharEx((cols - 9) / 2 - 26, y + 19, "Music promoted by DayDreamSound : https://youtu.be/FBdcNL39ucI", black, white);
}

void MultiDraw(const int x, const int y, const int select)
{
	LHPutAttrCharEx((cols - 8) / 2, 5, "방 선택", black, white);
	LHPutAttrCharEx(x, y, "방 생성", black, white);
	LHPutAttrCharEx(x, y + 1, "방 입장", black, white);
	LHPutAttrCharEx(x, y + 2, "Back", black, white);
	LHPutAttrCharEx(x - 3, y + select, cursor, black, white);
}

void HostRoom(const int x, const int y, const shared_ptr<Server> host)
{
	LHPutAttrCharEx((cols - 82) / 2, 2, "  ##   ##                      ##              ######", black, white);
	LHPutAttrCharEx((cols - 82) / 2, 3, "  ##   ##                      ##               ##  ##", black, white);
	LHPutAttrCharEx((cols - 82) / 2, 4, "  ##   ##   ####     #####    #####             ##  ##   ####     ####    ##  ##", black, white);
	LHPutAttrCharEx((cols - 82) / 2, 5, "  #######  ##  ##   ##         ##               #####   ##  ##   ##  ##   #######", black, white);
	LHPutAttrCharEx((cols - 82) / 2, 6, "  ##   ##  ##  ##    #####     ##               ## ##   ##  ##   ##  ##   ## # ##", black, white);
	LHPutAttrCharEx((cols - 82) / 2, 7, "  ##   ##  ##  ##        ##    ## ##            ##  ##  ##  ##   ##  ##   ##   ##", black, white);
	LHPutAttrCharEx((cols - 82) / 2, 8, "  ##   ##   ####    ######      ###            #### ##   ####     ####    ##   ##", black, white);

	string str = "호스트 IP : ";
	str += host->get_host_ip();

	LHPutAttrCharEx((cols - str.length()) / 2, 15, str.c_str(), black, white);

	LHPutAttrCharEx(x, y + 2, "Back", black, white);
	LHPutAttrCharEx(x - 3, y + 2, cursor, black, white);

	if (host->get_client_count() != 0)
	{
		LHPutAttrCharEx((cols - 43) / 2, 17, "연결되었습니다! 잠시 후 게임이 시작됩니다.", black, white);
	}
	else
	{
		LHPutAttrCharEx((cols - 20) / 2, 17, "연결을 기다리는중..", black, white);
	}
}

void UserRoom(const int x, const int y, const int select, const string input, const bool start, bool& is_error)
{
	static int delay = 0;

	LHPutAttrCharEx((cols - 7) / 2, 5, "대기실", black, white);
	string buf;

	if (start == true)
	{
		buf = "연결되었습니다! 잠시 후 게임이 시작됩니다.";
	}
	else if (is_error == true)
	{
		buf = "연결에 실패했습니다. 다시 입력해주세요.";

		if (delay++ > 200)
		{
			is_error = false;
		}
	}
	else
	{
		buf = "연결할 호스트의 IP를 입력해주세요 : ";
		buf += input;
	}

	LHPutAttrCharEx((cols - buf.length()) / 2, y - 5, buf.c_str(), black, white);

	LHPutAttrCharEx((cols - 37) / 2, y - 2, " 0  1  2  3  4  5  6  7  8  9  .  ←", black, white);

	LHPutAttrCharEx(x, y, "Connect", black, white);
	LHPutAttrCharEx(x, y + 1, "Back", black, white);

	if (select > 19)
	{
		LHPutAttrCharEx(x - 3, y + select % 10, cursor, black, white);
	}
	else
	{
		LHPutAttrCharEx((cols - 37) / 2 + select * 3, y - 2, "[", black, yellow);
		LHPutAttrCharEx((cols - 37) / 2 + select * 3 + 2 + (select == 11), y - 2, "]", black, yellow);
	}
}

void EndingDraw(int winer)
{
	int location_x = (cols - 120) / 2;
	static int delay = 0;
	WORD color[64];
	int spark;

	if (delay++ < 50)
	{
		spark = white;
	}
	else
	{
		spark = black;

		if (delay == 100)
		{
			delay = 0;
		}
	}

	for (int i = 0; i < 64; i++)
	{
		if (i < 16)
		{
			color[i] = cyan;
		}
		else
		{
			color[i] = yellow;
		}
	}
	LHPutAttrCharEx(location_x, 1, ":::::-::-::::-:::::::::-:::-:::--:::::::::::::::::::::::::::::::--:-----======+++++******#######%%%%%%@%@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 2, ": ## ##            ## ##   ######  ## ##      ##   #### ###     #########  #### ###    ## *****#*####%%#%%@%%@@@@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 3, ":  ##  ##   ##    ##   ##    ##    ## ##      ##   ##      ##   ##         ##      ##  ###  ## *****####%#%%%%%%%%@%@@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 4, ":   ##  ## #  #  ##   ##     ##    ##  ##     ##   ##       #   ##         ##       #    #### ## ***#######%%%%%%%%%@@@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 5, ":    ##  ###  ###    ##      ##    ##   ##    ##   ###### ##    #########  ###### ##     # #### *****#####%%%%%%%%%@@@@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 6, ":     ##  ##    ##  ##       ##    ##    ##   ##   ##    ###    ##         ##    ###  #######   ##  ### ***##%##%%%%%%%%%%@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 7, ":      ##  ##  ##  ##        ##    ##     ##  ##   ##     ##    ##         ##     ##   ## ##   ## *####%#%%%%%%%%%@@@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 8, ":       ##### ######       ######  ###     ######  #####   ###  #########  #####   ###  ##   ## #####%#%%%%%%%%@@@@@@@@@@@@@@@@", darkgray, white);
	LHPutAttrCharEx(location_x, 9, "++++++=++++++*++++++++*****+=++++++*+*****+*+++++++=++***#********+::-----+***##%#%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@", darkgray, white);
	if (winer)
	{
		LHPutAttrCharEx((cols - 61) / 2, 15, "######   ####       ##     ##  ##   #######  ######    ####", black, yellow);
		LHPutAttrCharEx((cols - 61) / 2, 16, " ##  ##   ##       ####    ##  ##    ##   #   ##  ##  ##  ##", black, yellow);
		LHPutAttrCharEx((cols - 61) / 2, 17, " ##  ##   ##      ##  ##   ##  ##    ## #     ##  ##      #", black, yellow);
		LHPutAttrCharEx((cols - 61) / 2, 18, " #####    ##      ##  ##    ####     ####     #####     ###", black, yellow);
		LHPutAttrCharEx((cols - 61) / 2, 19, " ##       ##   #  ######     ##      ## #     ## ##    ##", black, yellow);
		LHPutAttrCharEx((cols - 61) / 2, 20, " ##       ##  ##  ##  ##     ##      ##   #   ##  ##  ##  ##", black, yellow);
		LHPutAttrCharEx((cols - 61) / 2, 21, "####     #######  ##  ##    ####    #######  #### ##  ######", black, yellow);
	}
	else
	{
		LHPutAttrCharEx((cols - 61) / 2, 15, "######   ####       ##     ##  ##   #######  ######     ##", black, cyan);
		LHPutAttrCharEx((cols - 61) / 2, 16, " ##  ##   ##       ####    ##  ##    ##   #   ##  ##   ###", black, cyan);
		LHPutAttrCharEx((cols - 61) / 2, 17, " ##  ##   ##      ##  ##   ##  ##    ## #     ##  ##    ##", black, cyan);
		LHPutAttrCharEx((cols - 61) / 2, 18, " #####    ##      ##  ##    ####     ####     #####     ##", black, cyan);
		LHPutAttrCharEx((cols - 61) / 2, 19, " ##       ##   #  ######     ##      ## #     ## ##     ##", black, cyan);
		LHPutAttrCharEx((cols - 61) / 2, 20, " ##       ##  ##  ##  ##     ##      ##   #   ##  ##    ##", black, cyan);
		LHPutAttrCharEx((cols - 61) / 2, 21, "####     #######  ##  ##    ####    #######  #### ##  ######", black, cyan);
	}
}