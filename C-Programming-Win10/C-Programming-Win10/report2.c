#include "8x8ascii.h"
#include "gba1.h"
#define CURRENT_TIME *((unsigned short *)0x0400010C)
#define KEY_IN (~(*(hword *)KEY_STATUS)&KEY_ALL)
/* 関数のプロトタイプ宣言 */
void locate(hword cx, hword cy);
void prints(unsigned char *str);
void draw_point(hword x, hword y, hword color);
void draw_cube(hword color);
void draw_food(hword fx, hword fy, hword color);
void display_time(hword val);
void init_timer(void);
hword random(hword ex, hword ey, hword px, hword py);
hword cx, cy;
hword px, py;//playerの位置
hword ex, ey;//敵1の位置
hword intArray[30][20];
point p;
hword *ptr;

unsigned char Rpackman[8];
/*
unsigned char packman[8][6] = {
	{ 0x3C,0x6E,0x6C,0x70,0x60,0x70,0xFE,0x7C },//packmanRight
	{ 0x3C,0x6E,0x6C,0x70,0x60,0x70,0xFE,0x7C },//packman
};*/



int main(void) {
while(1){
	/* GBA Display Mode Setting */
	*(hword *)VRAM = 0x0F03;	/* Set Mode3 */
	hword *ptr;
	hword cx = 10, cy = 10;	// 文字座標
	locate(cx, cy);
	hword num;
	hword flg = 0;
	hword flgAB = 3;
	num = 0;
	locate(3, 1);
	hword n, m;
	hword x, y = 0;//画面真っ黒にする
	hword randomNum;
	hword cnt = 0;//player点数
	hword Enemycnt = 0;//enemy点数
	hword Acnt = 0;//全部とったかの総数
	hword Eflg = 0;
	hword distanceX = 0;
	hword distanceY = 0;
	/* タイマ初期化 (0.01秒設定) */
	init_timer();


	/* タイマを起動 */
	*((hword *)0x04000102) = 0x0080;

	//こっちにタイトルを作る
	//画面をリセット
	for (y = 0; y < 160; y++) {
		for (x = 0; x < 240; x++) {
			draw_point(x, y, BGR(0x00, 0x00, 0x00));
		}
	}
	//スコアやタイマー表示画面
	locate(24, 2);
	prints("Enemy");
	locate(24, 3);
	prints("SCORE");
	locate(24, 7);
	prints("SCORE");
	locate(24, 11);
	prints("TIME");
	locate(24, 17);
	prints("PACN");
	locate(24, 18);
	prints(" CANDY");
	*((hword *)0x04000000) = 0x0F03;	/* Set Mode3 */
	ptr = (hword*)0x04000130;

	ex = 11; ey = 4;
	px = 11; py = 15;

	//画面の障害物用意
	/*locate(10, 10);
	draw_cube(BGR(0x1F, 0x00, 0x1F));*/


	//画面の障害物用意(壁)
	for (n = 0; n < 24; n++) {
		locate(n, 0);
		draw_food(n, 0, BGR(0x1F, 0x1F, 0x1F));
	}
	for (n = 0; n < 24; n++) {
		locate(n, 19);
		draw_food(n, 19, BGR(0x1F, 0x1F, 0x1F));
	}
	for (n = 0; n < 19; n++) {
		locate(0, n);
		draw_food(0, n, BGR(0x1F, 0x1F, 0x1F));
	}
	for (n = 0; n < 19; n++) {
		locate(23, n);
		draw_food(23, n, BGR(0x1F, 0x1F, 0x1F));
	}

	while (1) {
		locate(1, 1);
		prints("-Description-");
		locate(1, 3);
		prints("START:A key");
		locate(1, 5);
		prints("MOVE:arrow key");
		locate(1, 7);
		prints("Return Title:B key");
		locate(1, 9);
		prints("Player:P(=You)Enemy:E");
		locate(1, 13);
		prints("Clear1:You eat more");
		locate(20, 13);
		draw_food(0, 0, BGR(0x1F, 0x00, 0x1F));
		locate(1, 11);
		prints("Clear2:You eat all");
		locate(19, 11);
		draw_food(0, 0, BGR(0x1F, 0x00, 0x1F));
		locate(1, 15);
		prints("Game Over1:E eat You");
		locate(1, 17);
		prints("Game Over2:E eat more");
		locate(22, 17);
		draw_food(0, 0, BGR(0x1F, 0x00, 0x1F));
		if (KEY_IN == KEY_A) {

			//画面をリセット
			for (y = 8; y < 152; y++) {
				for (x = 8; x < 184; x++) {
					draw_point(x, y, BGR(0x00, 0x00, 0x00));
				}
			}
			break;
		}
	}

	//食料配置
	locate(20, 17);
	draw_food(20, 17, BGR(0x1F, 0x00, 0x1F));
	locate(20, 2);
	draw_food(20, 2, BGR(0x1F, 0x00, 0x1F));
	locate(2, 17);
	draw_food(2, 17, BGR(0x1F, 0x00, 0x1F));
	locate(2, 2);
	draw_food(2, 2, BGR(0x1F, 0x00, 0x1F));
	locate(18, 15);
	draw_food(18, 15, BGR(0x1F, 0x00, 0x1F));
	locate(18, 4);
	draw_food(18, 4, BGR(0x1F, 0x00, 0x1F));
	locate(4, 15);
	draw_food(4, 15, BGR(0x1F, 0x00, 0x1F));
	locate(4, 4);
	draw_food(4, 4, BGR(0x1F, 0x00, 0x1F));

	/*//食料配置
	for (n = 2; n < 18; n++) {
		for (m = 2; m < 20; m++) {
			draw_food(n, m, BGR(0x1F, 0x00, 0x1F));
		}
	}*/
	locate(11, 15);
	prints("P");
	//キー入力判定
	while (1) {


		//画面をパックマンが動く
		switch (KEY_IN) {
		case KEY_LEFT:
			if (px > 1) {
				locate(px, py);
				prints(" ");
				px--;
				locate(px, py);
				prints("P");
				flg = 1;
			}
			break;
		case KEY_RIGHT:
			if (px < 22) {
				locate(px, py);
				prints(" ");
				px++;
				locate(px, py);
				prints("P");
				flg = 1;
			}
			break;
		case KEY_UP:
			if (py > 1) {
				locate(px, py);
				prints(" ");
				py--;
				locate(px, py);
				prints("P");
				flg = 1;
			}
			break;
		case KEY_DOWN:
			if (py < 18) {
				locate(px, py);
				prints(" ");
				py++;
				locate(px, py);
				prints("P");
				flg = 1;
			}
			break;
		default://flg = 0;
			break;

		}

		//敵1を動かす
		/*randomNum = random(px, py, ex, ey);
		if (randomNum==0) {
			if (ex > 2) {
				locate(ex, ey);
				prints(" ");
				ex++;
				locate(ex, ey);
				prints("E");
			}
			else {
				randomNum = 1;
			}
		}
		if (randomNum == 1) {
			if (ex < 21) {
				locate(ex, ey);
				prints(" ");
				ex--;
				locate(ex, ey);
				prints("E");
			}
			else {
				randomNum = 2;
			}
		}
		if (randomNum == 2) {
			if (ey > 1) {
				locate(ex, ey);
				prints(" ");
				ey--;
				locate(ex, ey);
				prints("E");
			}
			else {
				randomNum =3;
			}
		}
		if (randomNum == 3) {
			if (ey < 18) {
				locate(ex, ey);
				prints(" ");
				ey++;
				locate(ex, ey);
				prints("E");
			}
			else {
				randomNum = 0;
			}
		}
		*/

		//player と　敵の距離　を定義
		if (px >= ex) {
			distanceX = px - ex;
		}
		else if (px < ex) {
			distanceX = ex - px;
		}
		if (py >= ey) {
			distanceY = py - ey;
		}
		else if (py < ey) {
			distanceY = ey - py;
		}

		if (Eflg == 0) {
			if (distanceX >= distanceY) {
				if (ex <= px) {
					locate(ex, ey);
					prints(" ");
					ex++;
					locate(ex, ey);
					prints("E");
				}
				else if (ex > px) {
					locate(ex, ey);
					prints(" ");
					ex--;
					locate(ex, ey);
					prints("E");
				}
			}
			else if (distanceX < distanceY) {
				if (ey <= py) {
					locate(ex, ey);
					prints(" ");
					ey++;
					locate(ex, ey);
					prints("E");
				}
				else if (ey > py) {
					locate(ex, ey);
					prints(" ");
					ey--;
					locate(ex, ey);
					prints("E");
				}
			}
			Eflg = 1;
		}
		else if (Eflg == 1) {
			Eflg = 0;
			for (n = 0; n < 10000; n++) {
			}
		}

		//game over 画面
		if (ex == px) {
			if (ey == py) {
				//画面をリセット
				for (y = 8; y < 152; y++) {
					for (x = 8; x < 184; x++) {
						draw_point(x, y, BGR(0x00, 0x00, 0x00));
					}
				}
				//クリア表示
				locate(6, 10);
				prints("Game Over!");
				locate(2, 15);
				prints("Return Title :B key");
				/* 右上にタイム表示 */
				locate(26, 12);
				display_time(CURRENT_TIME);
				/* タイマを停止 */
				*((hword *)0x04000102) = 0x0000;
				break;
			}
		}

		//時間遅らせ
		for (n = 0; n < 10000; n++) {
		}

		//PlayerのSCORE表示
		if (cnt == 0) {
			locate(27, 8);
			prints("000");
		}
		if (cnt == 1) {
			locate(27, 8);
			prints("100");
		}
		if (cnt == 2) {
			locate(27, 8);
			prints("200");
		}
		if (cnt == 3) {
			locate(27, 8);
			prints("300");
		}
		if (cnt == 4) {
			locate(27, 8);
			prints("400");
		}
		if (cnt == 5) {
			locate(27, 8);
			prints("500");
		}
		if (cnt == 6) {
			locate(27, 8);
			prints("600");
		}
		if (cnt == 7) {
			locate(27, 8);
			prints("700");
		}
		if (cnt == 8) {
			locate(27, 8);
			prints("800");
		}
		//EnemyのSCORE表示
		if (Enemycnt == 0) {
			locate(27, 5);
			prints("000");
		}
		if (Enemycnt == 1) {
			locate(27, 5);
			prints("100");
		}
		if (Enemycnt == 2) {
			locate(27, 5);
			prints("200");
		}
		if (Enemycnt == 3) {
			locate(27, 5);
			prints("300");
		}
		if (Enemycnt == 4) {
			locate(27, 5);
			prints("400");
		}
		if (Enemycnt == 5) {
			locate(27, 5);
			prints("500");
		}
		if (Enemycnt == 6) {
			locate(27, 5);
			prints("600");
		}
		if (Enemycnt == 7) {
			locate(27, 5);
			prints("700");
		}
		if (Enemycnt == 8) {
			locate(27, 5);
			prints("800");
		}

		//クリアチェック
		if (intArray[px][py] == 1) {
			intArray[px][py] = 0;
			cnt++;
			Acnt++;
		}
		if (intArray[ex][ey] == 1) {
			intArray[ex][ey] = 0;
			Enemycnt++;
			Acnt++;
		}

		if (Acnt == 8) {
			/* 右上にタイム表示 */
			locate(26, 12);
			display_time(CURRENT_TIME);
			/* タイマを停止 */
			*((hword *)0x04000102) = 0x0000;


			//画面をリセット
			for (y = 8; y < 152; y++) {
				for (x = 8; x < 184; x++) {
					draw_point(x, y, BGR(0x00, 0x00, 0x00));
				}
			}
			if (cnt == 8) {
				locate(27, 8);
				prints("800");
			}
			//クリア表示
			if (cnt > Enemycnt) {
				locate(9, 10);
				prints("You Win!");
				locate(2, 15);
				prints("Return Title :B key");
			}
			else if (cnt <= Enemycnt) {
				locate(7, 10);
				prints("You lose!");
				locate(2, 15);
				prints("Return Title :B key");
			}
			break;
		}
	}

	while (1) {
		if (KEY_IN == KEY_B) {
			break;
		}
	}
}
	return 0;

}


