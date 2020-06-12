#include <iostream>
#include <string>
#include "mysql.h"
#include <vector>
#include <ctime>
#include <stdio.h>
#include <vector>

class Card;

class Bus;

using namespace std;

void show_all_car();

void show_all_car(MYSQL *mysql);

void create(MYSQL *mysql);

void select(MYSQL *mysql);

void create_tables(MYSQL *mysql);

void create_bus(MYSQL *mysql);

void init_all(MYSQL *mysql);

void insert_bus(MYSQL *mysql);

int insert_card(MYSQL *mysql, Card *a);

char *G2U(const char *gb2312);

char *U2G(const char *utf8);

vector<Bus *> buses;//ȫ��bus���� ���㴦��
char *U2G(const char *utf8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t *wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char *str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

char *G2U(const char *gb2312) {
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t *wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char *str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}


class Card {
public:
    int num;//??????????
    string name;//????
    char gender;//???
    string type;//????????????????????
    string college;//??
    string number;//???
    float money;//???
    Card() {}

    Card(string name, char gender, string type, string college, string n, float money) : name(name),
                                                                                         gender(gender),
                                                                                         type(type),
                                                                                         college(college),
                                                                                         money(money),
                                                                                         number(n),
                                                                                         num(0) {

    }


    void top_up(float new_money)//??
    {
        this->money += new_money;
        cout << "�˻����Ϊ" << this->money << "Ԫ" << endl;
    }

    void show_information() {
        cout << "-------------------------------" << endl
             << "-����:" << this->name << endl
             << "-�Ա�:" << this->gender << endl
             << "-����:" << this->type << endl
             << "-ѧԺ:" << this->college << endl
             << "-�˺�:" << this->number << endl
             << "-���³˳�����:" << this->num << endl
             << "-���:" << this->money << endl;
        if (this->type == "family") {
            cout << "����ʣ����ѳɳ�����" << 20 - this->num << endl;
        }
        cout << "------------------------------" << endl;
    }

};

class Bus {
public:
    string type;//???
    string license_plate_number;//?????
    string driver_name;//???????
    int maximum_number_of_passengers;//?????????
    int number_of_passengers;//????????
    float fare;//????
    string time;//???????
    string time_end;

    Bus() {}

    Bus(string t, string license_plate_number, string driver_name, int maximum_number_of_passengers,
        int number_of_passengers, float fare)
            : type(t), license_plate_number(license_plate_number), driver_name(driver_name),
              maximum_number_of_passengers(maximum_number_of_passengers), number_of_passengers(number_of_passengers),
              fare(fare) {
    }

    void show_information() //?????????
    {
        cout << "-license_plate_number:" << this->license_plate_number << endl
             << "-type:" << this->type << endl
             << "-maximum_number_of_passengers:" << this->maximum_number_of_passengers << endl
             << "-driver_name:" << this->driver_name << endl;
        getchar();
    }

    bool get_on(Card *a) // �ϳ�
    {
        if (this->number_of_passengers >= this->maximum_number_of_passengers) {
            cout << "�����ˣ��ϲ����ˣ�����һ�˰�" << endl;
            return false;
        }
        if (a->money < 2) {
            if (a->type == "s") {
                cout << "���㣬���ֵ" << endl;
                return false;
            }
            if (a->type == "f" && a->num == 20) {
                cout << "�������һ����ѳ˳�" << endl;
                return false;
            }

        }
        this->number_of_passengers++;
        a->num++;
        if (a->num < 20 && a->type == "f") {
            cout << "��ӭ����" << endl;

        }
        if (a->num == 20 && a->type == "f") {
            cout << "������ѳ˳����������꣬���ο�ʼ�շ�" << endl;
        }
        if ((a->type == "s") || (a->num > 20 && a->type == "f")) {
            a->money -= 2;
            cout << "2Ԫ" << endl;
            if (a->money < 5) {
                cout << "�����ͣ��뼰ʱ��ֵ" << endl;
            }
        }
        if (a->type == "t") {
            cout << "��ӭ����" << endl;
        }
        /*�Ʒ�*/
        return true;
    }
};

bool drop(MYSQL *mysql, string name) {
    system("cls");
    cout << "��ȷ��Ҫɾ�����˺��𣬴˲���������ת����0ȡ��" << endl;
    int a;
    cin >> a;
    if (a) {
        string sql = "delete from card where name=\"" + name + "\";";
        int res = mysql_query(mysql, G2U(sql.c_str()));
        if (res) {
            cout << "ɾ��ʧ��" << endl;
        } else {
            cout << "ɾ���ɹ�" << endl;
        }
        getchar();
        cout << "�����������" << endl;
        getchar();
        return true;
    }
    return false;

}

void chongzhi(MYSQL *mysql, Card *a) {
    system("cls");
    float money;
    cout << "��Ҫ��ֵ��Ǯ��:";
    cin >> money;
    a->top_up(money);
    string sql = "UPDATE card set money=" + to_string(a->money) + "where number=" + a->number + ";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (res) {
        cout << "��ֵʧ��" << endl;
    } else {
        cout << "��ֵ�ɹ�" << endl;
    }
    getchar();
    cout << "�����������" << endl;
    getchar();
}

