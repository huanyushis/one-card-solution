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

vector<Bus *> buses;//全局bus变量 方便处理
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
        cout << "账户余额为" << this->money << "元" << endl;
    }

    void show_information() {
        cout << "-------------------------------" << endl
             << "-姓名:" << this->name << endl
             << "-性别:" << this->gender << endl
             << "-类型:" << this->type << endl
             << "-学院:" << this->college << endl
             << "-账号:" << this->number << endl
             << "-本月乘车次数:" << this->num << endl
             << "-余额:" << this->money << endl;
        if (this->type == "family") {
            cout << "本月剩余免费成车次数" << 20 - this->num << endl;
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

    bool get_on(Card *a) // 上车
    {
        if (this->number_of_passengers >= this->maximum_number_of_passengers) {
            cout << "车满了，上不了了，等下一趟吧" << endl;
            return false;
        }
        if (a->money < 2) {
            if (a->type == "s") {
                cout << "余额不足，请充值" << endl;
                return false;
            }
            if (a->type == "f" && a->num == 20) {
                cout << "本月最后一次免费乘车" << endl;
                return false;
            }

        }
        this->number_of_passengers++;
        a->num++;
        if (a->num < 20 && a->type == "f") {
            cout << "欢迎乘坐" << endl;

        }
        if (a->num == 20 && a->type == "f") {
            cout << "本月免费乘车次数已用完，本次开始收费" << endl;
        }
        if ((a->type == "s") || (a->num > 20 && a->type == "f")) {
            a->money -= 2;
            cout << "2元" << endl;
            if (a->money < 5) {
                cout << "余额过低，请及时充值" << endl;
            }
        }
        if (a->type == "t") {
            cout << "欢迎乘坐" << endl;
        }
        /*计费*/
        return true;
    }
};

bool drop(MYSQL *mysql, string name) {
    system("cls");
    cout << "你确定要删除本账号吗，此操作不可逆转！按0取消" << endl;
    int a;
    cin >> a;
    if (a) {
        string sql = "delete from card where name=\"" + name + "\";";
        int res = mysql_query(mysql, G2U(sql.c_str()));
        if (res) {
            cout << "删除失败" << endl;
        } else {
            cout << "删除成功" << endl;
        }
        getchar();
        cout << "按任意键返回" << endl;
        getchar();
        return true;
    }
    return false;

}

void chongzhi(MYSQL *mysql, Card *a) {
    system("cls");
    float money;
    cout << "你要充值的钱数:";
    cin >> money;
    a->top_up(money);
    string sql = "UPDATE card set money=" + to_string(a->money) + "where number=" + a->number + ";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (res) {
        cout << "充值失败" << endl;
    } else {
        cout << "充值成功" << endl;
    }
    getchar();
    cout << "按任意键返回" << endl;
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
    cout << "输入你要乘的车:";
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
    cout << "按任意键返回" << endl;
    getchar();
}

