#define _CRT_SECURE_NO_WARNINGS
#define width 10
#define height 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pausemsg.h"

char B_field[height][width]; // sea of bot
int B_matrix[4][2]; // helpful matrix 

int B_ban[1000][2];
int B_last_point = 0;
int ship_coord_B[20][2];
int Life_B = 20;


int P_matryx[4][2];

int P_ban[1000][2];
int P_last_point = 0;
int ship_coord_P[20][2];
char P_T_field[height][width]; //display of player's targets
char P_field[height][width]; //sea of player
int Life_P = 20;

char Player_Field[10][10];

bool finish_ship = false;
bool kill_ship_P = false;
int history_of_strike[4][2] = { -1 };
int count_of_history = 0;
int next_strike = 0;
int sdv = 0;//переменна€ дл€ возврата по истории
int deck_of_ship;
int destroed_deck_B = 0;
int destroed_deck_P = 0;
bool END_GAME = false;
int matr_for_thinked[4][2];

bool setup_undone = true;

struct P_list *p_head_list = NULL;
struct P_list *p_root_list = NULL;
struct B_list *b_root_list = NULL;
struct B_list *b_head_list = NULL;



void B_Draw()// работает все нормально, чекнуть сдвиги
{
	system("cls");

	printf("    ");
	for (int i = 0; i < 10; i++)
	{
		printf("%d  ", i);
	}
	printf("\n");

	for (int j = 0; j < height; j++)
	{
		printf(" %d ", j);
		for (int i = 0; i < width; i++)
		{
			printf("(%c)", B_field[j][i]);

			if (i == (width - 1))
				printf("\n");
		}
	}
}

void P_Draw()// работает все нормально, чекнуть сдвиги
{
	system("cls");

	printf("    ");
	for (int i = 0; i < 10; i++)
	{
		printf("%d  ", i);
	}
	printf("\n");

	for (int j = 0; j < height; j++)
	{
		printf(" %d ", j);
		for (int i = 0; i < width; i++)
		{
			printf("(%c)", P_field[j][i]);

			if (i == (width - 1))
				printf("\n");
		}
	}
}

void T_Draw()
{
	printf("\n\n\n    ");
	for (int i = 0; i < 10; i++)
	{
		printf("%d  ", i);
	}
	printf("\n");

	for (int j = 0; j < height; j++)
	{
		printf(" %d ", j);
		for (int i = 0; i < width; i++)
		{
			printf("(%c)", P_T_field[j][i]);

			if (i == (width - 1))
				printf("\n");
		}
	}
}


void DRAW(void)
{
	P_Draw();
	printf("\n\n\n");
	if (setup_undone) {   //инструкци€
		printf("1 deck:  #                                 * - beginning coord\n");
		//2
		printf("2 deck:\n");
		printf("1 type: *   2 type: *#\n");
		printf("        #\n");
		//3
		printf("3 deck:\n");
		printf("1 type: *   2 type: *##   3 type: *#   4 type: #\n");
		printf("        #                          #           *#\n");
		printf("        #\n");
		//4
		printf("4 deck\n");
		printf("1 type: *   2 type: *###   3 type: *#   4 type: *   5 type: *#   6 type: *   7 type:  ##\n");
		printf("        #                           #           #           ##           ###         *#\n");
		printf("        #                           #           ##\n");
		printf("        #\n");
	}
	else {
		T_Draw();
	}
}

struct B_list {
	int ship_deck;
	int ship_num;
	int ship_type;
	int b_ship[4][2];
	struct B_list *b_next;
} list_b;

struct P_list {
		int ship_deck;
		int ship_num;
		int ship_type;
		int p_ship[4][2];
		struct P_list *p_next;
	} list_p;

void addelem_B_list(int deck, int type, int num)
{
	struct B_list *b_temporary;
	b_temporary = (struct B_list*)malloc(sizeof(B_list));

	if (b_head_list != NULL)
	{
		b_head_list->b_next = b_temporary;
	}

	if (b_root_list == NULL)
	{
		b_root_list = b_temporary;
	}

	for (int n = 0; n < deck; n++) {
		b_temporary->b_ship[n][0] = B_matrix[n][0];
		b_temporary->b_ship[n][1] = B_matrix[n][1];
	}
	b_temporary->ship_deck = deck;
	b_temporary->ship_type = type;
	b_temporary->ship_num = num;

	b_temporary->b_next = NULL;
	b_head_list = b_temporary;
}

