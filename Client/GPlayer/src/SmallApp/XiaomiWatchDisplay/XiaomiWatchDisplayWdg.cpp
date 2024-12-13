#include "XiaomiWatchDisplayWdg.h"
#include <direct.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include "GtooLogger.h"


XiaomiWatchDisplayWdg::XiaomiWatchDisplayWdg(QWidget* parent)
    : QWidget(parent) {
    ui->setupUi(this);

    parseCSV();
}

XiaomiWatchDisplayWdg::~XiaomiWatchDisplayWdg() {
}

void XiaomiWatchDisplayWdg::parseCSV() {
    char buffer[1024];
    getcwd(buffer, 1024);
    printf("%s", buffer);

    FILE* fd = fopen("sleep_only.csv", "r");
    if (fd == NULL) {
        printf("open fail errno = %d reason = %s \n", errno, strerror(errno));
        return;
    }

    size_t len = 0;
    char line[1024];
    std::vector<std::string> vLine;
    while (fgets(line, sizeof(line), fd)) {
        std::string jsonSub(line);

        if (jsonSub.length() > 3) { // 还需要去掉末尾的\0
            jsonSub = jsonSub.substr(1, jsonSub.length() - 3);
        } else {
            jsonSub.clear();  
        }

        size_t pos = jsonSub.find("\"\"");
        while (pos != std::string::npos) {
            jsonSub.replace(pos, 2, "\"");
            pos = jsonSub.find("\"\"", pos + 1);
        }
        LOG_INFO("replace:\n {}", jsonSub.c_str());
        vLine.push_back(jsonSub);
    }

    fclose(fd);


    ParseSleepData praseData;
    for (int i = 0; i < vLine.size(); i++) {
        SleepDetail tSleepDetail = praseData.parseSleepJson(vLine[i].c_str());
        praseData.vSleepDetail.push_back(tSleepDetail);
    }

    praseData.printSleepJson();
}

void XiaomiWatchDisplayWdg::parseJson(void) {
}

SleepDetail ParseSleepData::parseSleepJson(const char* json_str) {
    SleepDetail sleepDetail;
    cJSON* root = cJSON_Parse(json_str);
    if (root == nullptr) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != nullptr) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return sleepDetail;
    }
    sleepDetail.avg_hr = cJSON_GetObjectItemCaseSensitive(root, "avg_hr")->valueint;
    sleepDetail.avg_spo2 = cJSON_GetObjectItemCaseSensitive(root, "avg_spo2")->valueint;
    sleepDetail.day_sleep_evaluation = cJSON_GetObjectItemCaseSensitive(root, "day_sleep_evaluation")->valueint;

    cJSON* segment_details_array = cJSON_GetObjectItemCaseSensitive(root, "segment_details");
    if (cJSON_IsArray(segment_details_array)) {
        cJSON* segment_detail_item = nullptr;
        cJSON_ArrayForEach(segment_detail_item, segment_details_array) {
            SleepDetail::SegmentDetail detail;
            detail.bedtime = cJSON_GetObjectItemCaseSensitive(segment_detail_item, "bedtime")->valuedouble;
            detail.sleep_deep_duration =
                cJSON_GetObjectItemCaseSensitive(segment_detail_item, "sleep_deep_duration")->valueint;
            detail.duration = cJSON_GetObjectItemCaseSensitive(segment_detail_item, "duration")->valueint;
            detail.sleep_light_duration =
                cJSON_GetObjectItemCaseSensitive(segment_detail_item, "sleep_light_duration")->valueint;
            detail.timezone = cJSON_GetObjectItemCaseSensitive(segment_detail_item, "timezone")->valueint;
            detail.awake_count = cJSON_GetObjectItemCaseSensitive(segment_detail_item, "awake_count")->valueint;
            detail.sleep_awake_duration =
                cJSON_GetObjectItemCaseSensitive(segment_detail_item, "sleep_awake_duration")->valueint;
            detail.wake_up_time = cJSON_GetObjectItemCaseSensitive(segment_detail_item, "wake_up_time")->valuedouble;
            sleepDetail.segment_details.push_back(detail);
        }
    }

    sleepDetail.long_sleep_evaluation = cJSON_GetObjectItemCaseSensitive(root, "long_sleep_evaluation")->valueint;
    sleepDetail.max_hr = cJSON_GetObjectItemCaseSensitive(root, "max_hr")->valueint;
    sleepDetail.min_hr = cJSON_GetObjectItemCaseSensitive(root, "min_hr")->valueint;
    sleepDetail.total_sleep_awake_duration = cJSON_GetObjectItemCaseSensitive(root, "sleep_awake_duration")->valueint;
    sleepDetail.total_sleep_deep_duration = cJSON_GetObjectItemCaseSensitive(root, "sleep_deep_duration")->valueint;
    sleepDetail.total_sleep_light_duration = cJSON_GetObjectItemCaseSensitive(root, "sleep_light_duration")->valueint;
    //total_sleep_rem_duration = cJSON_GetObjectItemCaseSensitive(root, "sleep_rem_duration")->valueint;
    sleepDetail.sleep_score = cJSON_GetObjectItemCaseSensitive(root, "sleep_score")->valueint;
    sleepDetail.sleep_stage = cJSON_GetObjectItemCaseSensitive(root, "sleep_stage")->valueint;
    sleepDetail.total_duration = cJSON_GetObjectItemCaseSensitive(root, "total_duration")->valueint;

    cJSON_Delete(root);
    return sleepDetail;
}

void ParseSleepData::printSleepJson(void) {
    //std::cout << "Average Heart Rate: " << avg_hr << "\n";
    //std::cout << "Average SpO2: " << avg_spo2 << "\n";
    //std::cout << "Day Sleep Evaluation: " << day_sleep_evaluation << "\n";

    //for (const auto& detail : segment_details) {
    //    std::cout << "Bedtime: " << detail.bedtime << "\n";
    //    std::cout << "Sleep Deep Duration: " << detail.sleep_deep_duration << "\n";
    //    std::cout << "Duration: " << detail.duration << "\n";
    //    std::cout << "Sleep Light Duration: " << detail.sleep_light_duration << "\n";
    //    std::cout << "Timezone: " << detail.timezone << "\n";
    //    std::cout << "Awake Count: " << detail.awake_count << "\n";
    //    std::cout << "Sleep Awake Duration: " << detail.sleep_awake_duration << "\n";
    //    std::cout << "Wake Up Time: " << detail.wake_up_time << "\n";
    //}

    //std::cout << "Long Sleep Evaluation: " << long_sleep_evaluation << "\n";
    //std::cout << "Max Heart Rate: " << max_hr << "\n";
    //std::cout << "Min Heart Rate: " << min_hr << "\n";
    //std::cout << "Total Sleep Awake Duration: " << total_sleep_awake_duration << "\n";
    //std::cout << "Total Sleep Deep Duration: " << total_sleep_deep_duration << "\n";
    //std::cout << "Total Sleep Light Duration: " << total_sleep_light_duration << "\n";
    //std::cout << "Total Sleep REM Duration: " << total_sleep_rem_duration << "\n";
    //std::cout << "Sleep Score: " << sleep_score << "\n";
    //std::cout << "Sleep Stage: " << sleep_stage << "\n";
    //std::cout << "Total Duration: " << total_duration << "\n";
}
