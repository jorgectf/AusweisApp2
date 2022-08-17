/*!
 * \copyright Copyright (c) 2021-2022 Governikus GmbH & Co. KG, Germany
 */

#include "SimulatorFileSystem.h"

#include "FileRef.h"
#include "apdu/CommandApdu.h"
#include "apdu/ResponseApdu.h"
#include "asn1/ASN1TemplateUtil.h"
#include "asn1/ASN1Util.h"

#include <QFile>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QString>
#include <openssl/conf.h>


using namespace governikus;


Q_DECLARE_LOGGING_CATEGORY(card)


SimulatorFileSystem::SimulatorFileSystem()
	: mSelectedFile()
	, mFiles()
	, mFileIds()
{
	createFile(FileRef::efDir().getIdentifier(), FileRef::efDir().getShortIdentifier(), QByteArray::fromHex(
			"61324F0FE828BD080FA000000167455349474E500F434941207A752044462E655369676E5100730C4F0AA00000016745"
			"5349474E61094F07A0000002471001610B4F09E80704007F00070302610C4F0AA000000167455349474E"));
	createFile(FileRef::efCardAccess().getShortIdentifier(), QByteArray::fromHex(
			"3181C13012060A04007F0007020204020202010202010D300D060804007F00070202020201023012060A04007F000702"
			"02030202020102020129301C060904007F000702020302300C060704007F0007010202010D020129303E060804007F00"
			"0702020831323012060A04007F0007020203020202010202012D301C060904007F000702020302300C060704007F0007"
			"010202010D02012D302A060804007F0007020206161E687474703A2F2F6273692E62756E642E64652F6369662F6E7061"
			"2E786D6C"));
	createFile(FileRef::efCardSecurity().getShortIdentifier(), QByteArray::fromHex(
			"308209F606092A864886F70D010702A08209E7308209E3020103310F300D06096086480165030402030500308203E006"
			"0804007F0007030201A08203D2048203CE318203CA3012060A04007F0007020204020202010202010D300D060804007F"
			"00070202020201023017060A04007F0007020205020330090201010201010101003021060904007F0007020205023014"
			"06072A8648CE3D020106092B24030302080101073017060A04007F0007020205020330090201010201020101FF301206"
			"0A04007F00070202030202020102020129301C060904007F000702020302300C060704007F0007010202010D02012930"
			"62060904007F0007020201023052300C060704007F0007010202010D0342000419D4B7447788B0E1993DB35500999627"
			"E739A4E5E35F02D8FB07D6122E76567F17758D7A3AA6943EF23E5E2909B3E8B31BFAA4544C2CBF1FB487F31FF239C8F8"
			"0201293081A3060804007F00070202083181963012060A04007F0007020203020202010202012D301C060904007F0007"
			"02020302300C060704007F0007010202010D02012D3062060904007F0007020201023052300C060704007F0007010202"
			"010D034200041AC6CAE884A6C2B8461404150F54CD1150B21E862A4E5F21CE34290C741104BD1BF31ED91E085D7C630E"
			"8B4D10A8AE22BBB2898B44B52EA0F4CDADCF57CFBA2502012D302A060804007F0007020206161E687474703A2F2F6273"
			"692E62756E642E64652F6369662F6E70612E786D6C308201E6060804007F0007020207308201D8300B06096086480165"
			"03040204308201C73021020101041C2FF0247F59DD3C646E314F03ABB33EE91A586577EBDF48D3864EC34D3021020102"
			"041C37823963B71AF0BF5698D1FDC30DA2B7F9ECE57CFA4959BEE9D6D9943021020103041CE8B2A171DC1290A765F124"
			"AAFE33061C08C918A1069DFF5CAF4C62B53021020104041CAD81D20DBD4F5687FDB05E5037EC267609FDE28C6036FDBD"
			"F2C8B4333021020105041CA90F28EB7A0FA0DE83ABF3293D14E0838B9C85FC7277CBB97737A32B3021020106041C712B"
			"8550E49A13C64DCED4457E9A0F5A85DC26CD6A321596723005D63021020107041C42A8FA36B60887ED022CD3B6ECC255"
			"220FBE8CB3F607E416601FCAA63021020108041C6446E0A909967462B5C1117634F8A1B557EF74BE3F606C1E94EFAE43"
			"3021020109041C635D1017F4ABC656B9FDDDD7E0FBB1E992B7686E89485E6AB51B638B302102010D041C04DB93544A64"
			"BC1245B10AAB266386F08F8E89F72E1DB178C172624D3021020111041CAADEE20557D41AB9969E962282CAF259044751"
			"48D329D2F6B2F43E343021020112041C57CE396CA707B96FA37C580F693230E4D4AEBB97293F0909489D95CB30210201"
			"0A041C1880A259CDB497C15A7FDD1C9AC9490D7DC0D18743378603D43D1D4FA082049F3082049B308203FEA003020102"
			"020204D5300A06082A8648CE3D0403043046310B3009060355040613024445310D300B060355040A0C0462756E64310C"
			"300A060355040B0C03627369311A301806035504030C115445535420637363612D6765726D616E79301E170D32303031"
			"32313036333630345A170D3330303832313233353935395A305C310B3009060355040613024445310C300A060355040A"
			"0C03425349310D300B06035504051304303039393130302E06035504030C275445535420446F63756D656E7420536967"
			"6E6572204964656E7469747920446F63756D656E7473308201B53082014D06072A8648CE3D020130820140020101303C"
			"06072A8648CE3D01010231008CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B412B1DA197FB71123ACD3A729"
			"901D1A71874700133107EC53306404307BC382C63D8C150C3C72080ACE05AFA0C2BEA28E4FB22787139165EFBA91F90F"
			"8AA5814A503AD4EB04A8C7DD22CE2826043004A8C7DD22CE28268B39B55416F0447C2FB77DE107DCD2A62E880EA53EEB"
			"62D57CB4390295DBC9943AB78696FA504C110461041D1C64F068CF45FFA2A63A81B7C13F6B8847A3E77EF14FE3DB7FCA"
			"FE0CBD10E8E826E03436D646AAEF87B2E247D4AF1E8ABE1D7520F9C2A45CB1EB8E95CFD55262B70B29FEEC5864E19C05"
			"4FF99129280E4646217791811142820341263C53150231008CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B3"
			"1F166E6CAC0425A7CF3AB6AF6B7FC3103B883202E90465650201010362000401B434B9555974F51934687C520DAE3380"
			"32F5046999E1595D85B89A4CBDB90888B8DCAB2D6588CF73E8E43DB78AB40A0FDB710D971F1C0205B9243E1F769A9E06"
			"81C01D1B298C4D7DE7F3F7E6CE9F16657907B79328BEC8166F5FC035E26EE3A382016630820162301F0603551D230418"
			"30168014539DB1872AAC9193D76392EE80D9E5996CF99B3B301D0603551D0E0416041472571E58FC52EAD9641412875C"
			"615E8090508CFA300E0603551D0F0101FF040403020780302B0603551D1004243022800F323032303031323130363336"
			"30345A810F32303230303832313233353935395A30160603551D20040F300D300B060904007F00070301010130260603"
			"551D11041F301D820B6273692E62756E642E6465A40E300C310A300806035504070C014430510603551D12044A304881"
			"18637363612D6765726D616E79406273692E62756E642E6465861C68747470733A2F2F7777772E6273692E62756E642E"
			"64652F63736361A40E300C310A300806035504070C01443019060767810801010602040E300C02010031071301411302"
			"494430350603551D1F042E302C302AA028A0268624687474703A2F2F7777772E6273692E62756E642E64652F74657374"
			"5F637363615F63726C300A06082A8648CE3D04030403818A00308186024100A348C5E7948535C9ECB5043D62FA1F56F1"
			"6886AF76C434C870D988D345175FD51E60A89C0E9D06A94D35078853397D7C8403E32053DF6BDFC16CC1B3A5E7D1CB02"
			"41008506DC6ACA4F202B4BDF7957263010886D38D4991D101374F6A7B8F4BC1CE51CB278E9F8851951F6AF0ABA7D4773"
			"F42762FD8F840A01F2D526CC80682DCA08103182014430820140020101304C3046310B3009060355040613024445310D"
			"300B060355040A0C0462756E64310C300A060355040B0C03627369311A301806035504030C115445535420637363612D"
			"6765726D616E79020204D5300D06096086480165030402030500A06A301706092A864886F70D010903310A060804007F"
			"0007030201304F06092A864886F70D0109043142044066927654D73A84CCCD931E2C44A9B34EF3B848EE85B7F4A92699"
			"EA7BF5262FE73B101F31F580180C96EA642569E5E6DB8469A4C7E4CB47DFE9C5D95B0939125E300A06082A8648CE3D04"
			"0304046630640230582364C74D9C694D3C8F99ACBF82A7A847141248B015AED8BEE3C395E82788426F032978D196303A"
			"6B81D9FA8B8DBC8E02305BF169DE97B344A4B03E862C48A76226F044C6DA1EA78E380C2C6479B79526415735345764D7"
			"B6E738EE83931AABE840"));

	// Default profile 01        : 01 - 0A, 0D, 11 - 12
	// Additional data fields    : 0B - 0C, 0E - 0F, 13 - 16
	// Reserved for future usage : 10

	createFile(QByteArray::fromHex("01"), "FORMAT:UTF8,EXPLICIT:1A,PRINTABLESTRING:ID");
	createFile(QByteArray::fromHex("02"), "FORMAT:UTF8,EXPLICIT:2A,PRINTABLESTRING:D");
	createFile(QByteArray::fromHex("03"), "FORMAT:UTF8,EXPLICIT:3A,NUMERICSTRING:20291031");
	createFile(QByteArray::fromHex("04"), "FORMAT:UTF8,EXPLICIT:4A,UTF8String:ERIKA");
	createFile(QByteArray::fromHex("05"), "FORMAT:UTF8,EXPLICIT:5A,UTF8String:MUSTERMANN");
	createFile(QByteArray::fromHex("06"), "FORMAT:UTF8,EXPLICIT:6A,UTF8String");
	createFile(QByteArray::fromHex("07"), "FORMAT:UTF8,EXPLICIT:7A,UTF8String");
	createFile(QByteArray::fromHex("08"), "FORMAT:UTF8,EXPLICIT:8A,NUMERICSTRING:19640812");
	createFile(QByteArray::fromHex("09"), "FORMAT:UTF8,EXPLICIT:9A,EXPLICIT:1,UTF8String:BERLIN");
	createFile(QByteArray::fromHex("0A"), "FORMAT:UTF8,EXPLICIT:10A,PRINTABLESTRING:D");
	createFile(QByteArray::fromHex("0B"), "FORMAT:UTF8,EXPLICIT:11A,PRINTABLESTRING:F");
	createFile(QByteArray::fromHex("0C"), "EXPLICIT:12A,SET:optional_data",
			QByteArrayLiteral("[optional_data]\n"
							  "v0 = SEQUENCE:seq_section\n"
							  "[seq_section]\n"
							  "v1 = OID:1.2.840.10045.1.1\n"
							  "v2 = INTEGER:0xA9FB57DBA1EEA9BC3E660A909D838D726E3BF623D52620282013481D1F6E5377\n"));
	createFile(QByteArray::fromHex("0D"), "FORMAT:UTF8,EXPLICIT:13A,UTF8String:GABLER");
	createFile(QByteArray::fromHex("0E"), "FORMAT:HEX,EXPLICIT:14A,OCTETSTRING:",
			QStringLiteral(":/images/signature.jp2"));
	createFile(QByteArray::fromHex("0F"), "FORMAT:UTF8,EXPLICIT:15A,NUMERICSTRING:20191101");
	//                             "10"   "RFU"
	createFile(QByteArray::fromHex("11"), "EXPLICIT:17A,SEQUENCE:seq_section",
			QStringLiteral(u"[seq_section]\n"
						   u"v1 = FORMAT:UTF8,EXPLICIT:10,UTF8String:HEIDESTRA\u1E9EE 17\n"
						   u"v2 = FORMAT:UTF8,EXPLICIT:11,UTF8String:K\u00D6LN\n"
						   u"v3 = FORMAT:UTF8,EXPLICIT:13,PRINTABLESTRING:D\n"
						   u"v4 = FORMAT:UTF8,EXPLICIT:14,PRINTABLESTRING:51147\n").toUtf8());
	createFile(QByteArray::fromHex("12"), "FORMAT:HEX,EXPLICIT:18A,OCTETSTRING:02760503150000");
	createFile(QByteArray::fromHex("13"), "FORMAT:UTF8,EXPLICIT:19A,EXPLICIT:1,UTF8String:RESIDENCE PERMIT 1");
	createFile(QByteArray::fromHex("14"), "FORMAT:UTF8,EXPLICIT:20A,EXPLICIT:1,UTF8String:RESIDENCE PERMIT 2");
	createFile(QByteArray::fromHex("15"), "FORMAT:UTF8,EXPLICIT:21A,PRINTABLESTRING:tel:+49-030-1234567");
	createFile(QByteArray::fromHex("16"), "FORMAT:UTF8,EXPLICIT:22A,IA5STRING:erika@mustermann.de");
}