void get_bus_information(MYSQL *mysql) {
    string sql = "SELECT * FROM bus;";
    while (!buses.empty()) {
        Bus *a = buses.front();
        buses.pop_back();
        delete a;
    }
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (!res) {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result) {
            int numrows = mysql_num_rows(result);
            while (MYSQL_ROW sql_row = mysql_fetch_row(result)) {
                Bus *a = new Bus();
                a->type = U2G(sql_row[1]);
                a->license_plate_number = U2G(sql_row[2]);
                a->driver_name = U2G(sql_row[3]);
                a->maximum_number_of_passengers = atoi(U2G(sql_row[4]));
                a->number_of_passengers = atoi(U2G(sql_row[5]));
                a->time = U2G(sql_row[6]);
                a->time_end = U2G(sql_row[7]);
                buses.push_back(a);
            }
        }
    }
    getchar();

    cout<<buses.size();
    getchar();
}

void get_on(MYSQL *mysql, Card *a) {
    show_all_car(mysql);
    cout << "������Ҫ�˵ĳ�:";
    int num;
    cin >> num;
    Bus *b = buses[num-1];
    b->get_on(a);
    string sql = "UPDATE card set money=" + to_string(a->money) + "where number=" + a->number + ";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    sql = "UPDATE bus set number_of_passengers=" + to_string(b->number_of_passengers) + "where license_plate_number=" +
          b->license_plate_number + ";";
    res = mysql_query(mysql, G2U(sql.c_str()));
    getchar();
    cout << "�����������" << endl;
    getchar();
}

void view_c(MYSQL *mysql, Card *a) {
    get_bus_information(mysql);
    while (true) {
        system("cls");
        int choose;
        cout << "--------------------------------------" << endl
             << "         ��ӭʹ��У԰һ��ͨϵͳ           " << endl
             << "                                      " << endl
             << "         һ����ѯ�೵��Ϣ                " << endl
             << "         �����ϳ�                      " << endl
             << "         ������ֵ                      " << endl
             << "         �ġ�ɾ��                      " << endl
             << "         �塢�˳�                      " << endl
             << "                                      " << endl
             << "--------------------------------------" << endl;
        cout << "��ѡ����������:";
        cin >> choose;
        switch (choose) {
            case 1:
                show_all_car(mysql);
                break;
            case 2:
                get_on(mysql, a);
                break;
            case 3:
                chongzhi(mysql, a);
                break;
            case 4:
                if (drop(mysql, a->name))
                    delete a;
                return;
                break;
            case 5:
                return;
            default:
                cout << "error input" << endl;
                cout << "exiting" << endl;
                exit(0);
        }
    }
}