/* 文字座標(0, 0)～(29,19)の位置(cx, cy)を画面座標に変換する関数 */
void locate(hword cx, hword cy) {
	if (cx < 30) {
		p.x = cx << 3;			/* x座標の指定 */
	}
	else {
		p.x = 30 - 1;			/* xの最大値を設定 */
	}
	if (cy < 20) {
		p.y = cy << 3;			/* y座標の指定 */
	}
	else {
		p.y = 20 - 1;			/* yの最大値を設定 */
	}
}


/* locateで設定された位置にASCIIコード*strに相当する文字を表示する関数 */
void prints(unsigned char *str) {

	hword tx, ty;			/* 文字パターンの座標指定 */
	unsigned char cbit;						/* ビット判定 */
	hword i = 0;

	while (*str != 0x00) { 	//i<14の条件いらず

		for (ty = 0; ty < 8; ty++) {			/* 文字パターンのy座標指定 */

			cbit = 0x80;						/* 判定ビットを8ビットの最上位にセット */

			for (tx = 0; tx < 8; tx++) {							/* 文字パターンのx座標指定 */
				if ((char8x8[*str][ty] & cbit) == cbit) {			/* ビット位置の判定 */
					draw_point((p.x + tx + i * 8), (p.y + ty), BGR(0x00, 0x1F, 0x1F));	// 1なら白
				}
				else {
					draw_point((p.x + tx + i * 8), (p.y + ty), BGR(0x00, 0x00, 0x00));	// 0なら黒
				}
				cbit = cbit >> 1;					/* 判定ビットを1ビット右へシフト */

			}

		}
		str++;
		i++;


	}

}
/* 割り算の商を求める関数*/
hword div(hword dividened, hword divisor) {

	hword quotient = 0;			/* 商 */

	while (1) {
		if (dividened >= divisor) {
			dividened = dividened - divisor;
			quotient++;
		}
		else {
			break;
		}
	}
	return quotient;
}