SimulatorFileSystem::SimulatorFileSystem(const QJsonObject& pData)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
	using JsonValueRef = const QJsonValueRef;
#else
	using JsonValueRef = const QJsonValue&;
#endif

	const auto& files = pData[QLatin1String("files")].toArray();
	for (JsonValueRef value : files)
	{
		if (!value.isObject())
		{
			qCWarning(card) << "Skipping file entry. Expected JSON object, got" << value;
			continue;
		}

		const auto& file = value.toObject();
		const auto& fileId = file[QLatin1String("fileId")].toString();
		const auto& shortFileId = file[QLatin1String("shortFileId")].toString();
		const auto& content = file[QLatin1String("content")].toString();
		if (fileId.isNull() || shortFileId.isNull() || content.isNull())
		{
			qCWarning(card) << "Skipping file entry. Expected JSON object with 'fileId', 'shortFileId' and 'content', got" << file;
			continue;
		}

		createFile(QByteArray::fromHex(fileId.toUtf8()),
				QByteArray::fromHex(shortFileId.toUtf8()),
				QByteArray::fromHex(content.toUtf8()));
	}
}


StatusCode SimulatorFileSystem::select(const QByteArray& pFileId)
{
	if (mFiles.contains(pFileId))
	{
		mSelectedFile = pFileId;
		return StatusCode::SUCCESS;
	}

	if (mFileIds.contains(pFileId))
	{
		if (const auto& shortFileId = mFileIds.value(pFileId); mFiles.contains(shortFileId))
		{
			mSelectedFile = shortFileId;
			return StatusCode::SUCCESS;
		}
	}

	mSelectedFile.clear();
	return StatusCode::INVALID_DATAFIELD;
}