void addelem_P_list(int deck, int type, int num)
{
	struct P_list *temporary;
	temporary = (struct P_list*)malloc(sizeof(P_list));

	if (p_head_list != NULL)
	{
		p_head_list->p_next = temporary;
	}

	if (p_root_list == NULL)
	{
		p_root_list = temporary;
	}

	for (int n = 0; n < deck; n++) {
		temporary->p_ship[n][0] = P_matryx[n][0];
		temporary->p_ship[n][1] = P_matryx[n][1];
	}
	temporary->ship_deck = deck;
	temporary->ship_type = type;
	temporary->ship_num = num;

	temporary->p_next = NULL;
	p_head_list = temporary;
}

void B_listprint(B_list *list_p)//линейный список кораблей в структ
{
	struct B_list *b_temporary;
	b_temporary = b_root_list;
	if (b_temporary == NULL)
		printf("Empty list!\n");
	do {
		for (int n = 0; n < (b_temporary->ship_deck); n++)
		{
			printf("BOT!  deck:%d | number:%d} %d %d", b_temporary->ship_deck, b_temporary->ship_num, b_temporary->b_ship[n][0], b_temporary->b_ship[n][1]);// вывод значени€ элемента
			printf("\n");
		}
		printf("\n");
		b_temporary = b_temporary->b_next; // переход к следующему узлу
	} while (b_temporary != NULL);
}

void P_listprint(P_list *list_p)//линейный список кораблей в структ
{
	struct P_list *p_temporary;
	p_temporary = p_root_list;
	do {
		for (int n = 0; n < (p_temporary->ship_deck); n++)
		{
			printf("PLAYER!  deck:%d | number:%d} %d %d", p_temporary->ship_deck, p_temporary->ship_num, p_temporary->p_ship[n][0], p_temporary->p_ship[n][1]);// вывод значени€ элемента
			printf("\n");
		}
		printf("\n");
		p_temporary = p_temporary->p_next; // переход к следующему узлу
	} while (p_temporary != NULL);
}

void create_array_of_targets() {// test
	int count = 0;
	P_list *temp1 = p_root_list;
	B_list *temp2 = b_root_list;
	while (count < 20) {
		for (int i = 0; i < temp1->ship_deck; i++) {
			ship_coord_P[count][0] = temp1->p_ship[i][0];
			ship_coord_P[count][1] = temp1->p_ship[i][1];
			ship_coord_B[count][0] = temp2->b_ship[i][0];
			ship_coord_B[count][1] = temp2->b_ship[i][1];
			count++;
		}
		temp1 = temp1->p_next;
		temp2 = temp2->b_next;
	}
}

int get_num_of_ship(int x, int y, bool HOD) {
	for (int i = 0; i < 20; i++) {
		if (HOD) {
			if (ship_coord_B[i][0] == x && ship_coord_B[i][1] == y) {
				if (i < 4) return i;
				if (i < 6) return 4;
				if (i < 8) return 5;
				if (i < 10) return 6;
				if (i < 13) return 7;
				if (i < 16) return 8;
				if (i < 20) return 9;
			}
		}
		if (ship_coord_P[i][0] == x && ship_coord_P[i][1] == y) {
			if (i < 4) return i;
			if (i < 6) return 4;
			if (i < 8) return 5;
			if (i < 10) return 6;
			if (i < 13) return 7;
			if (i < 16) return 8;
			if (i < 20) return 9;
		}
	}
	return -1;
}

void thinked_ship(int deck, int ship_type, bool HOD) { //clone of add_ban_locality // HOD 1 = player, HOD 0 - Bot
	int coord1, coord2, length;
	int ban_tmp[14][2];
	if (deck == 1) length = 8;
	else if (deck == 2) length = 10;
	else if (deck == 3) length = 12;
	else if ((deck == 4) && (ship_type == 5)) length = 12;
	else length = 14;
	int count_tmp = 0;//кол-во заполненых €чеек масива ban_tmp
	bool flag437 = false;

	for (int i = 0; i < deck; i++) {
		coord1 = matr_for_thinked[i][0];// x
		coord2 = matr_for_thinked[i][1];// y
		for (int l = -1; l < 2; l++) {
			for (int m = -1; m < 2; m++) {
				flag437 = false;
				if (l != 0 || m != 0) {// исключение самой точки

					for (int k = 0; k < count_tmp; k++) {//проверка были ли такие точки раньше
						if (ban_tmp[k][0] == coord1 + l && ban_tmp[k][1] == coord2 + m) {
							flag437 = true;
							break;
						}
					}

					for (int p = 0; p < deck; p++) {//провер€ем наложение на тело корабл€
						if (matr_for_thinked[p][0] == coord1 + l && matr_for_thinked[p][1] == coord2 + m) {
							flag437 = true;
							
							break;
						}
					}
					if ((!flag437) && (coord1 + l >= 0 && coord1 + l < 10) && (coord2 + m >= 0 && coord2 + m < 10)) {//выход за границы пол€
						ban_tmp[count_tmp][0] = coord1 + l;
						ban_tmp[count_tmp][1] = coord2 + m;
						count_tmp++;
					}
				}
			}
		}

	}
	for (int i = 0; i < count_tmp; i++) {//в бан тмп присутствуют невалидные значени€ -overдоху€
		if (HOD) P_T_field[ban_tmp[i][1]][ban_tmp[i][0]] = '*';
		else P_field[ban_tmp[i][1]][ban_tmp[i][0]] = '*';
	}
}

