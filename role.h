 #pragma once
//Подключаемый фаил role, содержит класс Role, который описывает персонажа - его состояние и поведение
//для игры Station

class Role {
  public:
    //запоминает координаты комнаты в которой находиться персонаж
    void SetCurrentRoom(int rows, int columns) {
      current_row = rows;//текущий ряд
      current_column = columns;//текущая колонна
	  }

	  //возвращает значение ряда в котором находиться персонаж
    int GetCurrentRow() {
      return current_row;
    }

    //возвращает значение колонны в которой находиться персонаж
    int GetCurrentColumn() {
      return current_column;
    }

    //устанавливает стартовое значение очков жизни\ходов
    //удвоив расстояние между самыми дальними комнатами в лабиринте 
    //void SetStartHelthPointsValue(int max_stack) {
    //helth_points = max_stack * 2;
    //}

    //устанавливает новое значение очков жизни\ходов
    //после каждого хода
    //void SetNewHelthPointsValue() {
    //helth_points--;
    //}

    //возвращает текущее значение очков жизни\ходов
    //int GetHelthPointsValue() {
    //return helth_points;
    //}

    //устанавливает значение победа 
    //в положение 1, вызывается когда выполнены условия победы
    void SetViktoryValue() {
      viktory = 1;
    }

    //возвращает текущее значение победы, для проверки
    bool GetViktoryValue() {
      return viktory;
    }

    //устанавливает значение ключ
    //в положение 1, когда персонаж получает ключ
    void SetKeyValue() {
      key = 1;
    }

    //устанавливает значение лазер
    //в положение 1, когда персонаж получает лазер
    void SetLaserValue() {
      laser = 1;
    }

    //возвращает наличие\отсутствие ключа у персонажа
    bool GetKeyValue() {
      return key;
    }

  private:
    int current_row;//номер ряда в котором находиться персонаж
    int current_column;//номер колонны в которой находиться персонаж
    int room_number;//номер текущей комнаты
    bool key = 0;//ключ
    bool laser = 0;//лазер
    bool viktory = 0;//победа
  //int helth_points = 0;//очки жизни
};