QByteArray SimulatorFileSystem::read(int pOffset, int pLength, bool pExtendedLen)
{
	if (pLength <= CommandApdu::NO_LE || pLength > CommandApdu::EXTENDED_MAX_LE)
	{
		return ResponseApdu(StatusCode::WRONG_LENGTH);
	}

	if (!mFiles.contains(mSelectedFile))
	{
		return ResponseApdu(StatusCode::FILE_NOT_FOUND);
	}

	const auto& file = mFiles.value(mSelectedFile);
	if (pOffset < 0 || pOffset > file.size())
	{
		return ResponseApdu(StatusCode::ILLEGAL_OFFSET);
	}

	auto result = file.mid(pOffset, pLength);

	const bool explicitLen = (pLength != (pExtendedLen ? CommandApdu::EXTENDED_MAX_LE : CommandApdu::SHORT_MAX_LE));
	if ((explicitLen || pOffset > 0) && pLength > result.size())
	{
		result += ResponseApdu(StatusCode::END_OF_FILE);
	}
	else
	{
		result += ResponseApdu(StatusCode::SUCCESS);
	}

	return result;
}


StatusCode SimulatorFileSystem::write(int pOffset, const QByteArray& pData)
{
	if (!mFiles.contains(mSelectedFile))
	{
		return StatusCode::FILE_NOT_FOUND;
	}

	if (pOffset != 0)
	{
		return StatusCode::ILLEGAL_OFFSET;
	}

	mFiles.insert(mSelectedFile, pData);
	return StatusCode::SUCCESS;
}


