#pragma once

#include <QWidget>
#include "ui_XiaomiWatchDisplayWdg.h"
extern "C" {
#include "cJSON.h"
}

class SleepDetail {
public:
    int avg_hr;
    int avg_spo2;
    int day_sleep_evaluation;
    struct SegmentDetail {
        long bedtime;
        int sleep_deep_duration;
        int duration;
        int sleep_light_duration;
        int timezone;
        int awake_count;
        int sleep_awake_duration;
        long wake_up_time;
    };
    std::vector<SegmentDetail> segment_details;
    int long_sleep_evaluation;
    int max_hr;
    int min_hr;
    int total_sleep_awake_duration;
    int total_sleep_deep_duration;
    int total_sleep_light_duration;
    int total_sleep_rem_duration;
    int sleep_score;
    int sleep_stage;
    int total_duration;
};

class ParseSleepData {
public:
    SleepDetail parseSleepJson(const char* json_str);
    void printSleepJson(void);

    std::vector<SleepDetail> vSleepDetail;
};

class XiaomiWatchDisplayWdg : public QWidget {
    Q_OBJECT

public:
    XiaomiWatchDisplayWdg(QWidget* parent = nullptr);
    ~XiaomiWatchDisplayWdg();

    void parseCSV(void);
    void parseJson(void);

private:
    Ui::XiaomiWatchDisplayWdg* ui;
};