//void select(MYSQL *mysql) {
//    MYSQL_RES *result;
//    MYSQL_ROW sql_row;
//    int res = mysql_query(mysql, "select * from card;");
//    if (!res) {
//        result = mysql_store_result(mysql);
//        if (result) {
//            while (sql_row = mysql_fetch_row(result))//????????????
//            {
//                cout << "?��?:" << sql_row[1] << "   ";
//                cout << "???:" << sql_row[2] << "   ";
//                cout << "???1???:" << sql_row[2] << "   ";
//                cout << "???2???" << sql_row[2] << "   ";
//                cout << "???3???" << sql_row[2] << endl;
//            }
//        } else {
//            cout << "query sql failed!" << endl;
//        }
//    } else {
//        cout << "??????" << endl;
//    }
//}

void create_tables(MYSQL *mysql) {
    string sql;
    sql = "CREATE TABLE IF NOT EXISTS card ("
          "name VARCHAR(20) not null,"
          "gender CHAR,"
          "type VARCHAR(1) not null,"
          "college VARCHAR(20) ,"
          "number VARCHAR(8) not null,"
          "money float default 0.0,"
          "num int default 0,"
          "PRIMARY KEY ( number,type ))"
          "default charset = utf8mb4;";
    int a = mysql_query(mysql, sql.c_str());
    if (a) {
        cout << "error to create table card";
    }
    sql =
            "CREATE TABLE IF NOT EXISTS bus("
            "number int NOT NULL AUTO_INCREMENT,"
            "type varchar(10) NOT NULL,"
            "license_plate_number varchar(20) NOT NULL,"
            "driver_name varchar(20) NOT NULL,"
            "maximum_number_of_passengers int NOT NULL,"
            "number_of_passengers int NOT NULL,"
            "fare float NULL default 2.0,"
            "departure_time time NULL,"
            "departure_time_end time NULL,"
            "PRIMARY KEY (number)"
            ")default charset = utf8mb4;";
    a = mysql_query(mysql, sql.c_str());
    if (a) {
        cout << "error to create table bus";
    }
}

void create_bus(MYSQL *mysql) {
    MYSQL_RES *result;
    MYSQL_ROW sql_row;

    mysql_query(mysql, "select * from bus;");
    result = mysql_store_result(mysql);
    if (result) {
        int numrows = mysql_num_rows(result);
        if (!numrows) {
            insert_bus(mysql);
        }
    } else {
        cout << "query sql failed!" << endl;
    }
}

void init_all(MYSQL *mysql) {
    create_tables(mysql);
    create_bus(mysql);
}

