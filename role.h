 #pragma once
//������������ ���� role, �������� ����� Role, ������� ��������� ��������� - ��� ��������� � ���������
//��� ���� Station

class Role {
  public:
    //���������� ���������� ������� � ������� ���������� ��������
    void SetCurrentRoom(int rows, int columns) {
      current_row = rows;//������� ���
      current_column = columns;//������� �������
	  }

	  //���������� �������� ���� � ������� ���������� ��������
    int GetCurrentRow() {
      return current_row;
    }

    //���������� �������� ������� � ������� ���������� ��������
    int GetCurrentColumn() {
      return current_column;
    }

    //������������� ��������� �������� ����� �����\�����
    //������ ���������� ����� ������ �������� ��������� � ��������� 
    //void SetStartHelthPointsValue(int max_stack) {
    //helth_points = max_stack * 2;
    //}

    //������������� ����� �������� ����� �����\�����
    //����� ������� ����
    //void SetNewHelthPointsValue() {
    //helth_points--;
    //}

    //���������� ������� �������� ����� �����\�����
    //int GetHelthPointsValue() {
    //return helth_points;
    //}

    //������������� �������� ������ 
    //� ��������� 1, ���������� ����� ��������� ������� ������
    void SetViktoryValue() {
      viktory = 1;
    }

    //���������� ������� �������� ������, ��� ��������
    bool GetViktoryValue() {
      return viktory;
    }

    //������������� �������� ����
    //� ��������� 1, ����� �������� �������� ����
    void SetKeyValue() {
      key = 1;
    }

    //������������� �������� �����
    //� ��������� 1, ����� �������� �������� �����
    void SetLaserValue() {
      laser = 1;
    }

    //���������� �������\���������� ����� � ���������
    bool GetKeyValue() {
      return key;
    }

  private:
    int current_row;//����� ���� � ������� ���������� ��������
    int current_column;//����� ������� � ������� ���������� ��������
    int room_number;//����� ������� �������
    bool key = 0;//����
    bool laser = 0;//�����
    bool viktory = 0;//������
  //int helth_points = 0;//���� �����
};