#include "my_module.h"

int outputFile(string &name) {
    ifstream file(name, ios::binary | ios::in);
    if (!file) {
        cerr << "File opening error " << name << endl;
        return 1;
    }

    cout << endl << name << " contents:" << endl;

    Client client{};
    cout << "|                                        Surname                                         | Start time | End time |" << endl;
    while (file.read((char *) &client, sizeof(client))) {
        cout << "| " << client.name;
        for(int i = 0; i < 87 - strlen(client.name); i++) cout << " ";
        cout << "| ";

        if (client.start.hour < 10) cout << "0" << client.start.hour; // also possible cout.fill() + width()
        else cout << client.start.hour;

        if (client.start.min < 10) cout << ":" << "0" << client.start.min << "      | ";
        else cout << ":" << client.start.min << "      | ";

        if (client.end.hour < 10) cout << "0" << client.end.hour;
        else cout << client.end.hour;

        if (client.end.min < 10) cout << ":" << "0" << client.end.min << "    |" << endl;
        else cout << ":" << client.end.min << "    |" << endl;
    }
    cout << endl;
    file.close();
    return 0;
}

void inputFile(string &name) {
    char choice, colon, surname[MAX];
    Client cl{};

    do {
        while (true) {
            cout << "Enter a surname: ";
            cin.getline(surname, MAX);
            if (cin.fail() || (strlen(surname) == MAX - 1 && surname[MAX - 2] != '\n')) {
                cout << "Invalid input. Please enter surname up to " << MAX - 1 << " characters" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }else {
                strcpy(cl.name, surname);
                break;
            }
        }

        while (true) {
            cout << "Enter a start time (HH:MM): ";
            cin >> cl.start.hour >> colon >> cl.start.min;
            if (cin.fail() || colon != ':' || cl.start.hour < 0 || cl.start.hour > 23 || cl.start.min < 0 ||
                cl.start.min > 59) {
                cout << "Invalid input. Please enter correct time in correct HH:MM format." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        }

        while (true) {
            cout << "Enter an end time (HH:MM): ";
            cin >> cl.end.hour >> colon >> cl.end.min;
            if (cin.fail() || colon != ':' || cl.end.hour < 0 || cl.end.hour > 23 || cl.end.min < 0 ||
                cl.end.min > 59) {
                cout << "Invalid input. Please enter correct time in correct HH:MM format." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        }

        if (!validInput(name, cl)) {
            ofstream file(name, ios::binary | ios::app);
            if (!file) {
                cerr << "File opening error " << name << endl;
                return;
            }
            file.write((char *) &cl, sizeof(cl));
            file.close();
        } else {
            continue;
        }

        cout << "Do you want to continue printing (Y/N):_";
        cin >> choice;
        cin.ignore();
    } while (choice != 'N' && choice != 'n');
}

int validInput(string &name, Client &client) {
    tm start_cl = {};
    start_cl.tm_hour = client.start.hour;
    start_cl.tm_min = client.start.min;
    start_cl.tm_mday = 1;
    start_cl.tm_year = 70; // 01.01.1970

    tm end_cl = {};
    end_cl.tm_hour = client.end.hour;
    end_cl.tm_min = client.end.min;
    end_cl.tm_mday = 1;
    end_cl.tm_year = 70;

    time_t time1_s = mktime(&start_cl);
    time_t time1_e = mktime(&end_cl);

    if (time1_s >= time1_e) {
        cout << "Start time cannot be lesser or equal to end time. Please, try again" << endl;
        return 1;
    }

    ifstream file(name, ios::binary | ios::app);
    if (!file) {
        cerr << "File opening error " << name << endl;
        return 1;
    }
    Client client2{};
    while (file.read((char *) &client2, sizeof(client2))) {
        tm start_cl2 = {};
        start_cl2.tm_hour = client2.start.hour;
        start_cl2.tm_min = client2.start.min;
        start_cl2.tm_mday = 1;
        start_cl2.tm_year = 70;
        time_t time2_s = mktime(&start_cl2);

        tm end_cl2 = {};
        end_cl2.tm_hour = client2.end.hour;
        end_cl2.tm_min = client2.end.min;
        end_cl2.tm_mday = 1;
        end_cl2.tm_year = 70;
        time_t time2_e = mktime(&end_cl2);

        if (time1_s < time2_s) {
            if (time2_s < time1_e) {
                cout << "Hours cannot be intersecting. Please try again" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return 1;
            }
        } else {
            if (time2_e > time1_s) {
                cout << "Hours cannot be intersecting. Please try again" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return 1;
            }
        }
    }
    file.close();
    return 0;
}

void processFile(string &name, string &out_name) {
    ifstream in_file(name, ios::binary);
    if (!in_file) {
        cerr << "File opening error " << name << endl;
        return;
    }
    ofstream out_file(out_name, ios::binary);
    if (!out_file) {
        cerr << "File opening error " << out_name << endl;
        return;
    }
    Client client{};
    while (in_file.read((char *) &client, sizeof(client))) {
        tm start = {};
        start.tm_hour = client.start.hour;
        start.tm_min = client.start.min;
        start.tm_mday = 1;
        start.tm_year = 70;
        time_t time_s = mktime(&start);

        tm end = {};
        end.tm_hour = client.end.hour;
        end.tm_min = client.end.min;
        end.tm_mday = 1;
        end.tm_year = 70;
        time_t time_e = mktime(&end);

        if (time_e - time_s > 1800) {
            out_file.write((char *) &client, sizeof(client));
        }
    }
    in_file.close();
    out_file.close();
}

int isExist(string &name){
    ifstream file(name, ios::binary);
    if (file) {
        cerr << "File already exists " << name << endl;
        return 1;
    }
    return 0;
}