void insert_bus(MYSQL *mysql) {
    Bus *a = new Bus();
    string sql;

    vector<string> names = {"����", "��͹�", "���", "������", "���", "�°���", "�����", "������", "������", "����ѧ", "����ˮ", "�����", "�¹���",
                            "������", "������", "������", "����־", "���Ӣ", "������", "�Ը���", "��ѩ��", "�źƹ�", "������", "���Ž�", "�Ըս�", "��ïʵ",
                            "���޵�", "���ɳ�", "������", "��ïѫ", "�Ժ���", "������", "������", "��Ԫ��", "������", "���", "������", "�»���", "���",
                            "�±�ε", "���ܲ", "�����", "�º���", "���", "���Զ", "���߽�", "�ź��", "�Ժ��", "��־��", "������", "������", "�¸߽�",
                            "��Ӣ��", "�����", "�źƲ�", "����ƽ", "���", "������", "������", "������", "�Ժƿ�", "���", "�ź���", "�Կ���", "���»�",
                            "���ź�", "�Կ���", "���ï", "������", "���", "������", "������", "�°�ƽ", "��ï��", "�����", "�Ը߽�", "������", "�º��",
                            "�����", "���ʯ", "�żε�", "������", "�Կ���", "������", "������", "�Լ���", "������", "��Ԫ��", "������", "�º�ͨ", "�¸߳�",
                            "������", "����Զ", "������", "��־��", "������", "�Ը���", "������", "������", "������"};
    vector<string> types = {"big", "smail"};
    vector<string> Times = {"08:00:00", "09:00:00", "10:00:00", "11:00:00", "12:00:00", "13:00:00", "14:00:00",
                            "15:00:00", "16:00:00", "17:00:00", "18:00:00","19:00:00","20:00:00","21:00:00","22:00:00"};
    vector<string> times_ends = {"08:10:00", "09:10:00", "10:10:00", "11:10:00", "12:10:00", "13:10:00", "14:10:00",
                                 "15:10:00", "16:10:00", "17:10:00", "18:10:00","19:10:00","20:10:00","21:10:00","22:10:00"};
    vector<string> license_plate_number = {
            "��A", "��C", "��E", "��F", "��H", "��G", "��B", "��A", "��B", "��C", "��E",
            "��A", "��B", "��D", "��C", "��A", "��B", "��C", "��G", "��H", "��A",
            "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��R", "��T",
            "ԥA", "ԥB", "ԥC", "ԥD", "ԥE", "ԥF", "ԥG", "ԥH", "ԥJ", "ԥK", "ԥL",
            "ԥM", "ԥN", "ԥP", "ԥQ", "ԥR", "ԥS", "ԥU", "��A", "��C", "��D", "��E", "��F",
            "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��P", "��Q", "��R ", "��S",
            "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M",
            "��N", "��P", "��V", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��K",
            "��L", "��M", "��N", "��P", "��R", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J",
            "��K", "��L", "��M", "��N", "��U", "��S", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J",
            "��K", "��L", "��M", "��N", "��P", "��Q", "��R", "��S", "³A", "³B", "³C", "³D", "³E", "³F", "³G",
            "³H", "³J", "³K", "³L", "³M", "³N", "³P", "³Q", "³R", "³S", "³U", "³V", "³Y", "��A", "��B", "��C",
            "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��P", "��Q", "��R", "��A", "��B", "��C",
            "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��A", "��B", "��C", "��D", "��E", "��F",
            "��G", "��H", "��J", "��K ", "��L", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��A",
            "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��P", "��Q", "��R", "��S", "��A", "��B",
            "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��P", "��R", "��A", "��B", "��C", "��D", "��E",
            "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��P", "��A", "��B", "��C", "��D", "��E", "��F", "��H", "��J", "��K",
            "��L", "��M", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��A", "��B", "��C", "��D",
            "��E",
            "��F", "��G", "��H", "��J", "��K", "��U", "��V", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��A", "��B",
            "��C",
            "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J", "��A", "��B",
            "��C",
            "��D", "��E", "��F", "��G", "��H", "��J", "��K", "��L", "��M", "��N", "��P", "��Q", "��R", "��S", "��T", "��U", "��V", "��W",
            "��X", "��Y",
            "��Z", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��A", "��B", "��C", "��D", "��E", "��F", "��G", "��H", "��J",
            "��A", "��B",
            "��C", "��D", "��E", "��F", "��H", "��J", "��K", "��L", "��M", "��Q", "��R", "��S", "��T", "��U", "��V", "��W", "��X", "��Y",
            "��Z", "��A",
            "��B", "��C", "��D", "��A", "��B", "��C", "��D", "��E"};
    time_t now = time(0);
    srand(now);
    for (int c = 0; c < Times.size() * 2; c++) {
        cout << c + 1 << " ";
        a->driver_name = names[rand() % names.size()];
        a->type = types[rand() % types.size()];
        a->fare = 2;
        if (a->type == "big")
            a->maximum_number_of_passengers = 50;
        else
            a->maximum_number_of_passengers = 30;
        a->number_of_passengers = rand() % a->maximum_number_of_passengers;
        a->license_plate_number = license_plate_number[rand() % license_plate_number.size()] +
                                  to_string(10000 + (int) (rand() % (60000 + 10000 - 1)));
        a->time = Times[c / 2];
        a->time_end = times_ends[c / 2];
        sql = "insert into bus values(" + to_string(c + 1) + ",\"" + a->type + "\",\"" + a->license_plate_number +
              "\",\"" +
              a->driver_name + "\",\"" +
              to_string(a->maximum_number_of_passengers) + "\"," + to_string(0) + "," + to_string(a->fare) + ",\"" +
              a->time + "\",\"" + a->time_end + "\");";
        int b = mysql_query(mysql, G2U(sql.c_str()));
    }
    delete a;
}