void view_c(MYSQL *mysql, Card *a) {
    get_bus_information(mysql);
    while (true) {
        system("cls");
        int choose;
        cout << "--------------------------------------" << endl
             << "         欢迎使用校园一卡通系统           " << endl
             << "                                      " << endl
             << "         一、查询班车信息                " << endl
             << "         二、上车                      " << endl
             << "         三、充值                      " << endl
             << "         四、删卡                      " << endl
             << "         五、退出                      " << endl
             << "                                      " << endl
             << "--------------------------------------" << endl;
        cout << "请选择您的输入:";
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
//                cout << "?γ?:" << sql_row[1] << "   ";
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

    vector<string> names = {"赵锐翰", "孙和光", "李博艺", "王项明", "李博易", "陈安怡", "王华皓", "王博明", "赵正真", "李敏学", "孙心水", "孙弘亮", "陈国兴",
                            "陈正浩", "王高邈", "张升荣", "张奇志", "陈昊英", "李天泽", "赵高雅", "张雪松", "张浩广", "赵信瑞", "孙雅健", "赵刚洁", "李茂实",
                            "陈修德", "王飞驰", "陈乐容", "赵茂勋", "赵浩阔", "王才艺", "李逸明", "赵元化", "赵永嘉", "王宇航", "陈奇文", "陈华彩", "李俊雅",
                            "陈斌蔚", "李弘懿", "李宏朗", "陈宏逸", "李建明", "孙光远", "王高杰", "张鸿光", "赵弘厚", "赵志义", "赵向荣", "王俊人", "陈高杰",
                            "张英范", "李滨海", "张浩波", "陈良平", "李景龙", "孙阳冰", "王阳德", "陈明亮", "赵浩旷", "李俊发", "张鸿羽", "赵俊哲", "王德惠",
                            "孙信鸿", "赵康宁", "孙丰茂", "张奇迈", "李康健", "赵奇玮", "赵向荣", "陈安平", "孙茂材", "李锦程", "赵高洁", "王智勇", "陈鸿畴",
                            "李金鑫", "孙嘉石", "张嘉德", "王欣悦", "赵凯安", "陈鹏赋", "陈阳嘉", "赵嘉谊", "孙明轩", "孙元青", "陈明诚", "陈和通", "陈高畅",
                            "王博易", "王鸿远", "赵子琪", "陈志文", "赵向阳", "赵高丽", "李星阑", "王嘉瑞", "王开宇"};
    vector<string> types = {"big", "smail"};
    vector<string> Times = {"08:00:00", "09:00:00", "10:00:00", "11:00:00", "12:00:00", "13:00:00", "14:00:00",
                            "15:00:00", "16:00:00", "17:00:00", "18:00:00","19:00:00","20:00:00","21:00:00","22:00:00"};
    vector<string> times_ends = {"08:10:00", "09:10:00", "10:10:00", "11:10:00", "12:10:00", "13:10:00", "14:10:00",
                                 "15:10:00", "16:10:00", "17:10:00", "18:10:00","19:10:00","20:10:00","21:10:00","22:10:00"};
    vector<string> license_plate_number = {
            "京A", "京C", "京E", "京F", "京H", "京G", "京B", "津A", "津B", "津C", "津E",
            "沪A", "沪B", "沪D", "沪C", "渝A", "渝B", "渝C", "渝G", "渝H", "冀A",
            "冀B", "冀C", "冀D", "冀E", "冀F", "冀G", "冀H", "冀J", "冀R", "冀T",
            "豫A", "豫B", "豫C", "豫D", "豫E", "豫F", "豫G", "豫H", "豫J", "豫K", "豫L",
            "豫M", "豫N", "豫P", "豫Q", "豫R", "豫S", "豫U", "云A", "云C", "云D", "云E", "云F",
            "云G", "云H", "云J", "云K", "云L", "云M", "云N", "云P", "云Q", "云R ", "云S",
            "辽A", "辽B", "辽C", "辽D", "辽E", "辽F", "辽G", "辽H", "辽J", "辽K", "辽L", "辽M",
            "辽N", "辽P", "辽V", "黑A", "黑B", "黑C", "黑D", "黑E", "黑F", "黑G", "黑H", "黑J", "黑K",
            "黑L", "黑M", "黑N", "黑P", "黑R", "湘A", "湘B", "湘C", "湘D", "湘E", "湘F", "湘G", "湘H", "湘J",
            "湘K", "湘L", "湘M", "湘N", "湘U", "湘S", "皖A", "皖B", "皖C", "皖D", "皖E", "皖F", "皖G", "皖H", "皖J",
            "皖K", "皖L", "皖M", "皖N", "皖P", "皖Q", "皖R", "皖S", "鲁A", "鲁B", "鲁C", "鲁D", "鲁E", "鲁F", "鲁G",
            "鲁H", "鲁J", "鲁K", "鲁L", "鲁M", "鲁N", "鲁P", "鲁Q", "鲁R", "鲁S", "鲁U", "鲁V", "鲁Y", "新A", "新B", "新C",
            "新D", "新E", "新F", "新G", "新H", "新J", "新K", "新L", "新M", "新N", "新P", "新Q", "新R", "苏A", "苏B", "苏C",
            "苏D", "苏E", "苏F", "苏G", "苏H", "苏J", "苏K", "苏L", "苏M", "苏N", "浙A", "浙B", "浙C", "浙D", "浙E", "浙F",
            "浙G", "浙H", "浙J", "浙K ", "浙L", "赣A", "赣B", "赣C", "赣D", "赣E", "赣F", "赣G", "赣H", "赣J", "赣K", "赣L", "赣M", "鄂A",
            "鄂B", "鄂C", "鄂D", "鄂E", "鄂F", "鄂G", "鄂H", "鄂J", "鄂K", "鄂L", "鄂M", "鄂N", "鄂P", "鄂Q", "鄂R", "鄂S", "桂A", "桂B",
            "桂C", "桂D", "桂E", "桂F", "桂G", "桂H", "桂J", "桂K", "桂L", "桂M", "桂N", "桂P", "桂R", "甘A", "甘B", "甘C", "甘D", "甘E",
            "甘F", "甘G", "甘H", "甘J", "甘K", "甘L", "甘M", "甘N", "甘P", "晋A", "晋B", "晋C", "晋D", "晋E", "晋F", "晋H", "晋J", "晋K",
            "晋L", "晋M", "蒙A", "蒙B", "蒙C", "蒙D", "蒙E", "蒙F", "蒙G", "蒙H", "蒙J", "蒙K", "蒙L", "蒙M", "陕A", "陕B", "陕C", "陕D",
            "陕E",
            "陕F", "陕G", "陕H", "陕J", "陕K", "陕U", "陕V", "吉A", "吉B", "吉C", "吉D", "吉E", "吉F", "吉G", "吉H", "吉J", "闽A", "闽B",
            "闽C",
            "闽D", "闽E", "闽F", "闽G", "闽H", "闽J", "闽K", "贵A", "贵B", "贵C", "贵D", "贵E", "贵F", "贵G", "贵H", "贵J", "粤A", "粤B",
            "粤C",
            "粤D", "粤E", "粤F", "粤G", "粤H", "粤J", "粤K", "粤L", "粤M", "粤N", "粤P", "粤Q", "粤R", "粤S", "粤T", "粤U", "粤V", "粤W",
            "粤X", "粤Y",
            "粤Z", "青A", "青B", "青C", "青D", "青E", "青F", "青G", "青H", "藏A", "藏B", "藏C", "藏D", "藏E", "藏F", "藏G", "藏H", "藏J",
            "川A", "川B",
            "川C", "川D", "川E", "川F", "川H", "川J", "川K", "川L", "川M", "川Q", "川R", "川S", "川T", "川U", "川V", "川W", "川X", "川Y",
            "川Z", "宁A",
            "宁B", "宁C", "宁D", "琼A", "琼B", "琼C", "琼D", "琼E"};
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
    cout << "开始创建你的学生卡信息" << endl;
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
    cout << "你的账户已成功创建!" << endl;
    a->show_information();
}

void login(MYSQL *mysql) {
    Card *a = new Card();
    cout << "请输入你的名称:";
    string name;
    cin >> name;
    string sql = "select * from card where name=\"" + name + "\";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (!res) {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result) {
            int numrows = mysql_num_rows(result);
            if (!numrows) {
                cout << "没有查询到信息" << endl;
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
                /*调函数*/
                view_c(mysql, a);
            }
        } else {
            cout << "没有此用户，请前往注册!" << endl;
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
    cout << "按任意键返回" << endl;
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
    cout << "按任意键返回" << endl;
    getchar();
}

void drop(MYSQL *mysql) {
    system("cls");
    string name;
    cout << "请输入你要删除的名字:";
    cin >> name;
    string sql = "delete from card where name=\"" + name + "\";";
    int res = mysql_query(mysql, G2U(sql.c_str()));
    if (res) {
        cout << "删除失败" << endl;
    } else {
        cout << "删除成功" << endl;
    }
    getchar();
    cout << "按任意键返回" << endl;
    getchar();
}

void admin(MYSQL *mysql) {
    int choose;

    while (true) {
        system("cls");

        cout << "--------------------------------------" << endl
             << "         欢迎使用校园一卡通系统           " << endl
             << "                                      " << endl
             << "         一、查询所有车辆信息             " << endl
             << "         二、查询所有用户信息             " << endl
             << "         三、删除用户信息                 " << endl
             << "         四、返回                      " << endl
             << "         五、退出                      " << endl
             << "--------------------------------------" << endl;
        cout << "请选择您的输入:";
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
         << "         欢迎使用校园一卡通系统           " << endl
         << "                                      " << endl
         << "         一、用户登陆                   " << endl
         << "         二、管理员登陆                 " << endl
         << "         三、注册用户                 " << endl
         << "         四、退出                      " << endl
         << "--------------------------------------" << endl;

    cout << "请选择您的输入:";
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
    if (!mysql_real_connect(&mysql, "你的数据库IP地址", "用户名", "密码", "数据库名", 端口号, NULL, 0)) {
        cout << "连接失败" << endl;
        return 0;
    } else {
        init_all(&mysql);
    }
    mysql_set_character_set(&mysql, "utf8");

    while (true)
        view(&mysql);
    return 0;
}