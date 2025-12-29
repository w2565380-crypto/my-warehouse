#ifndef MODESELECTIONDIALOG_H
#define MODESELECTIONDIALOG_H

#include <QDialog>

// 定义场景模式枚举
enum SceneMode { HomeMode, AwayMode, SleepMode };

namespace Ui { class ModeSelectionDialog; }

class ModeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModeSelectionDialog(QWidget *parent = nullptr);
    ~ModeSelectionDialog();

    // 增加一个接口，让 MainWindow 能获取选择的模式
    SceneMode getSelectedMode() const { return m_selectedMode; }

    // 增加一个判断，是否点击了日志按钮
    bool isLogRequested() const { return m_showLog; }
    bool isBackRequested() const { return m_backToLogin; }

private slots:
    // 四个按钮的点击槽函数
    void on_homeBtn_clicked();
    void on_awayBtn_clicked();
    void on_sleepBtn_clicked();
    void on_logBtn_clicked();
    void on_backBtn_clicked();

private:
    Ui::ModeSelectionDialog *ui;
    SceneMode m_selectedMode;
    bool m_showLog = false; // 是否查看日志的标志位
    bool m_backToLogin = false;
};

#endif
