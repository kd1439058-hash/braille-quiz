#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(1250, 700, 32);

	ChangeWindowMode(TRUE);		// ウィンドウモードで起動

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	srand((unsigned int)time(NULL));

	float x[6] = {//点字の○のx座標

		555,555,555,695,695,695

	};

	float y[6] = {	//点字の○のy座標

		210,350,490,210,350,490

	};

	int radius = 60;//点字の○の半径

	int x_mouse;//マウスのx座標

	int y_mouse;//マウスのy座標

	int frag = 0;//０＝回答中　１＝正解　２＝不正解
	int hint_frag = 0;//ヒントを表示する　０＝非表示、１＝表示
	int i = 0;//ループカウンタ

	int color_white = GetColor(255, 255, 255);
	int color_black = GetColor(0, 0, 0);
	int color_blue = GetColor(0, 0, 255);
	int color_green = GetColor(0, 255, 0);
	int color_red = GetColor(255, 0, 0);
	int color_gray = GetColor(128, 128, 128);
	int color_cyan = GetColor(0, 255, 255);
	int color_magent = GetColor(255, 0, 255);
	int color_yello = GetColor(255, 255, 0);


	int pattern[][6] = {																		//各種点字のパターン　上から下、左から右に点字の○を１～６を割り当てている
	/*あ行*/{1,0,0,0,0,0},{1,1,0,0,0,0},{1,0,0,1,0,0},{1,1,0,1,0,0},{0,1,0,1,0,0},
	/*か行*/{1,0,0,0,0,1},{1,1,0,0,0,1},{1,0,0,1,0,1},{1,1,0,1,0,1},{0,1,0,1,0,1},
	/*さ行*/{1,0,0,0,1,1},{1,1,0,0,1,1},{1,0,0,1,1,1},{1,1,0,1,1,1},{0,1,0,1,1,1},
	/*た行*/{1,0,1,0,1,0},{1,1,1,0,1,0},{1,0,1,1,1,0},{1,1,1,1,1,0},{0,1,1,1,1,0},
	/*な行*/{1,0,1,0,0,0},{1,1,1,0,0,0},{1,0,1,1,0,0},{1,1,1,1,0,0},{0,1,1,1,0,0},
	/*は行*/{1,0,1,0,0,1},{1,1,1,0,0,1},{1,0,1,1,0,1},{1,1,1,1,0,1},{0,1,1,1,0,1},
	/*ま行*/{1,0,1,0,1,1},{1,1,1,0,1,1},{1,0,1,1,1,1},{1,1,1,1,1,1},{0,1,1,1,1,1},
	/*や行*/{0,0,1,1,0,0},{0,0,1,1,0,1},{0,0,1,1,1,0},
	/*ら行*/{1,0,0,0,1,0},{1,1,0,0,1,0},{1,0,0,1,1,0},{1,1,0,1,1,0},{0,1,0,1,1,0},
	/*わ行*/{0,0,1,0,0,0},{0,0,1,0,1,0},{0,0,1,0,1,1},
	};

	

	char tenji_name[][5] = {	//点字の名前
		"あ","い","う","え","お",	//04
		"か","き","く","け","こ",	//59
		"さ","し","す","せ","そ",	//1014
		"た","ち","つ","て","と",	//1519
		"な","に","ぬ","ね","の",	//2024
		"は","ひ","ふ","へ","ほ",	//2529
		"ま","み","む","め","も",	//3034
		"や","ゆ","よ",				//3537
		"ら","り","る","れ","ろ",	//3842
		"わ","を","ん",				//4345
	};
	

	int haikei = LoadGraph("c14b49109e7cb8ba05b895cbcf790890.png");//黒板の画像
	int hint = LoadGraph("tenji.png");//点字の答え（ヒント）
	int haikei2;//切り取られた黒板
	haikei2 = DerivationGraph(20, 10, 780, 400, haikei);//切り取られた黒板
	DeleteGraph(haikei);

	int currentpattern[6] = { 0,0,0,0,0,0 };	//画面上に表示される点字のパターン
	int mouseinput = 0;//マウスを押したどうか
	int keyinput = 0;//キーを押したかどうか
	int mouse_old = 0;//前週にマススを押したかどうか
	int ran = rand() % 46;//乱数
	float drawx = 927.5;//ボタンのx座標
	float drawy = 200;//ボタンのy座標
	int fontsize = 50;//フォントサイズ

	int FontHandle; ;//フォント
	
	FontHandle = CreateFontToHandle("Chalk-JP.otf", 50, 1, DX_FONTTYPE_NORMAL);//フォント

	//フォントが読み込まれない場合終了する
	if (FontHandle == -1) {

		DxLib::DxLib_End();
		
		return -1; 
	}

	//黒板を表示する
	DxLib::DrawExtendGraph(0, 0, 1250, 700, haikei, FALSE);

	//背景（黒板）が読み込まれない場合終了する
	if (haikei == -1) {

		DxLib::DxLib_End();

		return -1;
	}




	

		

		

	

	while (ProcessMessage() == 0) {




		
		GetMousePoint(&x_mouse, &y_mouse);

		mouseinput = GetMouseInput();

		//左マウスが押されており、前週でマウスが押されていない
		if (mouseinput & MOUSE_INPUT_LEFT && (mouse_old & MOUSE_INPUT_LEFT) == 0) {

			if (frag==0) {//回答中の時のみ
				if (x_mouse > x[0] - radius && x_mouse < x[0] + radius) {
					//左列が押された

					if (y_mouse > y[0] - radius && y_mouse < y[0] + radius) {		//1
						currentpattern[0] = 1 - currentpattern[0];
						//点字の１番を反転させる
					}	
					else if (y_mouse > y[1] - radius && y_mouse < y[1] + radius) {	//2
						currentpattern[1] = 1 - currentpattern[1];
						//点字の２番を反転させる
					}
					else if (y_mouse > y[2] - radius && y_mouse < y[2] + radius) {	//3
						currentpattern[2] = 1 - currentpattern[2];
						//点字の３番を反転させる
					}
					
				}


				else if (x_mouse > x[3]-radius && x_mouse < x[3]+radius) {
					//右列が押された

					if (y_mouse > y[0]-radius && y_mouse < y[0]+radius) {		//4
						currentpattern[3] = 1 - currentpattern[3];
						//点字の４番を反転させる
					}
					else if (y_mouse > y[1] - radius && y_mouse < y[1] + radius) {	//5
						currentpattern[4] = 1 - currentpattern[4];
						//点字の５番を反転させる
					}
					else if (y_mouse > y[2] - radius && y_mouse < y[2] + radius) {	//6
						currentpattern[5] = 1 - currentpattern[5];
						//点字の６番を反転させる
					}
			
				}
			}


			//判定ボタンが押されたとき正解か不正解か判断する
			if (x_mouse > drawx && x_mouse < drawx + fontsize * 3 && y_mouse>drawy + (fontsize + 20) && y_mouse < drawy + (fontsize + 20) + fontsize) {
				
				for (i = 0; i <= 5; i++) {
					//入力した点字が正解の展示と等しいか判断
					if (currentpattern[i] == pattern[ran][i]) {
						frag = 1;
					}
					else {
						frag = 2;
						break;
					}

				}
			
			}

			//次へボタンが押されたとき次の問題を表示する
			if (x_mouse > drawx && x_mouse < drawx + fontsize * 3 && y_mouse>drawy + (fontsize + 20) * 2 && y_mouse < drawy + (fontsize + 20) * 2 + fontsize) {

				for (i = 0; i <= 5; i++) {
					currentpattern[i] = 0;
				}
					ran = rand() % 46;
					frag = 0;
			}

			
				//ヒントボタンが押されたときヒントを表示する
			if (x_mouse > drawx && y_mouse > drawy && x_mouse < drawx + fontsize * 3 && y_mouse < drawy + fontsize ) {
				
					hint_frag = 1 - hint_frag;
			
			}
			
				

		}
		char Buf[256];
		GetHitKeyStateAll(Buf);

		//ESCを押したときヒントを表示/非表示にする
		if ( CheckHitKey(KEY_INPUT_ESCAPE) && (keyinput & KEY_INPUT_ESCAPE) == 0) {
			hint_frag = 1 - hint_frag;
			
		}

		if (Buf[KEY_INPUT_ESCAPE] == 1) {
			keyinput = 1;
		}
		else {
			keyinput = 0;
		}

		mouse_old = mouseinput;





		ClearDrawScreen();

		//黒板を表示する
		DrawExtendGraph(0, 0, 1250, 700, haikei2, FALSE);
		


		for (i = 0; i <= 5; i++) {

			if (currentpattern[i] == 0) {
				//点字の数列が0の時、中を開けて丸を表示する
					DrawCircleAA(x[i], y[i], radius,32, color_white, FALSE);
			}
			else {
				//点字の数列が１の時、丸を表示する
					DrawCircleAA(x[i], y[i], radius,32, color_white, TRUE);
			}

		}
			
		
		
		SetFontSize(fontsize);

		//背景
		if (x_mouse > drawx && y_mouse > drawy && x_mouse < drawx + fontsize * 3 && y_mouse < drawy + fontsize) {
			//マウスがボタン上に来た時薄くする
			DrawBoxAA(drawx, drawy, drawx + fontsize * 3, drawy + fontsize, GetColor(255,200,100), TRUE);
		
		}
		else {
			//通常の状態で表示する
			DrawBoxAA(drawx, drawy, drawx + fontsize * 3, drawy + fontsize, color_red, TRUE);
	
		}
	
		//枠
		DrawBoxAA(drawx, drawy, drawx + fontsize * 3, drawy + fontsize, color_white, FALSE,3);
		//ヒント
		


		//背景
		if (x_mouse > drawx && y_mouse > drawy + (fontsize + 20) && x_mouse < drawx + fontsize * 3 && y_mouse < drawy + (fontsize + 20) + fontsize) {
			//マウスがボタン上に来た時薄くする
			DrawBoxAA(drawx, drawy + (fontsize + 20), drawx + fontsize * 3, drawy + (fontsize + 20) + fontsize, GetColor(100,255,200), TRUE);
		
		}
		else {
			//通常の状態で表示する
			DrawBoxAA(drawx, drawy + (fontsize + 20), drawx + fontsize * 3, drawy + (fontsize + 20) + fontsize, color_green, TRUE);
		
		}
		
		//枠
		DrawBoxAA(drawx, drawy + (fontsize + 20), drawx + fontsize * 3, drawy + (fontsize + 20) + fontsize, color_white, FALSE,3);
		//判定
		



		//背景
		if (x_mouse > drawx && y_mouse > drawy + (fontsize + 20) * 2 && x_mouse < drawx + fontsize * 3 && y_mouse < drawy + (fontsize + 20) * 2 + fontsize) {
			//マウスがボタン上に来た時薄くする
			DrawBoxAA(drawx, drawy + (fontsize + 20) * 2, drawx + fontsize * 3, drawy + (fontsize + 20) * 2 + fontsize, GetColor(200,100,255), TRUE);
		
		}
		else {
			//通常の状態で表示する
			DrawBoxAA(drawx, drawy + (fontsize + 20) * 2, drawx + fontsize * 3, drawy + (fontsize + 20) * 2 + fontsize, color_blue, TRUE);
		
		}
		
		//枠
		DrawBoxAA(drawx, drawy + (fontsize + 20) * 2, drawx + fontsize * 3, drawy + (fontsize + 20) * 2 + fontsize, color_white, FALSE,3);
		//次



		


		//背景
		DrawBoxAA(drawx, drawy + (fontsize + 40) * 3, drawx + fontsize * 3, drawy + (fontsize + 40) * 3 + fontsize, color_gray, TRUE);
		
		//枠
		DrawBoxAA(drawx, drawy + (fontsize + 40) * 3, drawx + fontsize * 3, drawy + (fontsize + 40) * 3 + fontsize, color_white, FALSE, 3);
		//正解不正解
		



		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		//表示を透明にする

		DrawStringToHandle(drawx, drawy, "ヒント", color_white, FontHandle);
		//ヒント

		DrawStringToHandle(drawx + fontsize / 2, drawy + fontsize + 20, "判定", color_white, FontHandle);
		//判定

		DrawStringToHandle(drawx + fontsize, drawy + (fontsize + 20) * 2, "次", color_white, FontHandle);
		//次の問題



		if (frag == 1) {

			DrawFormatStringToHandle(drawx + fontsize / 2, drawy + (fontsize + 40) * 3, GetColor(255, 215, 0), FontHandle, "正解");
		//正解
		}
		else if (frag == 2) {

			DrawFormatStringToHandle(drawx, drawy + (fontsize + 40) * 3, color_cyan, FontHandle, "不正解");
		//不正解
		}
		


		

		
		DrawFormatStringToHandle(275, 620, color_white, FontHandle, "この点字を「%s」にしてみよう", tenji_name[ran]);
		//14文字  問題を表示
		if (hint_frag == 1) {
			
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			//半透明にする
			DrawExtendGraph(27, 30, 400, 600, hint, TRUE);
			//ヒントを表示する
			
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//透明度を元に戻す
		
	
		

		ScreenFlip();
		//画面に表示する

		

		}
		
	DeleteFontToHandle(FontHandle);
	DeleteGraph(haikei2);
	DeleteGraph(hint);

	DxLib::DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
