/*
 * \copyright Copyright (c) 2019-2022 Governikus GmbH & Co. KG, Germany
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15

import Governikus.Style 1.0
import Governikus.Type.ApplicationModel 1.0

RowLayout {
	id: root

	property string sectionName
	property string headerText
	property string footerText
	property string iconPath

	width: parent.width

	spacing: Constants.groupbox_spacing

	Image {
		id: sectionIcon

		visible: source !== ""
		sourceSize.height: Style.dimens.medium_icon_size
		sourceSize.width: Style.dimens.medium_icon_size

		source: iconPath
		fillMode: Image.PreserveAspectFit
	}

	Column {
		Layout.fillWidth: true

		spacing: Constants.text_spacing

		GText {
			width: parent.width

			text: headerText
			textStyle: isCurrentItem ? Style.text.normal : Style.text.normal_inverse
			maximumLineCount: 1
		}

		GText {
			width: parent.width

			text: sectionName
			textStyle: isCurrentItem ? Style.text.header : Style.text.header_inverse
			maximumLineCount: 1
			elide: Text.ElideRight
		}

		GText {
			width: parent.width

			text: footerText
			textStyle: isCurrentItem ? Style.text.normal : Style.text.normal_inverse
			maximumLineCount: 1
			elide: Text.ElideRight
		}
	}
}