/* 割り算の余りを求める関数*/
hword mod(hword dividened, hword divisor) {

	hword quotient = 0;			/* 商 */

	while (1) {
		if (dividened >= divisor) {
			dividened = dividened - divisor;
			quotient++;
		}
		else {
			break;
		}
	}
	return dividened;
}
/*1~4の乱数を生成する*/
hword random(hword ex,hword ey,hword px,hword py) {
	hword max1,max2,max3;
	if (px > ey) {
		max1 = ey;
	}
	else {
		max1 = px;
	}
	if (py > ex) {
		max2 = ex;
	}
	else {
		max2 = py;
	}
	if (max1 > max2) {
		max3 = max1;
	}
	else {
		max3 = max1;
	}

	return mod((2 * max3 + 3),3);
}


/* 指定した座標(x, y) に指定した色colorの点を描画する関数 */
void draw_point(hword x,hword y,hword color) {
	ptr = (hword*)VRAM;						/* VRAM Address(0,0) */
	ptr = ptr + x + (y * 240);		/* Display 240x160 */
	*ptr = color;
}

void draw_cube(hword color) {
	hword tx, ty;			/* 文字パターンの座標指定 */
	hword i = 0;
	for (ty = 0; ty < 8; ty++) {			/* 文字パターンのy座標指定 */
		for (tx = 0; tx < 8; tx++) {/* 文字パターンのx座標指定 */
			draw_point((p.x + tx), (p.y + ty), color);	// 1なら白
		}
		i++;
	}
}

