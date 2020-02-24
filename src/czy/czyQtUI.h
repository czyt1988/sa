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
namespace SA{
	namespace QtUI
	{

        ///
        /// \brief QStandardItemModel 的扩展
        ///
        class SAStandardItemModel
		{
		public:
            SAStandardItemModel(){}
            ~SAStandardItemModel(){}
            ///
            /// \brief callback_ergodicFun_ptr 回调函数指针，bool f(QStandardItem*),bool用于决定是否继续，如果为true就继续递归，如果为false就停止递归
            ///
            typedef std::function<bool(QStandardItem*)> callback_ergodicFun_ptr;

            //typedef void(*callback_ergodicFun_ptr)(QStandardItem*);
            static void ergodicAllItem(const QStandardItemModel* model,callback_ergodicFun_ptr pFun);

            static bool ergodicItem(QStandardItem* item,callback_ergodicFun_ptr pFun);
		};
	}
}


#endif // CZYQTUI_H
