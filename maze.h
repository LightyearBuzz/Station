#pragma once
/*������������ ���� maze, � ��� ������� ������ ��������, ����������, ������ � ��������
*��������� ��� ���� Station
*
*                     �������� ���������
*1.���� ������� ������� �� ��������
*  1.1 �������� ������� ��� ����������
*  1.2 ��� � ���� ����� ������� �������
*2.������� ������������ �������
*3.���� ������������ ������ ����
*  3.1 �������� �������� �����������(������� �����) � ������� ����� ���������.
*  3.2.��������� ����� � ����� �������
*  3.3.��������� ����� �� ������ ������� � ���������� �������� GenRooms() � ����� ��������
*4.���� ������������ ������� ��� � ���� �� ������
*  4.1.������������ �� ����� ����� �� ���� ������� 
*  4.2.���������� �������� GenRooms() � ���������� ��������(������� ������ �� ������� �����)
*        �� ��������, ��� � ��� ������� ��� �� ��� �� ��������� ������
*/


#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack> // ��� �������� � ����� ������� 

#define BODY "BODY" 
#define COMPUTER "COMPUTER"

using namespace std;

//����� Maze ��������� �������� ���������
class Maze {
  private:
    int visited_rooms_counter = 0;//������ ���������� ���������� ������
    int quantity_rooms = 0;//������ ����� ���������� ������ � ���������
    unsigned short quantity_rows;//������ ���������� ����� � ���������
    unsigned short quantity_columns;//������ ���������� ������ � ���������
    char body_string[10] = BODY;//������ ���� ��������������
    char empty_string_body[1] = "";//���� ���� ��� � �������
    char computer_string[10] = COMPUTER;//������ ���������
    char empty_string_computer[1] = "";//���� ���������� ��� � �������
	
    //��������� �������� ��������� �������
    struct Room {
      int row_number;//����������  �������, ����� ���� � ���������
      int column_number;//���������� �������, ����� ������� � ���������
      int room_number;//���������� ����� �������
      int doors[4] = {0, 0, 0, 0};// N E S W ��������� �����������
      int visited = 0;//���� ���������
      int computer = 0;//���� ������� ���������� � ������� 
      int body = 0;//���� ������� ���� ��������������
    };

    Room** ship;//��������� ������������ ������ ���� Room ��� �������� ��������� 
    stack <struct Room*> s;//���� � �������� ������
    int max_stack = 0;//������ ������������ �������� �����
	 
  public:
    //������� ������������ ��������� ������ ��� �������� ���������
    void GenField(int r, int c) {
      quantity_rows = r;
      quantity_columns = c;
      quantity_rooms = quantity_rows * quantity_columns;

      ship = new Room*[quantity_rows];//������ ����� �� ������ ����������

      //���� �������� � ������ ������� ������� ship ������ �� ������ ���� Room 
      for (int i = 0; i < quantity_rows; i++) {
        ship[i] = new Room[quantity_columns];
      }
    }

    //�������������� ������ �������
    void PrepRooms() {
      int number = 0;
      
      for (int i = 0; i < quantity_rows; i++) {
        for (int j = 0; j < quantity_columns; j++) {
          ship[i][j].room_number = number;//������������� ���������� ����� �������
          number++;
          ship[i][j].row_number = i;//������������� ���������� �������, ����� ���� � ���������
          ship[i][j].column_number = j;//������������� ���������� �������, ����� ������� � ���������
        }
      }
    }

    //���������� ����� ������� (0,0)
    Room* GetZeroAdress() {
      return &ship[0][0];
    }

    //���������� ������������ �������� �����
    int GetMaxStack() {
      return max_stack;
    }

