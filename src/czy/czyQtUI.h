#ifndef CZYQTUI_H
#define CZYQTUI_H

#include <QStandardItemModel>
#include <functional>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

#include <limits>
namespace czy{
	namespace QtUI
	{

        ///
        /// \brief QStandardItemModel 的扩展
        ///
		class StandardItemModel
		{
		public:
			StandardItemModel(){}
			~StandardItemModel(){}
            ///
            /// \brief callback_ergodicFun_ptr 回调函数指针，bool f(QStandardItem*),bool用于决定是否继续，如果为true就继续递归，如果为false就停止递归
            ///
            typedef std::function<bool(QStandardItem*)> callback_ergodicFun_ptr;

            //typedef void(*callback_ergodicFun_ptr)(QStandardItem*);
            static void ergodicAllItem(const QStandardItemModel* model,callback_ergodicFun_ptr pFun);

            static bool ergodicItem(QStandardItem* item,callback_ergodicFun_ptr pFun);
		};

		class MainWindowEx : public QMainWindow{
			Q_OBJECT
		public:
			explicit MainWindowEx(QWidget *parent = 0):QMainWindow(parent){}
			~MainWindowEx(){}
            void setHidden();

            bool eventFilter(QObject *object, QEvent *e);
            void closeEvent( QCloseEvent *e);
		signals:
			//! Emitted when the window was closed

			void closedWindow(MainWindowEx* wnd);
            //! 子窗口隐藏信号
            void hiddenWindow(MainWindowEx *);
            //! 显示自定义菜单
            void showContextMenu();
		};

        class QWidgetEx : public QWidget{
            Q_OBJECT
        public:
            explicit QWidgetEx(QWidget *parent = 0):QWidget(parent){}
            ~QWidgetEx(){}
            void setHidden();

            bool eventFilter(QObject *object, QEvent *e);
            void closeEvent( QCloseEvent *e);
        signals:
            //! Emitted when the window was closed

            void closedWindow(QWidget* wnd);
            //! 子窗口隐藏信号
            void hiddenWindow(QWidget *);
            //! 显示自定义菜单
            void showContextMenu();
        };
	}
}


#endif // CZYQTUI_H