int check_type_of_ship(int num) {
	if (num < 4) return (1);
	if (num < 10) return(2);
	if (num < 16) return(3);
	if (num < 20) return(4);
}

bool is_strike_correct_B(int x, int y) {
	if ((P_field[y][x] != 'X' && P_field[y][x] != '*') && (y > -1 && y < 10) && (x > -1 && x < 10)) return(true);
	//стрел€ем не по трупу, не по мимо, не за границы пол€
	return(false);
}

bool is_strike_correct_P(int x, int y) {
	if ((P_T_field[y][x] != 'X' && P_T_field[y][x] != '*') && (y > -1 && y < 10) && (x > -1 && x < 10)) return(true);
	//стрел€ем не по трупу, не по мимо, не за границы пол€
	return(false);
}



bool finishing() {//выбирает вариацию смещени€ выстрела относительно 1го попадани€
	int coord_strike1 = history_of_strike[count_of_history-1][0]; /// -1??
	int coord_strike2 = history_of_strike[count_of_history-1][1];

	if (destroed_deck_B == deck_of_ship) {
		
		
		next_strike = 0;
		finish_ship = false;

		int num = get_num_of_ship(coord_strike1, coord_strike2, false);
		P_list *temp;
		temp = p_root_list;
		while (num > 0) {
			temp = temp->p_next;
			num--;
		}
		for (int l = 0; l < temp->ship_deck; l++) {
			matr_for_thinked[l][0] = temp->p_ship[l][0];
			matr_for_thinked[l][1] = temp->p_ship[l][1];
		}
		destroed_deck_B = -1;
		deck_of_ship = -1;
		thinked_ship(temp->ship_deck, temp->ship_type, 0);
		DRAW();
		printf("\n Bot silen your ship, Comander!");
		pause();
		//обвести корабль *
		return true;
	}
	else {
		if (next_strike == 4) {

			sdv++;///!!!!!!!!!!!!1

			next_strike = 0;
		}
		while (next_strike < 4) {// 6, 3, 12, 9  часов соответственно

			switch (next_strike) {
			case 0: {
				coord_strike1 = history_of_strike[count_of_history - 1 - sdv][0];
				coord_strike2 = history_of_strike[count_of_history - 1 - sdv][1] + 1;
				break;
			}
			case 1: {
				coord_strike1 = history_of_strike[count_of_history - 1 - sdv][0] + 1;
				coord_strike2 = history_of_strike[count_of_history - 1 - sdv][1];
				break;
			}
			case 2: {
				coord_strike1 = history_of_strike[count_of_history - 1 - sdv][0];
				coord_strike2 = history_of_strike[count_of_history - 1 - sdv][1] - 1;
				break;
			}
			case 3: {
				coord_strike1 = history_of_strike[count_of_history - 1 - sdv][0] - 1;
				coord_strike2 = history_of_strike[count_of_history - 1 - sdv][1];
				break;
			}
			}
			
			if (is_strike_correct_B(coord_strike1, coord_strike2)) {

				for (int i = 0; i < 20; i++) {
					if (ship_coord_P[i][0] == coord_strike1) {
						if (ship_coord_P[i][1] == coord_strike2) {
							Life_P--;
							P_field[coord_strike2][coord_strike1] = 'X';
							Player_Field[coord_strike2][coord_strike1] = 'X';
							destroed_deck_B++;
							history_of_strike[count_of_history][0] = coord_strike1;//
							history_of_strike[count_of_history][1] = coord_strike2;//
							count_of_history++;
							sdv = 0;
							next_strike = 0;
							finish_ship = true;
							DRAW();
							return true;
							////bot attack
						}
					}
				}
				//missed strike
				P_field[coord_strike2][coord_strike1] = '*';
				Player_Field[coord_strike1][coord_strike2] = '*';
				next_strike++;
				DRAW();
				return(false);
			}
			next_strike++;//??????????????++?
		}
		DRAW();
		return true;
	}
}


