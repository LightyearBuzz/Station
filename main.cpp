/*Игра Station, написана 14.11.2018
*В ней используется: 
*1.язык С++
*2.объектно-ориентированный подход программирования
*3.шаблон проектирования MVC
*4.алгоритм построения лабиринтов Recursive Backtracker
*
*Место действие игры Station - космический корабль пришельцев, который потерпел крушение.
*При запуске игры пользвателю предлогается указать размеры корабля, корабль это прямоугольный
*лабиринт в котором игроку нужно найти компьютер пришельцев и получить описание гравитационной 
*технологии которая храниться в его памяти. Для доступа к компьютеру нужен электронный ключ, 
*корорый необходимо найти.
*
*
*Программа интересна тем, что при запуске не знает какого размера корабль нужно спроектировать, 
*размеры корабля вводит пользователь. Это означает, что помещение проектируется в момент выполнения,
*а не заранее. Для этого программа динамически выделяет память, и проектирует в ней лабиринт, затем 
*расставляет по лабиринту предметы.
*
*
*                   Устройство шаблона MVC
*Идея шаблона заключается в том, что мы разделяем весь код на 3 логических блока(Model View Controller).
*У каждого блока своое назначение, а главная особенность в том, что любой из них можно легко заменить
*или модифицировать, при этом, практически не касаясь других подсистем. Мы получаем такие преимущества 
*как модульность, расширяемость, простота поддержки и тестирования.
*
*Первый блок - Model 
*Модель отвечает за алгоритмы, расчёты, содержит основную "механику" приложения.
*
*Второй блок - View
*Отвечает за отображение информации. Он содежит все команды вывода текста на консоль.
*
*Третий блок - Controller
*Контроллер принимает команды от пользователя и передает их в модель на обработку
*
*Шаблон MVC подразумевает что блок View следит за состоянием блока Model и при
*изменении состояния выводит на консоль информацию об изменениях. Для реализации этой системы
*мы применим шаблон «Наблюдатель» (Observer).
*
*                 Устройство шаблона Observer
*Идея шаблона "Наблюдатель" заключается в том, что есть два блока кода "Наблюдатель"(Observer)
*и "Наблюдаемое" (Observable), один блок следит за изменениями во втором блоке и реагирует на них.
*Реализовано это так: блок Observable содержит метод notifyUpdate() который в случае изменения 
*состояния Observable вызывает метод Update() в блоке Observer, создается эффект наблюдения 
*Observer за Observable. В реальности же Observable оповещает Observer об изменениях
*
*В нашем случае мы наследуем класс View от класса Observer, и наследуем класс Model от
*класса Observable. Таким образом Model будет оповещать View об изменении своего состояния.
*
*код оформлен в стиле похожем на Google C++ Style
*/


#include "pch.h"
#include "maze.h"//класс maze для работы лабиринта
#include "role.h"//класс role для создания персонажа
#include <stdio.h>//для ввода-вывода
#include <stdlib.h>
#include <Windows.h>//для изменения цвета шрифта в консоли 
#include <stack>//для хранения в стеке адресов  

#define QUANTITY_OBSERVERS 1//количество наблюдателей
#define NORTH "N" 
#define EAST "E"
#define SOUTH "S"
#define WEST "W"
#define EXPLORE "EXPLORE"
#define GET_ACCESS "GET ACCESS"
#define BODY "BODY"
#define QUIT "Q"

using namespace std;

//класс "Наблюдатель" для реализации шаблона MVC
class Observer {
  public:
    virtual void Update() = 0;//функция обновления состояния экрана на каждом шаге
    virtual void ShowStart() = 0;//функция отображения стартового сообщения
    virtual void ShowWrongDirection() = 0;//функция отображения неверного направления 
    virtual void ShowWrongCommand() = 0;//функция отображения неверной команды
    virtual void ShowFindKeyLaser() = 0;//функция отображения получения ключа и лазера
    virtual void ShowViktory() = 0;//функция отображения победы
    virtual void ShowLoose() = 0;//функция отображения проигрыша
};

//класс "Наблюдаемое" для реализации шаблона MVC
class Observable {
  public:
    //позволяет добавить наблюдателя
    void AddObserver(Observer* observer){
      observer_list[observer_counter] = observer;
      observer_counter++;
    }

    //вызывает функию Update в наблюдателе
    void NotifyUpdate() {
      for (int i = 0; i < observer_counter; i++){
        observer_list[i]->Update();
      }
    }