QByteArray SimulatorFileSystem::getEfCardAccess() const
{
	return mFiles[FileRef::efCardAccess().getShortIdentifier()];
}


QByteArray SimulatorFileSystem::getCardAuthenticationKey() const
{
	return QByteArray::fromHex("A07EB62E891DAA84643E0AFCC1AF006891B669B8F51E379477DBEAB8C987A610");
}


QByteArray SimulatorFileSystem::getRestrictedIdentificationKey(int pKeyId) const
{
	switch (pKeyId)
	{
		case 1:
			return QByteArray::fromHex("0353859C2EC67780BA39015DE8C682AF2326D43DE9CE1E07737087BD1E17CB22");

		case 2:
			return QByteArray::fromHex("9AD0AD7F4DFAAA06988339FC31D3A111F4C7964AC7F377373A2454327C43E2FF");

		default:
			return QByteArray();
	}
}


StatusCode SimulatorFileSystem::verify(const Oid& pOid, const QSharedPointer<AuthenticatedAuxiliaryData>& pAuxiliaryData)
{
	if (!pAuxiliaryData)
	{
		return StatusCode::VERIFICATION_FAILED;
	}

	qDebug() << "Checking validity of" << pOid;

	if (pOid == KnownOid::ID_DATE_OF_BIRTH)
	{
		if (pAuxiliaryData->hasAgeVerificationDate())
		{
			const auto rawDate = mFiles.value(QByteArray::fromHex("08")).right(8);
			const auto dateOfBirth = QDate::fromString(QString::fromUtf8(rawDate), QStringLiteral("yyyyMMdd"));
			const auto requiredDate = pAuxiliaryData->getAgeVerificationDate();
			return requiredDate < dateOfBirth ? StatusCode::VERIFICATION_FAILED : StatusCode::SUCCESS;
		}
	}
	else if (pOid == KnownOid::ID_DATE_OF_EXPIRY)
	{
		if (pAuxiliaryData->hasValidityDate())
		{
			const auto rawDate = mFiles.value(QByteArray::fromHex("03")).right(8);
			const auto dateOfExpiry = QDate::fromString(QString::fromUtf8(rawDate), QStringLiteral("yyyyMMdd"));
			const auto validityDate = pAuxiliaryData->getValidityDate();
			return validityDate > dateOfExpiry ? StatusCode::VERIFICATION_FAILED : StatusCode::SUCCESS;
		}
	}
	else if (pOid == KnownOid::ID_MUNICIPALITY_ID)
	{
		if (pAuxiliaryData->hasCommunityID())
		{
			const auto communityID = mFiles.value(QByteArray::fromHex("12")).mid(4).toHex();
			return communityID.startsWith(pAuxiliaryData->getCommunityID()) ? StatusCode::SUCCESS : StatusCode::VERIFICATION_FAILED;
		}
	}

	return StatusCode::VERIFICATION_FAILED;
}


