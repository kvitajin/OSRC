#include <iostream>
#include <thread>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <iomanip>
#include <pthread.h>
#include <unistd.h>
#include <mutex>

#ifdef __x86_64
    #include <sqlite3.h>
    #include <wiringPi.h>
#endif
#ifdef __arm__
    #include "/usr/include/sqlite3.h"
    #include "/usr/include/wiringPi.h"
    #include "/usr/include/pigpio.h"
#endif


std::mutex locker;
using namespace std;
#define PIN	3
#define SECOND 1000
#define MICRO_SIEVERT 151
#define BUFF_SIZE 20
int pulses=0;

class CircleBuff{
public:
    double data[BUFF_SIZE]={0};
    time_t time[BUFF_SIZE]={0};
    int writePos=0;
};
class DataCell{
public:
    int id;
    time_t date;
    int pulses;
};
std::vector<DataCell> measuredData;

int write2db(int value){
    int err = 1;
    sqlite3* db;
    sqlite3_stmt* stmt;
    stringstream ss;
    auto tim = std::chrono::system_clock::now();
    auto time= std::chrono::system_clock::to_time_t(tim);
//    auto time= std::strftime(&tim2,80, "%T ");
//    time[std::strcspn(time, "\n")] = 0;
    ss<<"INSERT INTO measurements (time, number_of_pulses) VALUES ('"<<time<<"',"<<value<<" );";
    std::string sql(ss.str());
    if(sqlite3_open("../data.db", &db) != SQLITE_OK) {
        printf("ERROR: can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return err;
    }
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return err;
    }
    int ret_code = 0;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("TEST: id = %d\n", sqlite3_column_int(stmt, 0));
        err = +1;
    }
    if(ret_code != SQLITE_DONE) {
        //this error handling could be done better, but it works
        printf("ERROR: while performing sql: %s\n", sqlite3_errmsg(db));
        printf("ret_code = %d\n", ret_code);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int readLastN(int N){
    int err = 1;
    sqlite3* db;
    sqlite3_stmt* stmt;
    stringstream ss;

    ss<<"SELECT * FROM measurements order by id desc limit "<<N<<";";
    std::string sql(ss.str());
    if(sqlite3_open("../data.db", &db) != SQLITE_OK) {
        printf("ERROR: can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return err;
    }
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return err;
    }
    int ret_code = 0;
    DataCell tmp{};
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        tmp.id      = sqlite3_column_int(stmt, 0);
        tmp.date    = sqlite3_column_int(stmt, 1);;
        tmp.pulses  = sqlite3_column_int(stmt, 2);
        measuredData.push_back(tmp);
        std::cout<<"id: "<< sqlite3_column_int(stmt, 0)<<" time: "<< sqlite3_column_text(stmt, 1)<<" pulses: "<<sqlite3_column_int(stmt, 2)<<std::endl;
//        printf("TEST: id = %d\n", sqlite3_column_int(stmt, 0));
    }
    if(ret_code != SQLITE_DONE) {
        //this error handling could be done better, but it works
        printf("ERROR: while performing sql: %s\n", sqlite3_errmsg(db));
        printf("ret_code = %d\n", ret_code);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

void FloatingAVG(int N, CircleBuff &buff){
    if (!N){
        return;
    }
    double tmp=0;
    for (unsigned long i = measuredData.size()-N; i < measuredData.size(); ++i) {
        tmp+=measuredData[i].pulses;
    }

    buff.data[buff.writePos]=tmp/N;
    buff.time[buff.writePos]=measuredData[measuredData.size()].date;
    buff.writePos = ++buff.writePos%BUFF_SIZE;
}


void DBInterrupt(int &pulses){
    auto next = std::chrono::system_clock::now()+1s;
    while (true){
        std::this_thread::sleep_until(next);
        next+=1s;
        write2db(pulses);
        locker.lock();
        pulses=0;
        locker.unlock();
    }
}

void PulsesInterrupt(int &pulses){
    while (true){
        if(digitalRead(PIN) == HIGH){
            locker.lock();
            pulses += 1;
            locker.unlock();
        }
    }
}
void IOInterrupt(){
    if(digitalRead(PIN) == HIGH){
        locker.lock();
        pulses += 1;
        locker.unlock();
    }
}
int main() {

    wiringPiSetup();
    wiringPiISR(PIN, INT_EDGE_RISING, IOInterrupt);

//    int pulses = 0;

//    std::thread measurement = std::thread(PulsesInterrupt,std::ref(pulses));
    std::thread dbWrite = std::thread(DBInterrupt,std::ref(pulses));

    return EXIT_SUCCESS;

}