  protected:
    int observer_counter = 0;
    Observer* observer_list[QUANTITY_OBSERVERS];
};

//класс Model(Наблюдаемое) шаблона MVC, содержит всю механику
class ShipModel : public Observable {
  public:
    //создает лабиринт и наполняет его предметами
    void GenMaze(int rows, int columns) {
      current_ship.GenField(rows, columns);
      current_ship.PrepRooms();
      current_ship.GenRooms(current_ship.GetZeroAdress());
      current_ship.GenItems();
    }

    //устанавливает значения текущего положения персонажа
    //и количество очков его жизни
    void SetRanger() {
      ranger.SetCurrentRoom(0, 0);
    //ranger.SetStartHelthPointsValue(current_ship.GetMaxStack());
    }

    //вызывает сообщение из "наблюдателя" о старте
    void NotifyShowStart() {
      for (int i = 0; i < observer_counter; i++){
        observer_list[i]->ShowStart();
      }
    }

    //вызывает сообщение из "наблюдателя" о неверном направлении 
    void NotifyShowWrongDirection() {
      for (int i = 0; i < observer_counter; i++){
        observer_list[i]->ShowWrongDirection();
      }
    }
	
    //вызывает сообщение из "наблюдателя" о неверной команде 
    void NotifyShowWrongCommand() {
      for (int i = 0; i < observer_counter; i++) {
        observer_list[i]->ShowWrongCommand();
      }
    }

    //вызывает сообщение из "наблюдателя" о найденом ключе
    void NotifyFindKey() {
      for (int i = 0; i < observer_counter; i++) {
        observer_list[i]->ShowFindKeyLaser();
      }
    }

    //вызывает сообщение из "наблюдателя" о победе
    void NotifyShowViktory() {
      for (int i = 0; i < observer_counter; i++){
        observer_list[i]->ShowViktory();
      }
    }

    //вызывает сообщение из "наблюдателя" о проигрыше
    void NotifyShowLoose() {
      for (int i = 0; i < observer_counter; i++) {
        observer_list[i]->ShowLoose();
      }
    }

    //возвращает количество дверей в текущей комнате
    int GetQuantityDoors() {	
      int quantity = 0;
      quantity = current_ship.GetQuantityDoors(ranger.GetCurrentRow(), ranger.GetCurrentColumn());
      return quantity;
    }
	
    //возвращает номер ряда текущей комнаты
    int GetCurrentRow() {
      return ranger.GetCurrentRow();
    }

    //возвращает номер колонны текущей комнаты
    int GetCurrentColumn() {
      return ranger.GetCurrentColumn();
    }

    //возвращает состояние "победы"
    int GetViktory() {
      return ranger.GetViktoryValue();
    }

    //возвращает количество очков жизни
    //int GetHelthPoints() {
    //return ranger.GetHelthPointsValue();
    //}

    //возвращает список дверей в комнате
    char* GetDirectionList() {
      direction_list[0] = current_ship.GetNorthDirection(ranger.GetCurrentRow(), ranger.GetCurrentColumn());
      direction_list[1] = ' ';
      direction_list[2] = current_ship.GetEastDirection(ranger.GetCurrentRow(), ranger.GetCurrentColumn());
      direction_list[3] = ' ';
      direction_list[4] = current_ship.GetSouthDirection(ranger.GetCurrentRow(), ranger.GetCurrentColumn());
      direction_list[5] = ' ';
      direction_list[6] = current_ship.GetWestDirection(ranger.GetCurrentRow(), ranger.GetCurrentColumn());
      return direction_list;
    }

    //возвращает список предметов в комнате
    char* GetItemList() {	
      strcpy_s( item_list, "" );
      strcat_s( item_list, current_ship.GetBody(ranger.GetCurrentRow(), ranger.GetCurrentColumn()) );
      strcat_s( item_list, " " );
      strcat_s( item_list, current_ship.GetComp(ranger.GetCurrentRow(), ranger.GetCurrentColumn()) );
      return item_list;
    }

