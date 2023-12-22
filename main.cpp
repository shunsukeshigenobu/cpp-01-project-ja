//参考ソースコード[スネークゲーム]　http://vivi.dyndns.org/tech/cpp/console.html
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <time.h>
#include <windows.h>

using namespace std;

typedef unsigned int uint;
typedef pair<int, int> CurPos;			//	座標値タイプ定義

#define		CONS_WD		80
#define		CONS_HT		25
#define		POINT_gas		100		//	ガソリン補給ポイント
#define		POINT_TIME		10			//	一定時間毎のポイント
#define		TERM				10			//	一定期間
#define		N_gas			10			//	初期ガソリンスタンド数

//----------------------------------------------------------------------
//		色定義
#define		COL_BLACK	0x00
#define		COL_DARK_BLUE		0x01
#define		COL_DARK_GREEN	0x02
#define		COL_DARK_CYAN		0x03
#define		COL_DARK_RED		0x04
#define		COL_DARK_VIOLET	0x05
#define		COL_DARK_YELLOW	0x06
#define		COL_GRAY	0x07
#define		COL_LIGHT_GRAY		0x08
#define		COL_BLUE		0x09
#define		COL_GREEN	0x0a
#define		COL_CYAN		0x0b
#define		COL_RED		0x0c
#define		COL_VIOLET	0x0d
#define		COL_YELLOW	0x0e
#define		COL_WHITE	0x0f
#define		COL_INTENSITY		0x08	
#define		COL_RED_MASK		0x04
#define		COL_GREEN_MASK	0x02
#define		COL_BLUE_MASK	0x01

//	文字色指定 for Windows Console
void setColor(int col)
{
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	WORD attr = 0;
	if( col & COL_INTENSITY )
		attr |= FOREGROUND_INTENSITY;
	if( col & COL_RED_MASK )
		attr |= FOREGROUND_RED;
	if( col & COL_GREEN_MASK )
		attr |= FOREGROUND_GREEN;
	if( col & COL_BLUE_MASK )
		attr |= FOREGROUND_BLUE;
	SetConsoleTextAttribute(hCons, attr);
}
void setColor(int fg, int bg)
{
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	WORD attr = 0;
	if( fg & COL_INTENSITY )
		attr |= FOREGROUND_INTENSITY;
	if( fg & COL_RED_MASK )
		attr |= FOREGROUND_RED;
	if( fg & COL_GREEN_MASK )
		attr |= FOREGROUND_GREEN;
	if( fg & COL_BLUE_MASK )
		attr |= FOREGROUND_BLUE;
	
	if( bg & COL_INTENSITY )
		attr |= BACKGROUND_INTENSITY;
	if( bg & COL_RED_MASK )
		attr |= BACKGROUND_RED;
	if( bg & COL_GREEN_MASK )
		attr |= BACKGROUND_GREEN;
	if( bg & COL_BLUE_MASK )
		attr |= BACKGROUND_BLUE;
	SetConsoleTextAttribute(hCons, attr);
}
//カーソルによる位置指定
void setCursorPos(int x, int y)
{
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hCons, pos);
}
//key 取得
bool isKeyPressed(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) != 0; /*なぜ0x8000が必要？？*/
}
//ガソリンスタンド追加
void add_gas(vector<CurPos> &gas)
{
		int x = (CONS_WD - 10);
		int y = (CONS_HT - 10);
		gas.push_back(CurPos(x, y));
		setCursorPos(gas[0].first, gas[0].second);
		cout << "g";
		x = (CONS_WD - 20);
		y = (CONS_HT - 18);
		gas.push_back(CurPos(x, y));
		setCursorPos(gas[0].first, gas[0].second);
		cout << "g";
	}