bool first_shot() {
	count_of_history = 0;
	/*int destroed_deck = 0;
	int type_of_ship = -1;*/
	for (int i = 0; i < 4; i++) { //обнул€ем историю дл€ нового корабл€
		for (int j = 0; j < 2; j++) {
			history_of_strike[i][j] = -1;
		}
	}
	int X_coord = -2;
	int Y_coord = -2;
	/*int coord1 = 4;
	int coord2 = 4;
*/

	while (!is_strike_correct_B(X_coord, Y_coord)) {
		X_coord = rand() % 10;
		Y_coord = rand() % 10;
		//generate random coordinate
	}

	for (int i = 0; i < 20; i++) {
		if (ship_coord_P[i][0] == X_coord) {
			if (ship_coord_P[i][1] == Y_coord) {

				if (P_field[Y_coord][X_coord] != 'X' && P_field[Y_coord][X_coord] != '*') {// ненужна проверка?
					Life_P--;
					P_field[Y_coord][X_coord] = 'X';
					Player_Field[Y_coord][X_coord] = 'X';// ???????????? мусор
					destroed_deck_B=1;
					deck_of_ship = check_type_of_ship(i);
					history_of_strike[count_of_history][0] = X_coord;
					history_of_strike[count_of_history][1] = Y_coord;
					count_of_history++;
					finish_ship = true;
					sdv = 0;
					next_strike = 0;
					int num = get_num_of_ship(X_coord, Y_coord, false);
					P_list *temp;
					temp = p_root_list;
					while (num > 0) {
						temp = temp->p_next;
						num--;
					}
					if (destroed_deck_B == temp->ship_deck) {
						for (int l = 0; l < temp->ship_deck; l++) {
							matr_for_thinked[l][0] = temp->p_ship[l][0];
							matr_for_thinked[l][1] = temp->p_ship[l][1];
						}

						thinked_ship(temp->ship_deck, temp->ship_type, 0);
					}
				}
				DRAW();
				//intf("\n Bot silen your ship, Comander!");
				//pause();
				return(true);
			}
		}
	}


	P_field[Y_coord][X_coord] = '*';
	DRAW();
	return(false);
}



bool Bot_Atack() {
	while (finish_ship) {
		if (finishing()) {
			//DRAW();
			if (Life_P == 0) {//Check lose
				printf("\nYou lose again( Press any key to go out.");
				END_GAME = true;
			}
			return true;
		}
		else return false;
	}

	if (first_shot()) {
		if (deck_of_ship == 1) {
			//DRAW();
			if (Life_P == 0) {//Check lose
				printf("\nYou lose again( Press any key to go out.");
				pause();
				exit(0);
			}
		}
		DRAW();
		return true;
	}
	else {
		DRAW();
		return false;
	}

}



bool B_Check(int deck)// true
{

	for (int b = 0; b < deck; b++) {
		if ((B_matrix[b][0] > 9) || (B_matrix[b][0] < 0) || (B_matrix[b][1] < 0) || (B_matrix[b][1] > 9))
		{
			//printf("err1");
			//pause();
			return true;
		}
	}
	for (int a = 0; a < B_last_point; a++) {
		for (int b = 0; b < deck; b++) {
			if (B_ban[a][0] == B_matrix[b][0] && B_ban[a][1] == B_matrix[b][1]) {
				//printf("err2");
				//pause();
				return true;
			}

		}
	}
	return false;
}

bool P_Check(int deck)// true
{

	for (int b = 0; b < deck; b++) {
		if ((P_matryx[b][0] > 9) || (P_matryx[b][0] < 0) || (P_matryx[b][1] < 0) || (P_matryx[b][1] > 9))
		{
			//printf("err1");
			//pause();
			return true;
		}
	}
	for (int a = 0; a < P_last_point; a++) {
		for (int b = 0; b < deck; b++) {
			if (P_ban[a][0] == P_matryx[b][0] && P_ban[a][1] == P_matryx[b][1]) {
				//printf("err2");
				//pause();
				return true;
			}
			
		}
	}
	return false;
}

void B_Setup()
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (i == 0)
			{
				B_field[j][i] = (char)i;
			}
			else
			{
				B_field[j][i] = ' ';
			}
		}
	}
}

void P_Setup()
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (i == 0)
			{
				P_field[j][i] = (char)i;
			}
			else
			{
				P_field[j][i] = ' ';
			}
		}
	}
}

void T_Setup()
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (i == 0)
			{
				P_T_field[j][i] = (char)i;
			}
			else
			{
				P_T_field[j][i] = ' ';
			}
		}
	}
}