void SimulatorFileSystem::createFile(const QByteArray& pFileId, const QByteArray& pShortFileId, const QByteArray& pContent)
{
	mFileIds.insert(pFileId, pShortFileId);
	mFiles.insert(pShortFileId, pContent);
}


void SimulatorFileSystem::createFile(const QByteArray& pShortFileId, const QByteArray& pContent)
{
	createFile(QByteArray::fromHex("01") + pShortFileId, pShortFileId, pContent);
}


void SimulatorFileSystem::createFile(const QByteArray& pShortFileId, const char* pStr, const QByteArray& pConfig)
{
	ERR_clear_error();

	QSharedPointer<CONF> config;
	if (!pConfig.isEmpty())
	{
		QSharedPointer<BIO> bio(BIO_new(BIO_s_mem()), [](BIO* pB){BIO_free(pB);});
		BIO_puts(bio.data(), pConfig.constData());

		long error_line = 0;
		config.reset(NCONF_new(nullptr), [](CONF* pC){NCONF_free(pC);});
		if (NCONF_load_bio(config.data(), bio.data(), &error_line) == 0)
		{
			qCWarning(card) << "Cannot load ASN.1 config. Error on line" << error_line << "." << getOpenSslError();
		}
	}

	ASN1_TYPE* type = ASN1_generate_nconf(pStr, config.data());
	if (!type)
	{
		qCWarning(card) << "Cannot generate ASN.1 object:" << getOpenSslError();
		return;
	}

	createFile(pShortFileId, Asn1TypeUtil::encode(type));
	ASN1_TYPE_free(type);
}


void SimulatorFileSystem::createFile(const QByteArray& pShortFileId, const char* pStr, const QString& pFile)
{
	if (QFile inputFile(pFile); inputFile.open(QIODevice::ReadOnly))
	{
		QByteArray image(pStr);
		image += inputFile.readAll().toHex();
		createFile(pShortFileId, image.data());
		inputFile.close();
	}
}