int insert_card(MYSQL *mysql, Card *a) {
    string sql;
    sql = "insert into card values(\"" + a->name + "\",\"" + a->gender + "\",\"" + a->type + "\",\"" +
          a->college + "\",\"" +
          a->number + "\"," + to_string(a->money) + "," + to_string(a->num) + ");";
    int b = mysql_query(mysql, G2U(sql.c_str()));
    cout << sql.c_str() << endl;
    return b;
}

void create(MYSQL *mysql) {
    system("cls");
    cout << "��ʼ�������ѧ������Ϣ" << endl;
    Card *a = new Card();
    a->num = 0;
    cout << "input your name";
    cin >> a->name;
    int swi = 0;
    cout << "switch your gender(1.M 2.W):";
    cin >> swi;
    while (swi > 2 || swi < 1) {
        cout << "input error:";
        cin >> swi;
    }
    switch (swi) {
        case 1:
            a->gender = 'M';
            break;
        case 2:
            a->gender = 'N';
    }
    cout << "switch your type(1.student,2.teacher,3.family):";
    cin >> swi;
    while (swi > 3 || swi < 1) {
        cout << "input error:";
        cin >> swi;
    }
    switch (swi) {
        case 1:
            a->type = "s";
            break;
        case 2:
            a->type = "t";
            break;
        case 3:
            a->type = "f";
    }
    if (a->type != "f") {
        cout << "switch your college:";
        cin >> a->college;
    }
    a->money = 0;
    time_t now = time(0);
    string b(8, '0');
    srand(now);
    for (int i = 0; i < 8; i++) {
        b[i] = ('0' + rand() % (10));
    }
    a->number = b;
    int n = 0;
    cout << insert_card(mysql, a) << endl;
    cout << "����˻��ѳɹ�����!" << endl;
    a->show_information();
}

void login(MYSQL *mysql) {
    Card *a = new Card();
    cout << "�������������:";
    string name;
    cin >> name;
    string sql = "select * from card where name=\"" + name + "\";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (!res) {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result) {
            int numrows = mysql_num_rows(result);
            if (!numrows) {
                cout << "û�в�ѯ����Ϣ" << endl;
                return;
            }
            cout << endl;
            if (MYSQL_ROW sql_row = mysql_fetch_row(result)) {
                a->name = U2G(sql_row[0]);
                a->gender = *U2G(sql_row[1]);
                a->type = U2G(sql_row[2]);
                a->college = U2G(sql_row[3]);
                a->number = U2G(sql_row[4]);
                a->money = atoi(U2G(sql_row[5]));
                a->num = atof(U2G(sql_row[6]));
//                a->show_information();
                /*������*/
                view_c(mysql, a);
            }
        } else {
            cout << "û�д��û�����ǰ��ע��!" << endl;
            return;
        }
        delete a;
    }
}

//void show_all_car() {
//    for (int i = 0; i < buses.size(); i++) {
//        cout << i << " ";
//        buses[i]->show_information();
//    }
//}