bool Player_Atack() {
	//return false;// for debug
	int X_coord = -1;
	int Y_coord = -1;
	while (!is_strike_correct_P(X_coord, Y_coord)) {
		printf("Write coordinates of target");
		scanf(" %d %d", &X_coord, &Y_coord);
	}

	for (int i = 0; i < 20; i++) {
		if (ship_coord_B[i][0] == X_coord) {
			if (ship_coord_B[i][1] == Y_coord) {


				if (P_T_field[Y_coord][X_coord] != 'X') {
					Life_B--;
					P_T_field[Y_coord][X_coord] = 'X';
					//обрисовка убитого *
					int num = get_num_of_ship(X_coord, Y_coord, true);
					B_list *temp;
					temp = b_root_list;
					while (num > 0) {
						temp = temp->b_next;
						num--;
					}

					//

					int count = 0;
					for (int k = 0; k < temp->ship_deck; k++) {
						int x, y;
						x = temp->b_ship[k][0];
						y = temp->b_ship[k][1];
						if (P_T_field[y][x] == 'X') count++;
					}
					if (count == temp->ship_deck) {
						printf("\nTarget down!");

						for (int l = 0; l < temp->ship_deck; l++) {
							matr_for_thinked[l][0] = temp->b_ship[l][0];
							matr_for_thinked[l][1] = temp->b_ship[l][1];
						}

						thinked_ship(temp->ship_deck, temp->ship_type, 1);
						if (Life_B == 0) {
							printf("\nYou win!");
							END_GAME = true;
							return false;
						}
						DRAW();
						return true;
					}
					else {
						DRAW();
						return true;
					}
				}
			}
			//else printf("\nStop kicking a dead dog, please!!!");
			//Draw();
			

		}
	}
	P_T_field[Y_coord][X_coord] = '*';
	printf("\nYou missed");
	DRAW();
	return false;
}

void add_B_ban_locality(void)
{
	B_last_point = 0;
	for (int i = 0; i < 1000; i++) {
		B_ban[i][0] = -2;
		B_ban[i][1] = -2;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (B_field[j][i] == '#')
			{
				for (int l = -1; l < 2; l++) {
					for (int m = -1; m < 2; m++)
					{
						B_ban[B_last_point][0] = i + l;
						B_ban[B_last_point][1] = j + m;
						B_last_point++;
					}
				}
			}
		}
	}
}

void add_P_ban_locality(void) //ship - mass of point of ship
{ 
	P_last_point = 0;
	for (int i = 0; i < 1000; i++) {
		P_ban[i][0] = -2;
		P_ban[i][1] = -2;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (P_field[j][i] == '#')
			{
				for (int l = -1; l < 2; l++) {
					for (int m = -1; m < 2; m++)
					{
						P_ban[P_last_point][0] = i + l;
						P_ban[P_last_point][1] = j + m;
						P_last_point++;
					}
				}
			}
		}
	}
}

void B_ship_coord(int x, int y, int type, int deck)
{
	for (int i = 0; i < 4; i++) {
		B_matrix[i][0] = -1;
		B_matrix[i][1] = -1;
	}
	//1
	if (deck == 1) {
		B_matrix[0][0] = x;
		B_matrix[0][1] = y;
	}
	//2
	if (deck == 2) {
		if (type == 1) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
		}
		if (type == 2) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
		}
	}
	//3
	if (deck == 3) {
		if (type == 1) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_matrix[2][0] = x;
			B_matrix[2][1] = y + 2;
		}
		if (type == 2) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_matrix[2][0] = x + 2;
			B_matrix[2][1] = y;

		}
		if (type == 3) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y;
		}
		if (type == 4) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y - 1;
		}
	}
	//4
	if (deck == 4) {
		if (type == 1) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_matrix[2][0] = x;
			B_matrix[2][1] = y + 2;
			B_matrix[3][0] = x;
			B_matrix[3][1] = y + 3;
		}
		if (type == 2) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_matrix[2][0] = x + 2;
			B_matrix[2][1] = y;
			B_matrix[3][0] = x + 3;
			B_matrix[3][1] = y;
		}
		if (type == 3) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y + 1;
			B_matrix[3][0] = x + 1;
			B_matrix[3][1] = y + 2;
		}
		if (type == 4) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_matrix[2][0] = x;
			B_matrix[2][1] = y + 2;
			B_matrix[3][0] = x + 1;
			B_matrix[3][1] = y + 2;
		}
		if (type == 5) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y;
			B_matrix[3][0] = x + 1;
			B_matrix[3][1] = y + 1;
		}
		if (type == 6) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y + 1;
			B_matrix[3][0] = x + 2;
			B_matrix[3][1] = y + 1;
		}
		if (type == 7) {
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y - 1;
			B_matrix[3][0] = x + 2;
			B_matrix[3][1] = y - 1;
		}
	}
}