//地図外壁の表示
void print_field()
{
	setColor(COL_YELLOW, COL_YELLOW);
	setCursorPos(0, 1);
	for (int i = 0; i < CONS_WD; ++i) {
		cout << " ";
	}
	for (int y = 2; y < CONS_HT - 2; ++y) {
		setCursorPos(0, y);
		cout << " ";
		setCursorPos(CONS_WD - 1, y);
		std::cout << " ";
	}
	setCursorPos(0, CONS_HT - 2);		//	最下行のひとつ上
	for (int i = 0; i < CONS_WD; ++i) {
		std:cout << " ";
	}
	setColor(COL_YELLOW, COL_BLACK);		//	背景黒
	for (int y = 0; y < CONS_HT - 4; ++y) {
		setCursorPos(1, y + 2);
		for (int x = 0; x < CONS_WD - 2; ++x) {
			std::cout << " ";
		}
	}
	setCursorPos(0, CONS_HT - 1);		//	最下行
	for (int i = 0; i < CONS_WD - 1; ++i) {
		std::cout << " ";
	}
}
//車の描写する関数 後ろは空白を入れてる
void print_vehicle(const deque<CurPos> &vehicle)
{
	setColor(COL_VIOLET);
	setCursorPos(vehicle[0].first,vehicle[0].second);
	cout << "@";		//	車
	}
//車の位置を更新 実装の意味なし？
 void update_vehicle(deque<CurPos> &vehicle, int x, int y){
 	vehicle.push_front(CurPos(x, y));//車を新しい座標に
 	setCursorPos(vehicle.back().first, vehicle.back().second);//元の座標のV消去
 		cout << " ";
 		vehicle.pop_back();
 }
//	壁に車がぶつかったかチェック（外壁）
bool collapsed(const deque<CurPos> &vehicle)
{
	int x = vehicle[0].first;
	int y = vehicle[0].second;
	if( x <= 0 || x >= CONS_WD - 1 || y <= 1 || y >= CONS_HT - 2 ){
		return true;		//	周辺の壁に衝突
	}else{
	return false;
	}		//	衝突無し
}

int main(){
	srand((int)time(0));	//時間を取得;
	int stop_flag = 0;
	int display_flag = 0;
   for (;;) { //Contenueしたらここに戻る
		if (display_flag == 0){
			std::cout << "'@'=vehicle" <<std::endl;
			std::cout << "'g'= gasstatiom" <<std::endl;
			std::cout << "START'S'" <<std::endl;
			std::cout << "Turn light'H'" <<std::endl;
			std::cout << "Turn Right'L'" <<std::endl;
			std::cout << "Speed UP'U'" <<std::endl;
			std::cout << "Turn douwn'D'" <<std::endl;
			std::cout << "BACK'J'" <<std::endl;
			std::cout << "Stop'T'" <<std::endl;
			display_flag = +1;
		}
		if(isKeyPressed('S')){
		int score = 0;		//	スコアを０に初期化 後々実装
		int x = CONS_WD / 2;		//	初期位置座標真ん中
		int y = CONS_HT -3 ;		//　初期座標下から3番目
		int dx = 0, dy = -1;		//	速度ベクトル
		int speed = 2000;
        deque<CurPos> vehicle;		//	車両座標
	    vector<CurPos> gas;			//ガソリンスタンド座標		
		vehicle.push_front(CurPos(x, y-2));
		vehicle.push_front(CurPos(x, y-1));
		vehicle.push_front(CurPos(x, y));
        print_field();
		print_vehicle(vehicle);
		add_gas(gas);
	//for(;;)
	//if(stop_flag == 0)
		for (int cnt = 0; ; ++cnt) {	//方向転換,スピード変更
			//	キーによる方向転換処理、スピードUP、SPEEDDOWN
			if( isKeyPressed('H')) {
				dx = -1;
				dy = 0;
			} else if(isKeyPressed('J')) {
				dx = 0;
				dy = 1;
			} else if(isKeyPressed('K')) {
				dx = 0;
				dy = -1;
			} else if(isKeyPressed('L')) {
				dx = 1;
				dy = 0;
			} else if( isKeyPressed('U')){
			speed = speed - 200 ;
			}else if( isKeyPressed('D')){
			speed = speed + 200 ;
			// }else if ( isKeyPressed('T')){　//後々実装
			// 	break;
			}
			x += dx;
			y += dy;
			print_vehicle(vehicle);
			update_vehicle(vehicle,x,y);
			if( collapsed(vehicle) ) {
				cout << (char)0x07 << (char)0x07 << (char)0x07;		//	ビープ音*３
				break;
				}
			Sleep(speed);		//	Speed分wait
		}
		setCursorPos(0, CONS_HT - 1);
		cout << "try again ?";
		for (;;) {
			if( isKeyPressed('Y') )
				break;
			if( isKeyPressed('N') )
				return 0;
		}
		}
   }
	return 0;
}

