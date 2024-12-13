
```json
{
    "avg_hr": 69, // 平均心率（次/分钟）
    "avg_spo2": 0, // 平均血氧饱和度（百分比），这里为0可能是数据缺失或未测量
    "day_sleep_evaluation": 0, // 日间小睡评价分数，0可能表示没有日间小睡或数据缺失
    "segment_details": [ // 睡眠片段详细信息列表
        {
            "avg_hr": 69, // 该睡眠片段的平均心率（次/分钟）
            "bedtime": 1730999820, // 入睡时间的时间戳（秒），可以通过转换得知具体日期和时间
            "sleep_deep_duration": 129, // 深睡时长（分钟）
            "duration": 381, // 总睡眠时长（分钟）
            "sleep_light_duration": 171, // 浅睡时长（分钟）
            "max_hr": 91, // 最高心率（次/分钟）
            "min_hr": 53, // 最低心率（次/分钟）
            "sleep_rem_duration": 81, // 快速眼动（REM）睡眠时长（分钟）
            "timezone": 32, // 时区偏移量（分钟），正值表示UTC+，负值表示UTC-
            "awake_count": 1, // 醒来次数
            "sleep_awake_duration": 32, // 清醒时长（分钟），即在应该睡觉的时候清醒的时间
            "wake_up_time": 1731024600 // 起床时间的时间戳（秒），同上可以通过转换得知具体日期和时间
        }
    ],
    "long_sleep_evaluation": 7, // 长时间睡眠评价分数，通常反映整体睡眠质量
    "max_hr": 91, // 整个记录期间的最大心率（次/分钟）
    "min_hr": 53, // 整个记录期间的最小心率（次/分钟）
    "sleep_awake_duration": 32, // 整个记录期间的清醒时长（分钟）
    "sleep_deep_duration": 129, // 整个记录期间的深睡时长（分钟）
    "sleep_light_duration": 171, // 整个记录期间的浅睡时长（分钟）
    "sleep_rem_duration": 81, // 整个记录期间的快速眼动（REM）睡眠时长（分钟）
    "sleep_score": 71, // 整体睡眠评分，用来综合评估睡眠质量
    "sleep_stage": 3, // 睡眠阶段，不同系统定义不同，3可能指特定的一种睡眠状态
    "total_duration": 381 // 总睡眠时长（分钟），包括所有类型的睡眠阶段
},
```