void P_ship_coord(int x, int y, int type, int deck)
{
	for (int i = 0; i < 4; i++) {
		P_matryx[i][0] = -1;
		P_matryx[i][1] = -1;
	}
	//1
	if (deck == 1) {
		P_matryx[0][0] = x;
		P_matryx[0][1] = y;
	}
	//2
	if (deck == 2) {
		if (type == 1) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
		}
		if (type == 2) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
		}
	}
	//3
	if (deck == 3) {
		if (type == 1) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_matryx[2][0] = x;
			P_matryx[2][1] = y + 2;
		}
		if (type == 2) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_matryx[2][0] = x + 2;
			P_matryx[2][1] = y;

		}
		if (type == 3) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y;
		}
		if (type == 4) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y - 1;
		}
	}
	//4
	if (deck == 4) {
		if (type == 1) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_matryx[2][0] = x;
			P_matryx[2][1] = y + 2;
			P_matryx[3][0] = x;
			P_matryx[3][1] = y + 3;
		}
		if (type == 2) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_matryx[2][0] = x + 2;
			P_matryx[2][1] = y;
			P_matryx[3][0] = x + 3;
			P_matryx[3][1] = y;
		}
		if (type == 3) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y + 1;
			P_matryx[3][0] = x + 1;
			P_matryx[3][1] = y + 2;
		}
		if (type == 4) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_matryx[2][0] = x;
			P_matryx[2][1] = y + 2;
			P_matryx[3][0] = x + 1;
			P_matryx[3][1] = y + 2;
		}
		if (type == 5) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y;
			P_matryx[3][0] = x + 1;
			P_matryx[3][1] = y + 1;
		}
		if (type == 6) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y + 1;
			P_matryx[3][0] = x + 2;
			P_matryx[3][1] = y + 1;
		}
		if (type == 7) {
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y - 1;
			P_matryx[3][0] = x + 2;
			P_matryx[3][1] = y - 1;
		}
	}
}

void P_Placement(int x, int y, int type, int deck, int num)
{
	//1
	if (deck == 1) {
		P_field[y][x] = '#';
		P_matryx[0][0] = x;
		P_matryx[0][1] = y;
		addelem_P_list(deck, type, num);
	}
	//2
	if (deck == 2) {
		if (type == 1) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
		}
		else {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y][x + 1] = '#';
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
		}
		addelem_P_list(deck, type, num);
	}
	//3
	if (deck == 3) {
		if (type == 1) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_field[y + 2][x] = '#';
			P_matryx[2][0] = x;
			P_matryx[2][1] = y + 2;
		}
		if (type == 2) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y][x + 1] = '#';
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_field[y][x + 2] = '#';
			P_matryx[2][0] = x + 2;
			P_matryx[2][1] = y;

		}
		if (type == 3) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_field[y][x + 1] = '#';
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y;
		}
		if (type == 4) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y][x + 1] = '#';
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_field[y - 1][x + 1] = '#';
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y - 1;
		}
		addelem_P_list(deck, type, num);
	}
	//4
	if (deck == 4) {
		if (type == 1) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_field[y + 2][x] = '#';
			P_matryx[2][0] = x;
			P_matryx[2][1] = y + 2;
			P_field[y + 3][x] = '#';
			P_matryx[3][0] = x;
			P_matryx[3][1] = y + 3;
		}
		if (type == 2) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y][x + 1] = '#';
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_field[y][x + 2] = '#';
			P_matryx[2][0] = x + 2;
			P_matryx[2][1] = y;
			P_field[y][x + 3] = '#';
			P_matryx[3][0] = x + 3;
			P_matryx[3][1] = y;
		}
		if (type == 3) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y][x + 1] = '#';
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_field[y + 1][x + 1] = '#';
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y + 1;
			P_field[y + 2][x + 1] = '#';
			P_matryx[3][0] = x + 1;
			P_matryx[3][1] = y + 2;
		}
		if (type == 4) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_field[y + 2][x] = '#';
			P_matryx[2][0] = x;
			P_matryx[2][1] = y + 2;
			P_field[y + 2][x + 1] = '#';
			P_matryx[3][0] = x + 1;
			P_matryx[3][1] = y + 2;
		}
		if (type == 5) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_field[y][x + 1] = '#';
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y;
			P_field[y + 1][x + 1] = '#';
			P_matryx[3][0] = x + 1;
			P_matryx[3][1] = y + 1;
		}
		if (type == 6) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y + 1][x] = '#';
			P_matryx[1][0] = x;
			P_matryx[1][1] = y + 1;
			P_field[y + 1][x + 1] = '#';
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y + 1;
			P_field[y + 1][x + 2] = '#';
			P_matryx[3][0] = x + 2;
			P_matryx[3][1] = y + 1;
		}
		if (type == 7) {
			P_field[y][x] = '#';
			P_matryx[0][0] = x;
			P_matryx[0][1] = y;
			P_field[y][x + 1] = '#';
			P_matryx[1][0] = x + 1;
			P_matryx[1][1] = y;
			P_field[y - 1][x + 1] = '#';
			P_matryx[2][0] = x + 1;
			P_matryx[2][1] = y - 1;
			P_field[y - 1][x + 2] = '#';
			P_matryx[3][0] = x + 2;
			P_matryx[3][1] = y - 1;
		}
		addelem_P_list(deck, type, num);
	}
}

