/*!
 * \brief Model implementation for the authentication action.
 *
 * \copyright Copyright (c) 2015-2022 Governikus GmbH & Co. KG, Germany
 */

#pragma once

#include "ReaderManagerPlugInInfo.h"
#include "context/WorkflowContext.h"

#include <QObject>
#include <QSharedPointer>
#include <QString>

class test_WorkflowModel;

namespace governikus
{

class WorkflowModel
	: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString currentState READ getCurrentState NOTIFY fireCurrentStateChanged)
	Q_PROPERTY(QString resultString READ getResultString NOTIFY fireResultChanged)
	Q_PROPERTY(bool error READ isError NOTIFY fireResultChanged)
	Q_PROPERTY(bool errorIsMasked READ isMaskedError NOTIFY fireResultChanged)
	Q_PROPERTY(ReaderManagerPlugInType readerPlugInType READ getReaderPlugInType WRITE setReaderPlugInType NOTIFY fireReaderPlugInTypeChanged)
	Q_PROPERTY(bool isSmartSupported READ isSmartSupported NOTIFY fireSupportedPlugInTypesChanged)
	Q_PROPERTY(QVector<ReaderManagerPlugInType> supportedPlugInTypes READ getSupportedReaderPlugInTypes NOTIFY fireSupportedPlugInTypesChanged)
	Q_PROPERTY(bool isBasicReader READ isBasicReader NOTIFY fireSelectedReaderChanged)
	Q_PROPERTY(bool isRemoteReader READ isRemoteReader NOTIFY fireSelectedReaderChanged)
	Q_PROPERTY(bool isSmartCardAllowed READ isSmartCardAllowed NOTIFY fireIsSmartCardAllowedChanged)
	Q_PROPERTY(QString readerImage READ getReaderImage NOTIFY fireReaderImageChanged)
	Q_PROPERTY(bool hasNextWorkflowPending READ getNextWorkflowPending NOTIFY fireNextWorkflowPendingChanged)
	Q_PROPERTY(QString statusHintText READ getStatusHintText NOTIFY fireResultChanged)
	Q_PROPERTY(QString statusHintActionText READ getStatusHintActionText NOTIFY fireResultChanged)
	Q_PROPERTY(bool showRemoveCardFeedback READ showRemoveCardFeedback WRITE setRemoveCardFeedback NOTIFY fireRemoveCardFeedbackChanged)
	friend class ::test_WorkflowModel;

	private:
		QSharedPointer<WorkflowContext> mContext;
		QString mReaderImage;
#if defined(Q_OS_IOS)
		bool mRemoteScanWasRunning;
#endif
		void insertCard(ReaderManagerPlugInType pType);

	public:
		explicit WorkflowModel(QObject* pParent = nullptr);
		~WorkflowModel() override = default;

		void resetWorkflowContext(const QSharedPointer<WorkflowContext>& pContext = QSharedPointer<WorkflowContext>());

		QString getCurrentState() const;
		virtual QString getResultString() const;
		bool isError() const;
		bool isMaskedError() const;

		ReaderManagerPlugInType getReaderPlugInType() const;
		void setReaderPlugInType(ReaderManagerPlugInType pReaderPlugInType);

		bool isBasicReader() const;
		bool isRemoteReader() const;

		bool isSmartCardAllowed() const;

		bool isSmartSupported() const;
		virtual QVector<ReaderManagerPlugInType> getSupportedReaderPlugInTypes() const;

		bool getNextWorkflowPending() const;

		GlobalStatus::Code getStatusCode() const;
		QString getReaderImage() const;

		QString getStatusHintText() const;
		QString getStatusHintActionText() const;
		Q_INVOKABLE bool invokeStatusHintAction();

		[[nodiscard]] bool showRemoveCardFeedback() const;
		void setRemoveCardFeedback(bool pEnabled);

		Q_INVOKABLE void insertSmartCard();
		Q_INVOKABLE void insertSimulator();
		Q_INVOKABLE void cancelWorkflow();
		Q_INVOKABLE void startScanIfNecessary();
		Q_INVOKABLE void continueWorkflow();
		Q_INVOKABLE void setInitialPluginType();
		Q_INVOKABLE bool shouldSkipResultView() const;
		Q_INVOKABLE bool isCancellationByUser() const;
		Q_INVOKABLE QString getEmailHeader() const;
		Q_INVOKABLE QString getEmailBody(bool pPercentEncoding = false, bool pAddLogNotice = false) const;
		Q_INVOKABLE void sendResultMail() const;

	private Q_SLOTS:
		void onApplicationStateChanged(bool pIsAppInForeground);

	public Q_SLOTS:
		void onReaderManagerSignal();

	Q_SIGNALS:
		void fireCurrentStateChanged(const QString& pState);
		void fireResultChanged();
		void fireReaderPlugInTypeChanged();
		void fireSelectedReaderChanged();
		void fireIsSmartCardAllowedChanged();
		void fireReaderImageChanged();
		void fireNextWorkflowPendingChanged();
		void fireSupportedPlugInTypesChanged();
		void fireRemoveCardFeedbackChanged();
};


} // namespace governikus
