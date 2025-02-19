/*!
 * \copyright Copyright (c) 2014-2022 Governikus GmbH & Co. KG, Germany
 */

#include "StateChangeSmartPin.h"

#include "AppSettings.h"
#include "ReaderManager.h"

using namespace governikus;

StateChangeSmartPin::StateChangeSmartPin(const QSharedPointer<WorkflowContext>& pContext)
	: AbstractState(pContext)
	, GenericContextContainer(pContext)
{
}


void StateChangeSmartPin::run()
{
	const auto& context = getContext();
	if (context->getNewPin().isEmpty())
	{
		mConnections += connect(context.data(), &PersonalizationContext::fireNewPinChanged, this, &StateChangeSmartPin::callSetEidPin);
		return;
	}

	callSetEidPin();
}


void StateChangeSmartPin::callSetEidPin()
{
	auto cardConnection = getContext()->getCardConnection();

	Q_ASSERT(cardConnection);
	qDebug() << "Invoke set Eid PIN command";
	mConnections += cardConnection->callSetEidPinCommand(this, &StateChangeSmartPin::onSetEidPinDone, getContext()->getNewPin().toLatin1());
}


void StateChangeSmartPin::onSetEidPinDone(QSharedPointer<BaseCardCommand> pCommand)
{
	const CardReturnCode returnCode = pCommand->getReturnCode();
	getContext()->setLastPaceResult(returnCode);
	switch (returnCode)
	{
		case CardReturnCode::OK:
		{
			const auto& context = getContext();
			context->setProgress(100, context->getProgressMessage());

			GeneralSettings& settings = Env::getSingleton<AppSettings>()->getGeneralSettings();
			settings.setLastReaderPluginType(getEnumName(ReaderManagerPlugInType::SMART));

			Q_EMIT fireContinue();
			break;
		}

		default:
			updateStatus(CardReturnCodeUtil::toGlobalStatus(returnCode));
			Q_EMIT fireAbort();
	}
}
