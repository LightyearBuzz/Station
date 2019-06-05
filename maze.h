#pragma once
/*Заголовочный фаил maze, в нем описана логика создания, наполнения, работы и удаления
*лабиринта для игры Station
*
*                     Описание алгоритма
*1.Если текущая комната не посещена
*  1.1 Отмечаем комнату как посещенную
*  1.2 Пуш в стек адрес текущей комнаты
*2.Считаем непосещенных соседей
*3.Если непосещенные соседи есть
*  3.1 Случайно выбираем направление(сторону света) в которое будем двигаться.
*  3.2.Указываем дверь с одной стороны
*  3.3.Указываем дверь со второй стороны и рекурсивно вызываем GenRooms() с новой комнатой
*4.Если непосещенных соседей нет и стек не пустой
*  4.1.Возвращаемся по стеку назад на одну позицию 
*  4.2.Рекурсивно вызываем GenRooms() с предыдущей комнатой(которая теперь на вершине стека)
*        тк возможно, что в ней созданы еще не все из возможных дверей
*/


#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack> // для хранения в стеке адресов 

#define BODY "BODY" 
#define COMPUTER "COMPUTER"

using namespace std;

//класс Maze реализует механику лабиринта
class Maze {
  private:
    int visited_rooms_counter = 0;//хранит количество посещенных комнат
    int quantity_rooms = 0;//хранит общее количество комнат в лабиринте
    unsigned short quantity_rows;//хранит количество рядов в лабиринте
    unsigned short quantity_columns;//хранит количество колонн в лабиринте
    char body_string[10] = BODY;//строка тело инопланетянина
    char empty_string_body[1] = "";//если тела нет в комнате
    char computer_string[10] = COMPUTER;//строка компьютер
    char empty_string_computer[1] = "";//если компьютера нет в комнате
	
    //структура содержит состояние комнаты
    struct Room {
      int row_number;//координаты  комнаты, номер ряда в лабиринте
      int column_number;//координаты комнаты, номер колонны в лабиринте
      int room_number;//порядковый номер комнаты
      int doors[4] = {0, 0, 0, 0};// N E S W возможные направления
      int visited = 0;//флаг посещения
      int computer = 0;//флаг наличия компьютера в комнате 
      int body = 0;//флаг наличия тела инопланетянина
    };

    Room** ship;//двумерный динамический массив типа Room для хранения лабиринта 
    stack <struct Room*> s;//стек с адресами комнат
    int max_stack = 0;//хранит максимальную величину стека
	 
  public:
    //создает динамический двумерный массив для хранения лабиринта
    void GenField(int r, int c) {
      quantity_rows = r;
      quantity_columns = c;
      quantity_rooms = quantity_rows * quantity_columns;

      ship = new Room*[quantity_rows];//хранит адрес на массив указателей

      //цикл помещает в каждый элемент массива ship ссылку на массив типа Room 
      for (int i = 0; i < quantity_rows; i++) {
        ship[i] = new Room[quantity_columns];
      }
    }

    //подготавливает каждую комнату
    void PrepRooms() {
      int number = 0;
      
      for (int i = 0; i < quantity_rows; i++) {
        for (int j = 0; j < quantity_columns; j++) {
          ship[i][j].room_number = number;//устанавливает порядковый номер комнаты
          number++;
          ship[i][j].row_number = i;//устанавливает координаты комнаты, номер ряда в лабиринте
          ship[i][j].column_number = j;//устанавливает координаты комнаты, номер колонны в лабиринте
        }
      }
    }

    //возвращает адрес комнаты (0,0)
    Room* GetZeroAdress() {
      return &ship[0][0];
    }

    //возвращает максимальное значение стека
    int GetMaxStack() {
      return max_stack;
    }

