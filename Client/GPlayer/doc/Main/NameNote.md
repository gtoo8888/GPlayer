# 一、名称规范

```cpp
// 变量名
// 小驼峰命名法：首字母小写，后续单词首字母大写
int studentCount = 1;
int userName = 1;

// 常量
// 全大写字母，单词之间用下划线分隔
const int MAX_VALUE = 100;
const int DEFAULT_TIMEOUT = 30;

// 函数
// 小驼峰命名法：首字母小写，后续单词首字母大写
void setMimeTypeFilters(const QStringList &filters);
void calculateTotal(int a, int b);

// 类型名（类、结构体、枚举等）
// 大驼峰命名法：每个单词首字母大写。
class MyExample {};
struct ConfigurationItem {};
enum class Color { RedColor, GreenColor };
enum class Direction { NorthDirection, SouthDirection };

// 命名空间
// 大驼峰命名法：每个单词首字母大写。
namespace GraphicsEngine { };

// 宏定义
// 全大写字母，单词之间用下划线分隔。
#define ENABLE_DEBUG
#define MAX_CONNECTIONS 5

```

# 二、代码规范
```cpp
// 1. 以小写字母m（member的首字母）开头的变量表示类的成员变量
int mSensor;
int mTrackingState;
std::mutex mMutexMode;

// 2. mp开头的变量表示指针（pointer）型类成员变量
Tracking* mpTracker;
LocalMapping* mpLocalMapper;
LoopClosing* mpLoopCloser;
Viewer* mpViewer;

// 3. mb开头的变量表示布尔（bool）型类成员变量
bool mbOnlyTracking;

// 4. mv开头的变量表示向量（vector）型类成员变量
std::vector<int> mvIniLastMatches;
std::vector<cv::Point3f> mvIniP3D;

// 5. mpt开头的变量表示指针（pointer）型类成员变量，并且它是一个线程（thread）；
std::thread* mptLocalMapping;
std::thread* mptLoopClosing;
std::thread* mptViewer;

// 6. ml开头的变量表示列表（list）型类成员变量
// mlp开头的变量表示列表（list）型类成员变量，并且它的元素类型是指针（pointer）；
// mlb开头的变量表示列表（list）型类成员变量，并且它的元素类型是布尔型（bool）；
list<double> mlFrameTimes;
list<bool> mlbLost;
list<cv::Mat> mlRelativeFramePoses;
list<KeyFrame*> mlpReferences;


// 7. ms开头的变量表示列表（string）型类成员变量
std::string msTest;

```

# Qt相关
```c++
// 1. 信号与槽的命名方式
// 信号函数
// 小驼峰
signal:
void sigSendMessage(void){}

// 槽函数
// 小驼峰
public slots:
void slotForwardPlay (void){}


// Qt控件命名方法
QPushButton btnTest;
QToolButton tbnTest;
QLineEdit leTest;
QTabWidget twTest;
QToolBox tbTest;
QSlider sldTest;


QSpinBox spbTest;
QToolButton tbnTest;
QComboBox cmbTest;
QRadioButton rdoTest;
QGroupBox gbTest;
QStackedWidget stkTest;
QWidget wdTest;
QFrame frmTest;
QDialog dlgTest;
QScrollBar scbTest;
QTextBrowser txbTest;
QLayout lyTest;
QProcessBar prbTest;
QListView lsvTest;
QListWidget lswTest;
QTableView tbvTest;
QTableWidget tbwTest;
QTreeView tvTest;
QTreeWidget twTest;
```

7. 变量名最好是缩写，每个单词最好不超过5个
sendMsgMultiple

sendMsgMulti

# 参考资料
[Qt中的控件缩写](https://blog.csdn.net/qq_37996632/article/details/108438231)