void show_all_car(MYSQL *mysql) {
    system("cls");
    cout << "--------------------------------------" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    string sql = "SELECT * FROM bus WHERE departure_time_end<CURRENT_TIME;";

    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (!res) {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result) {
            int numrows = mysql_num_rows(result);
            cout << endl;
            while (MYSQL_ROW sql_row = mysql_fetch_row(result)) {
                printf("%-2d ", atoi(U2G(sql_row[0])));
                for (int i = 1; i < mysql_num_fields(result); i++) {
                    cout << U2G(sql_row[i]) << " ";
                }
                cout << endl;

            }
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    sql = "SELECT * FROM bus WHERE departure_time_end>=CURRENT_TIME;";

    res = mysql_query(mysql, G2U(sql.c_str()));
    if (!res) {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result) {
            int numrows = mysql_num_rows(result);
            while (MYSQL_ROW sql_row = mysql_fetch_row(result)) {
                printf("%-2d ", atoi(U2G(sql_row[0])));
                for (int i = 1; i < mysql_num_fields(result); i++) {
                    cout << U2G(sql_row[i]) << " ";
                }
                cout << endl;
            }

        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
    getchar();
    cout << "�����������" << endl;
    getchar();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
                                                             FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void show_all_people(MYSQL *mysql) {
    system("cls");
    cout << "--------------------------------------" << endl;
    string sql = "SELECT * FROM card;";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (!res) {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result) {
            int numrows = mysql_num_rows(result);
            cout << endl;
            while (MYSQL_ROW sql_row = mysql_fetch_row(result)) {
                for (int i = 0; i < mysql_num_fields(result); i++) {
                    cout << U2G(sql_row[i]) << " ";
                }
                cout << endl;
            }
        }
    }
    getchar();
    cout << "�����������" << endl;
    getchar();
}

void drop(MYSQL *mysql) {
    system("cls");
    string name;
    cout << "��������Ҫɾ��������:";
    cin >> name;
    string sql = "delete from card where name=\"" + name + "\";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (res) {
        cout << "ɾ��ʧ��" << endl;
    } else {
        cout << "ɾ���ɹ�" << endl;
    }
    getchar();
    cout << "�����������" << endl;
    getchar();
}

void admin(MYSQL *mysql) {
    int choose;

    while (true) {
        system("cls");

        cout << "--------------------------------------" << endl
             << "         ��ӭʹ��У԰һ��ͨϵͳ           " << endl
             << "                                      " << endl
             << "         һ����ѯ���г�����Ϣ             " << endl
             << "         ������ѯ�����û���Ϣ             " << endl
             << "         ����ɾ���û���Ϣ                 " << endl
             << "         �ġ�����                      " << endl
             << "         �塢�˳�                      " << endl
             << "--------------------------------------" << endl;
        cout << "��ѡ����������:";
        cin >> choose;
        switch (choose) {
            case 1:
                show_all_car(mysql);
                break;
            case 2:
                show_all_people(mysql);
                break;
            case 3:
                drop(mysql);
                break;
            case 4:
                return;
            case 5:
                exit(0);
            default:
                cout << "error input" << endl;
                cout << "exiting" << endl;
                exit(0);
        }
    }
}

void view(MYSQL *mysql) {
    int choose;
    cout << "--------------------------------------" << endl
         << "         ��ӭʹ��У԰һ��ͨϵͳ           " << endl
         << "                                      " << endl
         << "         һ���û���½                   " << endl
         << "         ��������Ա��½                 " << endl
         << "         ����ע���û�                 " << endl
         << "         �ġ��˳�                      " << endl
         << "--------------------------------------" << endl;

    cout << "��ѡ����������:";
    cin >> choose;
    switch (choose) {
        case 1:
            login(mysql);
            break;
        case 2:
            admin(mysql);
            break;
        case 3:
            create(mysql);
            break;
        case 4:
            exit(0);
        default:
            cout << "error input" << endl;
            cout << "exiting" << endl;
            exit(0);
    }
}

int main() {

    MYSQL mysql;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "������ݿ�IP��ַ", "�û���", "����", "���ݿ���", �˿ں�, NULL, 0)) {
        cout << "����ʧ��" << endl;
        return 0;
    } else {
        init_all(&mysql);
    }
    mysql_set_character_set(&mysql, "utf8");

    while (true)
        view(&mysql);
    return 0;
}