void B_Placement(int x, int y, int type, int deck, int num)
{
	//1
	if (deck == 1) {
		B_field[y][x] = '#';
		B_matrix[0][0] = x;
		B_matrix[0][1] = y;
		addelem_B_list(deck, type, num);
	}
	//2
	if (deck == 2) {
		if (type == 1) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
		}
		else {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y][x + 1] = '#';
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
		}
		addelem_B_list(deck, type, num);
	}
	//3
	if (deck == 3) {
		if (type == 1) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_field[y + 2][x] = '#';
			B_matrix[2][0] = x;
			B_matrix[2][1] = y + 2;
		}
		if (type == 2) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y][x + 1] = '#';
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_field[y][x + 2] = '#';
			B_matrix[2][0] = x + 2;
			B_matrix[2][1] = y;

		}
		if (type == 3) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_field[y][x + 1] = '#';
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y;
		}
		if (type == 4) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y][x + 1] = '#';
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_field[y - 1][x + 1] = '#';
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y - 1;
		}
		addelem_B_list(deck, type, num);
	}
	//4
	if (deck == 4) {
		if (type == 1) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_field[y + 2][x] = '#';
			B_matrix[2][0] = x;
			B_matrix[2][1] = y + 2;
			B_field[y + 3][x] = '#';
			B_matrix[3][0] = x;
			B_matrix[3][1] = y + 3;
		}
		if (type == 2) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y][x + 1] = '#';
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_field[y][x + 2] = '#';
			B_matrix[2][0] = x + 2;
			B_matrix[2][1] = y;
			B_field[y][x + 3] = '#';
			B_matrix[3][0] = x + 3;
			B_matrix[3][1] = y;
		}
		if (type == 3) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y][x + 1] = '#';
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_field[y + 1][x + 1] = '#';
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y + 1;
			B_field[y + 2][x + 1] = '#';
			B_matrix[3][0] = x + 1;
			B_matrix[3][1] = y + 2;
		}
		if (type == 4) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_field[y + 2][x] = '#';
			B_matrix[2][0] = x;
			B_matrix[2][1] = y + 2;
			B_field[y + 2][x + 1] = '#';
			B_matrix[3][0] = x + 1;
			B_matrix[3][1] = y + 2;
		}
		if (type == 5) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_field[y][x + 1] = '#';
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y;
			B_field[y + 1][x + 1] = '#';
			B_matrix[3][0] = x + 1;
			B_matrix[3][1] = y + 1;
		}
		if (type == 6) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y + 1][x] = '#';
			B_matrix[1][0] = x;
			B_matrix[1][1] = y + 1;
			B_field[y + 1][x + 1] = '#';
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y + 1;
			B_field[y + 1][x + 2] = '#';
			B_matrix[3][0] = x + 2;
			B_matrix[3][1] = y + 1;
		}
		if (type == 7) {
			B_field[y][x] = '#';
			B_matrix[0][0] = x;
			B_matrix[0][1] = y;
			B_field[y][x + 1] = '#';
			B_matrix[1][0] = x + 1;
			B_matrix[1][1] = y;
			B_field[y - 1][x + 1] = '#';
			B_matrix[2][0] = x + 1;
			B_matrix[2][1] = y - 1;
			B_field[y - 1][x + 2] = '#';
			B_matrix[3][0] = x + 2;
			B_matrix[3][1] = y - 1;
		}
		addelem_B_list(deck, type, num);
	}
}

