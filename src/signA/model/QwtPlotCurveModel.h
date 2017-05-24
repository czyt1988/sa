#ifndef MODELQWTCURVEDATA_H
#define MODELQWTCURVEDATA_H
#include <QAbstractTableModel>
#include <QList>
#include <algorithm>
#include <vector>
#include <qwt_plot_curve.h>
class QwtPlotCurveModel : public QAbstractTableModel
{
public:
	QwtPlotCurveModel(QObject *parent = 0):QAbstractTableModel(parent){
		m_show_mode = SHOW_XY_TOGETHER;
	}
	void addCurve(QwtPlotCurve* cur){
		beginResetModel();
		m_curves.append(cur);
		m_max_row = getMaxRow();
		endResetModel();
	}
	void addCurves(const QList<QwtPlotCurve*>& cur){
		beginResetModel();
		m_curves.append(cur);
		m_max_row = getMaxRow();
		endResetModel();
	}
	void refreshAll()
	{
		beginResetModel();endResetModel();
	}
    int rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
		return m_max_row;
	}
	int getMaxRow(){
		if (m_curves.size()<=0)
			return 0;
		std::vector<int> v;
		v.reserve(m_curves.size());
		for (auto iter=m_curves.begin();iter!=m_curves.end();++iter)
		{
			v.push_back((*iter)->data()->size());
		}
		return *(std::max_element(v.begin(),v.end()));
	}
	int columnCount(const QModelIndex &parent) const
	{
        Q_UNUSED(parent);
		if (SHOW_XY_SEPARATE == m_show_mode )
		{
			return 2*m_curves.size();
		}
		return m_curves.size();
	}
	QVariant data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role == Qt::TextAlignmentRole){ //返回的是对其方式
			return int(Qt::AlignRight | Qt::AlignVCenter);
		}
		else if (role == Qt::DisplayRole){ //返回的是现实内容
			if(index.row() > m_max_row)
				return QVariant();
			if (SHOW_XY_TOGETHER == m_show_mode)
			{
				if (index.column() < m_curves.size())
				{
					QwtPlotCurve* cur = m_curves[index.column()];
					if(index.row() < cur->data()->size())
						return QString("(%1,%2)")
						.arg(cur->data()->sample(index.row()).x())
						.arg(cur->data()->sample(index.row()).y());//显示点
				}
			}
			else if (SHOW_XY_SEPARATE == m_show_mode)
			{
				QwtPlotCurve* cur = m_curves[index.column()/2];
				if(index.row() < cur->data()->size())
					return index.column()%2 == 0
					? cur->data()->sample(index.row()).x()
					: cur->data()->sample(index.row()).y();//显示点
			}
			else if (SHOW_XY_IN_REAL_MODE == m_show_mode)
			{

			}
		}
		return QVariant();
	}
	///
	/// \brief 返回表头名称,(行号或列号，水平或垂直，角色)
	/// \param section
	/// \param orientation
	/// \param role
	/// \return
	///
	QVariant headerData(int section, Qt::Orientation orientation,
		int role) const
	{
		if (role != Qt::DisplayRole)
			return QVariant();
		if (SHOW_XY_IN_REAL_MODE == m_show_mode)
		{
			
		}
		else
		{
			if(Qt::Horizontal == orientation){//说明是水平表头
				if (SHOW_XY_TOGETHER == m_show_mode)
				{
					if (section < m_curves.size())
					{
						return m_curves[section]->title().text();
					}
				}
				else//SHOW_XY_SEPARATE
				{
					if (section < (2*m_curves.size()))
					{
						return QString("%1 %2")
							.arg(m_curves[section/2]->title().text())
								.arg(section%2 == 0 ? "x" : "y");

					}
				}
			}
			else if(Qt::Vertical == orientation){//垂直表头
				return section+1;
			}
		}
		return QVariant();
	}
	enum SHOW_MODE{
		SHOW_XY_TOGETHER,///<xy显示在一个内容里，如（1,2）
		SHOW_XY_SEPARATE,///< xy分开显示，如曲线1，将会有两列，1x值，1y值
		SHOW_XY_IN_REAL_MODE///< 按实际情况显示，这时垂直表头将会是x值，如果对应曲线没有这个x值就会空着，这种显示将会很慢
	};
	void setShowMode(SHOW_MODE mode){
		m_show_mode = mode;
	}

private:
	QList<QwtPlotCurve*> m_curves;
	int m_max_row;
	int m_show_mode;///< 显示模式
	//bool m_show_bk_color;
};

#endif // MODELQWTCURVEDATA_H
