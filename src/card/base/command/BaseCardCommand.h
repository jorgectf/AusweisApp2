/*!
 * \brief Holds some basic card control commands
 *
 * \copyright Copyright (c) 2014-2022 Governikus GmbH & Co. KG, Germany
 */

#pragma once

#include "CardConnectionWorker.h"
#include "CardReturnCode.h"

#include <QSharedPointer>

class test_CardConnection;

namespace governikus
{

class BaseCardCommand
	: public QObject
{
	Q_OBJECT
	friend class ::test_CardConnection;

	private:
		Q_INVOKABLE void execute();
		QSharedPointer<CardConnectionWorker> mCardConnectionWorker;

	protected:
		CardReturnCode mReturnCode;
		explicit BaseCardCommand(QSharedPointer<CardConnectionWorker> pCardConnectionWorker);

		[[nodiscard]] QSharedPointer<CardConnectionWorker> getCardConnectionWorker()
		{
			return mCardConnectionWorker;
		}


		virtual void internalExecute() = 0;
		~BaseCardCommand() override = default;

	public:
		void run();

		[[nodiscard]] CardReturnCode getReturnCode() const
		{
			return mReturnCode;
		}

	Q_SIGNALS:
		void commandDone(QSharedPointer<BaseCardCommand> pCommand);
};

} // namespace governikus