void B_Enter()
{
	int x, y,type;
	bool flag = false;
	for (int deck = 1; deck <= 4; deck++)
	{
		if (deck == 1) {
			for (int number = 0; number < 4; number++)
			{
			loop1:
				type = 1;
				x = rand() % 10;
				y = rand() % 10;
				B_ship_coord(x, y, type, deck);
				flag = B_Check(deck);
				if (flag) {
					goto loop1;
				}
				B_Placement(x, y, type, deck, number);
				add_B_ban_locality();

				//add_B_ship_ban(deck, type); //int** ship, int deck, int ship_type _________________________first call!
				
				
				B_Draw();
				pause();
			}
		}
		if (deck == 2) {
			for (int number = 0; number < 3; number++)
			{
			loop2:
				type = 1 + rand() % 2;
				x = rand() % 10;
				y = rand() % 10;
				B_ship_coord(x, y, type, deck);
				flag = B_Check(deck);
				if (flag) {
					goto loop2;
				}
				B_Placement(x, y, type, deck, number);
				add_B_ban_locality();

				
				B_Draw();
				pause();
			}
		}
		if (deck == 3) {
			for (int number = 0; number < 2; number++)
			{
			loop3:
				type = 1 + rand() % 4;
				x = rand() % 10;
				y = rand() % 10;
				B_ship_coord(x, y, type, deck);
				flag = B_Check(deck);
				if (flag) {
					goto loop3;
				}
				B_Placement(x, y, type, deck, number);
				add_B_ban_locality();


				B_Draw();
				pause();
			}
		}
		if (deck == 4) {
			for (int number = 0; number < 1; number++)
			{
			loop4:
				type = 1 + rand() % 7;
				x = rand() % 10;
				y = rand() % 10;
				B_ship_coord(x, y, type, deck);
				flag = B_Check(deck);
				if (flag) {
					goto loop4;
				}
				B_Placement(x, y, type, deck, number);
				add_B_ban_locality();

				
				B_Draw();
				pause();
			}
		}
	}
}

void P_Enter()
{
	int x, y, type;
	bool flag = false;
	for (int deck = 1; deck <= 4; deck++)
	{
		if (deck == 1) {
			for (int number = 0; number < 4; number++)
			{
			loop1:
				type = 1;
				printf("\nEnter the coordinates of the %d-deck ship є%d: ", deck, number);
				scanf("%d %d", &x, &y);
				P_ship_coord(x, y, type, deck);
				flag = P_Check(deck);
				if (flag) {
					printf("Invalid input!\n");
					pause();
					goto loop1;
				}
						 //int** ship, int deck, int ship_type _________________________first call!
				P_Placement(x, y, type, deck, number);
				add_P_ban_locality();
				DRAW();
			}
		}
		if (deck == 2) {
			for (int number = 0; number < 3; number++)
			{
			loop2:
				printf("\nEnter ship type:");
				scanf("%d", &type);
				printf("Enter the starting coordinates of the %d-deck ship є%d: ", deck, number);
				scanf("%d %d", &x, &y);
				P_ship_coord(x, y, type, deck);
				flag = P_Check(deck);
				if (flag) {
					printf("Invalid input!\n");
					pause();
					goto loop2;
				}
				P_Placement(x, y, type, deck, number);
				add_P_ban_locality();
				DRAW();
			}
		}
		if (deck == 3) {
			for (int number = 0; number < 2; number++)
			{
			loop3:
				printf("\nEnter ship type:");
				scanf("%d", &type);
				printf("Enter the starting coordinates of the %d-deck ship є%d: ", deck, number);
				scanf("%d %d", &x, &y);
				P_ship_coord(x, y, type, deck);
				flag = P_Check(deck);
				if (flag) {
					printf("Invalid input!\n");
					pause();
					goto loop3;
				}
				P_Placement(x, y, type, deck, number);
				add_P_ban_locality();
				DRAW();
			}
		}
		if (deck == 4) {
			for (int number = 0; number < 1; number++)
			{
			loop4:
				printf("\nEnter ship type:");
				scanf("%d", &type);
				printf("Enter the starting coordinates of the %d-deck ship є%d: ", deck, number);
				scanf("%d %d", &x, &y);
				P_ship_coord(x, y, type, deck);
				flag = P_Check(deck);
				if (flag) {
					printf("Invalid input!\n");
					pause();
					goto loop4;
				}
				P_Placement(x, y, type, deck, number);
				add_P_ban_locality();
				DRAW();
			}
		}
	}
}



void game() {
	DRAW();
	while (!END_GAME) {
		DRAW();
		while (Player_Atack()) {
			//DRAW();
		};
		if (END_GAME) break;
		while (Bot_Atack()) {
			pause();
			//DRAW();
		};
	}
	return;
}

int main(void)
{
	srand(time(NULL));
	P_Setup();
	B_Setup();
	DRAW();
	P_Enter();
	setup_undone = false;
	B_Enter();
	create_array_of_targets();
	game();
	//B_Draw();
	//listprint(&list_p);
	
	/*game();*/
	pause();
	
	return 0;
}