void draw_food(hword fx,hword fy,hword color) {
	/* 文字パターンの座標指定 */
	hword tx, ty;
	for (ty = 0; ty < 8; ty++) {			/* 文字パターンのy座標指定 */
		for (tx = 0; tx < 8; tx++) {/* 文字パターンのx座標指定 */
			if (tx > 2 & tx < 6) {
				if (ty > 2 & ty < 6) {
					draw_point((p.x + tx), (p.y + ty), color);

				}
		}
	}

	}
	intArray[fx][fy]=1;
}

void display_time(hword val) {

	byte char_data[] = "0123456789";
	byte buf[6];
	hword tmp;
	int i;

	/* 入力値valの桁数(i+1)設定 (i=3の場合4桁表示)*/
	i = 3;

	/* 文字列の最後にNULLコード挿入 */
	buf[i + 1] = 0;

	/* 最下位桁の文字（10で割った余り）を挿入し，入力値を10で割る */
	for (; i >= 0; i--) {
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}

	/* 文字列全体を表示 */
	prints(buf);

	return;
}


void init_timer(void) {

	/* タイマ制御レジスタ */
	*((hword *)0x04000100) = 0xFFFF - 1678 + 1;
	*((hword *)0x04000104) = 0xFFFF - 10 + 1;
	*((hword *)0x04000108) = 0xFFFF - 10 + 1;
	*((hword *)0x0400010C) = 0x0000;

	/* タイマ設定レジスタ */
	*((hword *)0x04000102) = 0x0000;	// 初期状態でタイマ停止
	*((hword *)0x04000106) = 0x0084;	// カスケード接続
	*((hword *)0x0400010A) = 0x0084;	// カスケード接続
	*((hword *)0x0400010E) = 0x0084;	// カスケード接続
}