    //обрабатывает ответ пользователя
    void AnswerProcces(char* answer) {	
      
      //тут ВАЖНЫЙ момент: данный блок реализован не через switch, а через if else потому, 
      //что сравнивать нужно строки, а не числа

      //если ответ совпадает с одной из команд, обрабатываем ответ,
      //иначе, сообщаем что команда неверная
      if ( (strncmp( answer, NORTH, sizeof(answer) ) == 0) ||
           (strncmp( answer, EAST, sizeof(answer) ) == 0) ||
           (strncmp( answer, SOUTH, sizeof(answer) ) == 0) ||
           (strncmp( answer, WEST, sizeof(answer) ) == 0) ||
           (strncmp( answer, EXPLORE, sizeof(answer) ) == 0) ||
           (strncmp( answer, GET_ACCESS, sizeof(answer) ) == 0) ||
           (strncmp( answer, QUIT, sizeof(answer) ) == 0) ){
        
        //если ответ совпадает с командой N
        if (strncmp( answer, NORTH, sizeof(answer) ) == 0) {	
          //если есть дверь на север, устанавливаем новые координаты персонажа 
          //и вызываем сообщение о новой комнате
          if ( ( current_ship.GetNorthDirection(ranger.GetCurrentRow(), ranger.GetCurrentColumn() ) ) == 'N' ){
            ranger.SetCurrentRoom(ranger.GetCurrentRow() - 1, ranger.GetCurrentColumn());
            NotifyUpdate();
            return;

          } else {//иначе вызываем сообщение о неверном направлении
            NotifyShowWrongDirection();
            return;
          }
        }
        
        // если ответ совпадает с командой E
        if (strncmp( answer, EAST, sizeof(answer) ) == 0) {	
          //если есть дверь на восток, устанавливаем новые координаты персонажа 
          //и вызываем сообщение о новой комнате
          if ( ( current_ship.GetEastDirection( ranger.GetCurrentRow(), ranger.GetCurrentColumn() ) ) == 'E' ){
            ranger.SetCurrentRoom(ranger.GetCurrentRow(), ranger.GetCurrentColumn() + 1);
            NotifyUpdate();
            return;

          } else { //иначе вызываем сообщение о неверном направлении
            NotifyShowWrongDirection();
            return;
          }
        }

        // если ответ совпадает с командой S
        if (strncmp( answer, SOUTH, sizeof(answer) ) == 0) {
          //если есть дверь на юг, устанавливаем новые координаты персонажа 
          //и вызываем сообщение о новой комнате
          if ( ( current_ship.GetSouthDirection( ranger.GetCurrentRow(), ranger.GetCurrentColumn() ) ) == 'S' ) {
            ranger.SetCurrentRoom( ranger.GetCurrentRow() + 1, ranger.GetCurrentColumn() );
            NotifyUpdate();
            return;

          } else { //иначе вызываем сообщение о неверном направлении
            NotifyShowWrongDirection();
            return;
          }
        }
        
        //если ответ совпадает с командой W
        if (strncmp( answer, WEST, sizeof(answer) ) == 0) {
          //если есть дверь на запад, устанавливаем текущие координаты персонажа
          //и вызываем сообщение о новой комнате
          if ( ( current_ship.GetWestDirection( ranger.GetCurrentRow(), ranger.GetCurrentColumn() ) ) == 'W') {
            ranger.SetCurrentRoom(ranger.GetCurrentRow(), ranger.GetCurrentColumn() - 1);   
            NotifyUpdate();
            return;

          } else { //иначе вызываем сообщение о неверном направлении
            NotifyShowWrongDirection();
            return;
          }
        }

        //если ответ совпадает с командой EXPLORE
        if (strncmp( answer, EXPLORE, sizeof(answer) ) == 0) {
          //если в комнате есть тело, осмотреть его:
          //       -исчезает тело
          //       -в инвентаре появляется ключ и лазер
          //       -вывод сообщения о находке

          if (strncmp( current_ship.GetBody(ranger.GetCurrentRow(), ranger.GetCurrentColumn() ),
            BODY, sizeof(BODY) ) == 0) {
            current_ship.SetBodyZero(ranger.GetCurrentRow(), ranger.GetCurrentColumn());
            ranger.SetKeyValue();
            NotifyFindKey();
            return;

          } else {
            printf("There is nothing to explore.\n");
            return;
          }
        }

        // если ответ совпадает с командой GET ACCESS
        if (strncmp( answer, GET_ACCESS, sizeof(answer) ) == 0) {
          //если в комнате есть компьютер и у персонажа есть ключ, 
          //показываем сообщение победы
          if (strncmp( current_ship.GetComp( ranger.GetCurrentRow(), ranger.GetCurrentColumn() ),
					COMPUTER, sizeof(COMPUTER) ) == 0 && ranger.GetKeyValue() == 1 ) {
            ranger.SetViktoryValue();
            NotifyShowViktory();
            return;

          } else {
            printf("There is no computer or you have no electronic key.\n");
            return;
          }
        }

        // если ответ совпадает с командой Q
        if (strncmp(answer, QUIT, sizeof(answer)) == 0) {
          return;
        }


      } else {
      NotifyShowWrongCommand();
      return;
      }
    }