    //реализует АЛГОРИТМ Recursive Backtrack 
    //настраивает каждую комнату и связи между ними 
    void GenRooms(Room* r) {
      int number_random_neighbour = 0;//хранит номер случайного соседа от 1 до 4
      bool passible_neighbours[4] = { 0,0,0,0 };//хранит возможных соседей текущей комнаты (NESW)
      int quant_passible_neighbours = 0;//количество возможных соседей от 0 до 4

      //1.Если текущая комната не посещена 
      if (r->visited == 0) {
        r->visited = 1;//1.1 Отмечаем комнату как посещенную 	
        visited_rooms_counter++;
        s.push(r);//1.2 Пуш в стек адрес текущей комнаты
        max_stack++;
      }

		  //2.Считаем непосещенных соседей
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

      //3.Если непосещенные соседи есть
      if (quant_passible_neighbours > 0) {

        //3.1 Случайно выбираем направление(сторону света) в которое будем двигаться.       
        //для определения стороны света нужно из непосещенных соседей случайно выбрать соседа
        number_random_neighbour = rand() % quant_passible_neighbours + 1; 
        
        //определяем какая сторона света соответствует номеру соседа
        //для этого в цикле перебираем стороны света(destination++) в зависимости от уменьшения
        //номера соседа (number_random_neighbour--)
        int destination;//хранит название стороны света в которую нужно идти, в виде числа от 1 до 4 (NESW)
        for (destination = 0; number_random_neighbour > 0; destination++){
          if (passible_neighbours[destination] == 1) {
            number_random_neighbour--;
          }
        }
 
        //3.2.Указываем дверь в текущей комнате в сторону новой комнаты
        r->doors[destination - 1] = 1;

        //3.3.Указываем дверь в новой комнате в сторону старой комнаты и рекурсивно вызываем GenRooms() с новой комнатой
        switch (destination) {
          case 1://на север
            ship[r->row_number - 1][r->column_number].doors[2] = 1;
            GenRooms(&ship[r->row_number - 1][r->column_number]);
            break;
          case 2://на восток
            ship[r->row_number][r->column_number + 1].doors[3] = 1;
            GenRooms(&ship[r->row_number][r->column_number + 1]);
            break;
          case 3://на юг
            ship[r->row_number + 1][r->column_number].doors[0] = 1;
            GenRooms(&ship[r->row_number + 1][r->column_number]);
            break;
          case 4://на запад
            ship[r->row_number][r->column_number - 1].doors[1] = 1;
            GenRooms(&ship[r->row_number][r->column_number - 1]);
            break;
        }

      //4.Если непосещенных соседей нет и стек не пустой	
      } else if (s.size() > 1) { 	

        //4.1.Возвращаемся по стеку назад на одну позицию
        s.pop();	

        //4.2.Рекурсивно вызываем GenRooms() с предыдущей комнатой(которая теперь на вершине стека)
        //тк возможно, что в ней созданы еще не все из возможных дверей
        GenRooms(s.top());
      }
    }

    //помещает компьютер и тело в случайные комнаты
    void GenItems() {
      int computer_address;
      int body_address;

      computer_address = rand() % quantity_rooms;
      body_address = rand() % quantity_rooms;

      for (int i = 0; i < quantity_rows; i++){
        for (int j = 0; j < quantity_columns; j++){
          if (ship[i][j].room_number == computer_address) {
            ship[i][j].computer = 1;//появился компьютер
          }

          if (ship[i][j].room_number == body_address) {
            ship[i][j].body = 1;//появилось тело
          }
        }
      }
    }

    //возвращает количество дверей в заданной комнате
    int GetQuantityDoors(int r, int c){
      int q = 0;
      for (int i = 0; i < 4; i++){
        if (ship[r][c].doors[i] == 1) {
          q++;
        }
      }
      return q;
    }

    //возвращает N если в комнате есть дверь на север
    char GetNorthDirection(int r, int c){
      if (ship[r][c].doors[0] == 1) {
        return 'N';
      } else {
        return ' ';
      }
    }

    //возвращает S если в комнате есть дверь на юг
    char GetSouthDirection(int r, int c){
      if (ship[r][c].doors[2] == 1) {
        return 'S';
      } else {
        return ' ';
      }
    }

    //возвращает E если в комнате есть дверь на восток
    char GetEastDirection(int r, int c){
      if (ship[r][c].doors[1] == 1) {
        return 'E';
      } else {
        return ' ';
      }
    }

    //возвращает W если в комнате есть дверь на запад
    char GetWestDirection(int r, int c){
      if (ship[r][c].doors[3] == 1) {
        return 'W';
      } else {
        return ' ';
      }
    }

    //возвращает BODY если в комнате есть тело
    char* GetBody(int r, int c){
      if (ship[r][c].body == 1) {
        return body_string;
      } else {
        return empty_string_body;
      }
    }

    //возвращает Computer если в комнате есть компьютер
    char* GetComp(int r, int c) {
      if (ship[r][c].computer == 1) {
        return computer_string;
      } else {
        return empty_string_computer;
      }
    }

    //устанавливает значение "тело" в 0
    void SetBodyZero(int r, int c) {
      ship[r][c].body = 0;
    }

    //освобождаем память от лабиринта
    void DeleteField() { 
      for (int i = 0; i < quantity_rows; i++) {
        delete[] ship[i];
      }
      delete[] ship;
    }
};