    //��������� �������� Recursive Backtrack 
    //����������� ������ ������� � ����� ����� ���� 
    void GenRooms(Room* r) {
      int number_random_neighbour = 0;//������ ����� ���������� ������ �� 1 �� 4
      bool passible_neighbours[4] = { 0,0,0,0 };//������ ��������� ������� ������� ������� (NESW)
      int quant_passible_neighbours = 0;//���������� ��������� ������� �� 0 �� 4

      //1.���� ������� ������� �� �������� 
      if (r->visited == 0) {
        r->visited = 1;//1.1 �������� ������� ��� ���������� 	
        visited_rooms_counter++;
        s.push(r);//1.2 ��� � ���� ����� ������� �������
        max_stack++;
      }

		  //2.������� ������������ �������
      //to North
      if (r->row_number > 0 && ship[r->row_number - 1][r->column_number].visited == 0) {
        passible_neighbours[0] = 1;
      }

      //to East
      if (r->column_number < quantity_columns - 1 && ship[r->row_number][r->column_number + 1].visited == 0) {
        passible_neighbours[1] = 1;
      }

      //to South
      if (r->row_number < quantity_rows - 1 && ship[r->row_number + 1][r->column_number].visited == 0) {
        passible_neighbours[2] = 1;
      }

      //to West
      if (r->column_number > 0 && ship[r->row_number][r->column_number - 1].visited == 0) {
        passible_neighbours[3] = 1;
      }

      for (int i = 0; i < 4; i++) {
        if (passible_neighbours[i] == 1) {
          quant_passible_neighbours++;
        }
      }

      //3.���� ������������ ������ ����
      if (quant_passible_neighbours > 0) {

        //3.1 �������� �������� �����������(������� �����) � ������� ����� ���������.       
        //��� ����������� ������� ����� ����� �� ������������ ������� �������� ������� ������
        number_random_neighbour = rand() % quant_passible_neighbours + 1; 
        
        //���������� ����� ������� ����� ������������� ������ ������
        //��� ����� � ����� ���������� ������� �����(destination++) � ����������� �� ����������
        //������ ������ (number_random_neighbour--)
        int destination;//������ �������� ������� ����� � ������� ����� ����, � ���� ����� �� 1 �� 4 (NESW)
        for (destination = 0; number_random_neighbour > 0; destination++){
          if (passible_neighbours[destination] == 1) {
            number_random_neighbour--;
          }
        }
 
        //3.2.��������� ����� � ������� ������� � ������� ����� �������
        r->doors[destination - 1] = 1;

        //3.3.��������� ����� � ����� ������� � ������� ������ ������� � ���������� �������� GenRooms() � ����� ��������
        switch (destination) {
          case 1://�� �����
            ship[r->row_number - 1][r->column_number].doors[2] = 1;
            GenRooms(&ship[r->row_number - 1][r->column_number]);
            break;
          case 2://�� ������
            ship[r->row_number][r->column_number + 1].doors[3] = 1;
            GenRooms(&ship[r->row_number][r->column_number + 1]);
            break;
          case 3://�� ��
            ship[r->row_number + 1][r->column_number].doors[0] = 1;
            GenRooms(&ship[r->row_number + 1][r->column_number]);
            break;
          case 4://�� �����
            ship[r->row_number][r->column_number - 1].doors[1] = 1;
            GenRooms(&ship[r->row_number][r->column_number - 1]);
            break;
        }

      //4.���� ������������ ������� ��� � ���� �� ������	
      } else if (s.size() > 1) { 	

        //4.1.������������ �� ����� ����� �� ���� �������
        s.pop();	

        //4.2.���������� �������� GenRooms() � ���������� ��������(������� ������ �� ������� �����)
        //�� ��������, ��� � ��� ������� ��� �� ��� �� ��������� ������
        GenRooms(s.top());
      }
    }

    //�������� ��������� � ���� � ��������� �������
    void GenItems() {
      int computer_address;
      int body_address;

      computer_address = rand() % quantity_rooms;
      body_address = rand() % quantity_rooms;

      for (int i = 0; i < quantity_rows; i++){
        for (int j = 0; j < quantity_columns; j++){
          if (ship[i][j].room_number == computer_address) {
            ship[i][j].computer = 1;//�������� ���������
          }

          if (ship[i][j].room_number == body_address) {
            ship[i][j].body = 1;//��������� ����
          }
        }
      }
    }

    //���������� ���������� ������ � �������� �������
    int GetQuantityDoors(int r, int c){
      int q = 0;
      for (int i = 0; i < 4; i++){
        if (ship[r][c].doors[i] == 1) {
          q++;
        }
      }
      return q;
    }

    //���������� N ���� � ������� ���� ����� �� �����
    char GetNorthDirection(int r, int c){
      if (ship[r][c].doors[0] == 1) {
        return 'N';
      } else {
        return ' ';
      }
    }

    //���������� S ���� � ������� ���� ����� �� ��
    char GetSouthDirection(int r, int c){
      if (ship[r][c].doors[2] == 1) {
        return 'S';
      } else {
        return ' ';
      }
    }

    //���������� E ���� � ������� ���� ����� �� ������
    char GetEastDirection(int r, int c){
      if (ship[r][c].doors[1] == 1) {
        return 'E';
      } else {
        return ' ';
      }
    }

    //���������� W ���� � ������� ���� ����� �� �����
    char GetWestDirection(int r, int c){
      if (ship[r][c].doors[3] == 1) {
        return 'W';
      } else {
        return ' ';
      }
    }

    //���������� BODY ���� � ������� ���� ����
    char* GetBody(int r, int c){
      if (ship[r][c].body == 1) {
        return body_string;
      } else {
        return empty_string_body;
      }
    }

    //���������� Computer ���� � ������� ���� ���������
    char* GetComp(int r, int c) {
      if (ship[r][c].computer == 1) {
        return computer_string;
      } else {
        return empty_string_computer;
      }
    }

    //������������� �������� "����" � 0
    void SetBodyZero(int r, int c) {
      ship[r][c].body = 0;
    }

    //����������� ������ �� ���������
    void DeleteField() { 
      for (int i = 0; i < quantity_rows; i++) {
        delete[] ship[i];
      }
      delete[] ship;
    }
};