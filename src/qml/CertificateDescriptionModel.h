/*!
 * \brief Model implementation for the CV certificate description.
 *
 * \copyright Copyright (c) 2016 Governikus GmbH & Co. KG
 */

#pragma once

#include <QAbstractListModel>
#include <QPair>
#include <QSharedPointer>
#include <QString>
#include <QVector>

namespace governikus
{

class AuthContext;
struct CertificateDescription;
struct cvcertificate_st;


class CertificateDescriptionModel
	: public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString subjectName READ getSubjectName NOTIFY fireChanged)
	Q_PROPERTY(QString purpose READ getPurpose NOTIFY fireChanged)

	QVector<QPair<QString, QString> > mData;
	QSharedPointer<AuthContext> mContext;

	enum UserRoles
	{
		LABEL = Qt::UserRole + 1,
		TEXT
	};


	inline QSharedPointer<CertificateDescription> getCertificateDescription() const;
	inline QString getValidity() const;

	private Q_SLOTS:
		void onDidAuthenticateEac1Changed();

	public:
		CertificateDescriptionModel(QObject* pParent = nullptr);

		void resetContext(QSharedPointer<AuthContext> pContext = QSharedPointer<AuthContext>());

		QString getSubjectName() const;
		QString getSubjectUrl() const;
		QString getPurpose() const;

		int rowCount(const QModelIndex& = QModelIndex()) const override;
		QVariant data(const QModelIndex& pIndex, int pRole = Qt::DisplayRole) const override;
		QHash<int, QByteArray> roleNames() const override;

	Q_SIGNALS:
		void fireChanged();
};


} /* namespace governikus */