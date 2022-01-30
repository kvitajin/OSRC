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
#define PIN	8           //wiringpi 8 a bcm 3
#define SECOND 1000
#define MICRO_SIEVERT 151
#define BUFF_SIZE 100
int pulses=0;

class CircleBuff{
public:
    double data[BUFF_SIZE]={0};
    std::string time[BUFF_SIZE];
    int writePos=0;
};
class DataCell{
public:
    int id;
    std::string date;
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
std::string getReadableTime(time_t time){
    std::string s(17, '\0');
    std::strftime(&s[0], s.size(), "%H:%M %d.%m.%Y ", std::localtime(&time));
    return s;
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
    time_t timeTmp;
    measuredData.clear();
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        tmp.id      = sqlite3_column_int(stmt, 0);
        tmp.date    = getReadableTime(sqlite3_column_int(stmt, 1));
        tmp.pulses  = sqlite3_column_int(stmt, 2);
        measuredData.push_back(tmp);
    }
//    for (auto & i : measuredData) {
//        std::cout<<"id: "<< i.id<<" time: "<< i.date<<" pulses: "<<i.pulses<<std::endl;
//    }
    if(ret_code != SQLITE_DONE) {
        //this error handling could be done better, but it works
        printf("ERROR: while performing sql: %s\n", sqlite3_errmsg(db));
        printf("ret_code = %d\n", ret_code);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

void floatingAVG(int N, CircleBuff &buff){
    if (!N){
        return;
    }
    double tmp=0;
    for (unsigned long i = measuredData.size()-N; i < measuredData.size(); ++i) {
        tmp+=measuredData[i].pulses;
    }
    buff.data[buff.writePos]=tmp/N;
    buff.time[buff.writePos]=measuredData[measuredData.size()-1].date;
    buff.writePos = ++buff.writePos%BUFF_SIZE;
}


void DBInterrupt(){
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

void PulsesInterrupt(){
    while (true){
        waitForInterrupt(PIN, -1);
        locker.lock();
        pulses += 1;
        locker.unlock();
        std::cout<<"aaaa"<<std::endl;
    }
}
void IOInterrupt(){
    if(digitalRead(PIN) == HIGH){
        locker.lock();
        pulses += 1;
        locker.unlock();
    }
}

void dataPrint(bool valueXavg, int numberOfMinutes){
    readLastN(60*numberOfMinutes);
    if (valueXavg){
        for (int i = 0; i < numberOfMinutes; ++i) {
            double minSum=0;
            for (int j = 0; j < 60; ++j) {
                minSum+= measuredData[(i*60)+j].pulses;
            }
            std::cout << "Date: "<< measuredData[60*i].date << " Value: " << minSum/MICRO_SIEVERT<<" μSv"<<std::endl;
        }
    }
}
int main() {
    wiringPiSetup();
    wiringPiISR(PIN, INT_EDGE_RISING, IOInterrupt);

//    floatingAVG(10, buff);
    std::thread measurement = std::thread(PulsesInterrupt);
    std::thread dbWrite = std::thread(DBInterrupt);
    readLastN(100);
    dataPrint(true, 5);
    while (1){
        delayMicroseconds(60*SECOND);

    }


    return EXIT_SUCCESS;

}