    //освобождает память от лабиринта
    void DeleteMaze() {
      current_ship.DeleteField();
    }

  private:
    Role ranger;//персонаж
    Maze current_ship;//лабиринт
    char item_list[40]="";//список предметов в комнате
    char direction_list[10] = "";//список дверей из комнаты
};

//класс View(Наблюдатель) для реализации MVC
class shipView : public Observer {
  public:
    //конструктор записывет свой экземпляр в список наблюдателей в модели
    shipView(ShipModel* m) {
      model = m;
      model->AddObserver(this);
    }
	
    //отображает сообщение старта
    virtual void ShowStart() {	
      printf("Enter size of the Ship(rows from 1 to 40, columns from 1 to 40)\n");
    }

    //отображает сообщение хода
    virtual void Update() {	
      printf("__________________________________________________________\n");
      printf("You are in the room [%d, %d]. There are [%d] doors: [%s].\nItems in the room: [%s].\n\n",
        model->GetCurrentRow(), model->GetCurrentColumn(), model->GetQuantityDoors(), 
        model->GetDirectionList(), model->GetItemList());

      printf("Enter the command:\n");
      printf("- N, S, W, E to move.\n");
      printf("- explore, to explore some thing.\n");
      printf("- get access, to get access to the computer.\n");
      printf("- q to quit.\n");
    }

    //отображает сообщение "неверная команда"
    virtual void ShowWrongCommand() {	
      printf("Wrong command!\n");
      printf("Enter the command:\n");
      printf("- N, S, W, E to move.\n");
      printf("- explore to explore some thing.\n");
      printf("- get access, to get access to the computer.\n");
      printf("- q to quit.\n");
    }

    //отображает сообщение "неверная направление"
    virtual void ShowWrongDirection() {
      printf("You can't pass through the wall.\n");
    }

    //отображает сообщение "победа"
    virtual void ShowViktory() {
      SetConsoleTextAttribute(console, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
      printf("You got information about gravity technology!\n");
      SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    //отображает сообщение "найдены предметы"
    virtual void ShowFindKeyLaser() {	
      SetConsoleTextAttribute(console, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
      printf("You have explored the body of alien solder. You have found electronic key! You have found Alien laser!\n");
      SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    //отображает сообщение "проигрыш"
    virtual void ShowLoose() {
      SetConsoleTextAttribute(console, FOREGROUND_INTENSITY | FOREGROUND_RED);
      printf("You die of radiation!!! The alien ship devoured you!!!\n");
      SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
 
  private:
    ShipModel *model;//объект модели
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);// создаем объект управления консолью
};

//класс Controller шаблона MVC, содержит пульт управления
class shipController {
  public:
    // конструктор запоминает адрес модели
    shipController(ShipModel* m) {
      model = m;
    }

    //спрашивает у пользователя длинну и ширину лабиринта от 1-40
    //затем создает отдает команду модели создать лабиринт
    void Start() {
      model->NotifyShowStart();
      std::cin >> rows;
      getchar();
      std::cin >> columns;
      getchar();

      //проверяет ввод пользователя
      while (rows < 0 || rows > 40 || columns < 0 || columns > 40) {	
        model->NotifyShowStart();
        std::cin >> rows;
        getchar();
        std::cin >> columns;
        getchar();
      }
      
      model->GenMaze(rows, columns);
      model->SetRanger();
      model->NotifyUpdate();

      //принимаем команды пользователя в цикле пока пользователь
      //не победит или проиграет или не выйдет. И отправляем команду на обработку
      do {
        cin.getline(answer, 15);
        for (int i = 0; i < sizeof(answer) - 1; i++) {
          answer[i] = toupper(answer[i]);//преобразует ответ в верхний регистр
        }
        model->AnswerProcces(answer);
      } while (strncmp( answer, QUIT, sizeof(answer) ) != 0 &&
                    model->GetViktory() == 0);

      model->DeleteMaze();
    }
  private:
    int rows = 0;//количество рядов
    int columns = 0;//количество колонн
    char answer[15];//хранит ответ пользователя
    ShipModel* model;
};

int main(void) {	
  ShipModel model;
  shipView view(&model);
  shipController controller(&model);

  controller.Start();
  return 0;
}