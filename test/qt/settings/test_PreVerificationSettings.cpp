/*!
 * \brief Unit tests for \ref PreVerificationSettings
 *
 * \copyright Copyright (c) 2014-2022 Governikus GmbH & Co. KG, Germany
 */

#include "PreVerificationSettings.h"

#include "ResourceLoader.h"
#include "TestFileHelper.h"
#include "asn1/CVCertificate.h"

#include <QFile>
#include <QtTest>


using namespace governikus;


class test_PreVerificationSettings
	: public QObject
{
	Q_OBJECT

	QByteArrayList cvcs;

	private Q_SLOTS:
		void initTestCase()
		{
			ResourceLoader::getInstance().init();

			cvcs.append(TestFileHelper::readFile(":/card/cvca-DETESTeID00002_DETESTeID00001.hex", true));
			cvcs.append(TestFileHelper::readFile(":/card/cvca-DETESTeID00004_DETESTeID00002.hex", true));
			cvcs.append(TestFileHelper::readFile(":/card/cvdv-DEDVeIDDPST00035.hex", true));
			cvcs.append(TestFileHelper::readFile(":/card/cvat-DEDEMODEV00038.hex", true));
		}


		void init()
		{
			AbstractSettings::mTestDir.clear();
		}


		void testAdd()
		{
			PreVerificationSettings settings;

			settings.addLinkCertificate(cvcs.at(0));
			settings.addLinkCertificate(cvcs.at(1));
			settings.addLinkCertificate(cvcs.at(2));
			settings.addLinkCertificate(cvcs.at(3));
			QCOMPARE(settings.getLinkCertificates().size(), 4);
			settings.save();

			QFile testFile(settings.mStore->fileName());
			QVERIFY(testFile.exists());
			QVERIFY(testFile.open(QIODevice::ReadOnly | QIODevice::Text));
			QCOMPARE(testFile.readAll(), QByteArray("[preverification]\nlinkcertificates\\1\\linkcertificate=@ByteArray(7f218201b67f4e82016e5f290100420e44455445535465494430303030317f4982011d060a04007f000702020202038120a9fb57dba1eea9bc3e660a909d838d726e3bf623d52620282013481d1f6e537782207d5a0975fc2c3057eef67530417affe7fb8055c126dc5c6ce94a4b44f330b5d9832026dc5c6ce94a4b44f330b5d9bbd77cbf958416295cf7e1ce6bccdc18ff8c07b68441048bd2aeb9cb7e57cb2c4b482ffc81b7afb9de27e1e3bd23c23a4453bd9ace3262547ef835c3dac4fd97f8461a14611dc9c27745132ded8e545c1d54c72f0469978520a9fb57dba1eea9bc3e660a909d838d718c397aa3b561a6f7901e0e82974856a7864104096eb58bfd86252238ec2652185c43c3a56c320681a21e37a8e69ddc387c0c5f5513856efe2fdc656e604893212e29449b365e304605ac5413e75be31e641f128701015f200e44455445535465494430303030327f4c12060904007f0007030102025305fe0f01ffff5f25060100000902015f24060103000902015f3740141120a0fdfc011a52f3f72b387a3dc7aca88b4868d5ae9741780b6ff8a0b49e5f55169a2d298ef5cf95935dca0c3df3e9d42dc45f74f2066317154961e6c746)\nlinkcertificates\\2\\linkcertificate=@ByteArray(7f218201b67f4e82016e5f290100420e44455445535465494430303030327f4982011d060a04007f000702020202038120a9fb57dba1eea9bc3e660a909d838d726e3bf623d52620282013481d1f6e537782207d5a0975fc2c3057eef67530417affe7fb8055c126dc5c6ce94a4b44f330b5d9832026dc5c6ce94a4b44f330b5d9bbd77cbf958416295cf7e1ce6bccdc18ff8c07b68441048bd2aeb9cb7e57cb2c4b482ffc81b7afb9de27e1e3bd23c23a4453bd9ace3262547ef835c3dac4fd97f8461a14611dc9c27745132ded8e545c1d54c72f0469978520a9fb57dba1eea9bc3e660a909d838d718c397aa3b561a6f7901e0e82974856a786410474ff63ab838c73c303ac003dfee95cf8bf55f91e8febcb7395d942036e47cf1845ec786ec95bb453aac288ad023b6067913cf9b63f908f49304e5cfc8b3050dd8701015f200e44455445535465494430303030347f4c12060904007f0007030102025305fc0f13ffff5f25060102000501015f24060105000501015f37405c035a0611b6c58f0b5261fdd009decab7dc7a79482d5248cca119059b7d82b2157cf0c4a499bcf441efdd35e294a58c0af19a34a0762159533285acf170a505)\nlinkcertificates\\3\\linkcertificate=@ByteArray(7f2181e77f4e81a05f290100420e44455445535465494430303030347f494f060a04007f000702020202038641045b1cb1090d5064fe0aee21bd95c062ab94c952f7d64274eaf004c2e3da4abfda2d0108b2545ceaf8baf40bdea1d161be8950b3353bfd267f0674ec9acaba71d05f2010444544566549444450535430303033357f4c12060904007f0007030102025305400513ff875f25060104000201015f24060104000501025f37409389856b8da9956ba3e9894812ba87f866646660557131ef618349bf145a0826a8fb4a9be22589cde868b074c3fda73db84f9fdc84b87b3896702e42b4fe86e7)\nlinkcertificates\\4\\linkcertificate=@ByteArray(7f218201487f4e8201005f2901004210444544566549444450535430303033357f494f060a04007f0007020202020386410400c7dedb2117e45acf998e9d3ed34883e0617d1614b60430ca1df1d2ecc96bc214d97451588ef706deaf7f68163f7c8eaadf9ea028f0f8bf5d0dd67b650907175f200e444544454d4f44455630303033387f4c12060904007f0007030102025305000513ff875f25060104000301035f2406010400040101655e732d060904007f000703010301802012ca9d0a51df9297eaba7ebe9ab49df2f4cf83e0dbb02772efad89c8ad75fccd732d060904007f0007030103028020cb1e1940159f11dc96845b87c23b86f9baa755a789a914bbd5b8fa9784019d1c5f37407ab2b3c8de4b3f7136f7da91ccac25b26aec5bc35ad0b603fa2ffe50cea49f785614ad3fb2eff1719971fbcabc95f95a9f601f9017bd73952b45e645b90b774f)\nlinkcertificates\\size=4\n"));
		}


		void testAddAlreadyContainedCert()
		{
			PreVerificationSettings settings;

			settings.addLinkCertificate(cvcs.at(0));
			settings.addLinkCertificate(cvcs.at(0));
			QCOMPARE(settings.getLinkCertificates().size(), 1);
			settings.save();

			QFile testFile(settings.mStore->fileName());
			QVERIFY(testFile.exists());
			QVERIFY(testFile.open(QIODevice::ReadOnly | QIODevice::Text));
			QCOMPARE(testFile.readAll(), QByteArray("[preverification]\nlinkcertificates\\1\\linkcertificate=@ByteArray(7f218201b67f4e82016e5f290100420e44455445535465494430303030317f4982011d060a04007f000702020202038120a9fb57dba1eea9bc3e660a909d838d726e3bf623d52620282013481d1f6e537782207d5a0975fc2c3057eef67530417affe7fb8055c126dc5c6ce94a4b44f330b5d9832026dc5c6ce94a4b44f330b5d9bbd77cbf958416295cf7e1ce6bccdc18ff8c07b68441048bd2aeb9cb7e57cb2c4b482ffc81b7afb9de27e1e3bd23c23a4453bd9ace3262547ef835c3dac4fd97f8461a14611dc9c27745132ded8e545c1d54c72f0469978520a9fb57dba1eea9bc3e660a909d838d718c397aa3b561a6f7901e0e82974856a7864104096eb58bfd86252238ec2652185c43c3a56c320681a21e37a8e69ddc387c0c5f5513856efe2fdc656e604893212e29449b365e304605ac5413e75be31e641f128701015f200e44455445535465494430303030327f4c12060904007f0007030102025305fe0f01ffff5f25060100000902015f24060103000902015f3740141120a0fdfc011a52f3f72b387a3dc7aca88b4868d5ae9741780b6ff8a0b49e5f55169a2d298ef5cf95935dca0c3df3e9d42dc45f74f2066317154961e6c746)\nlinkcertificates\\size=1\n"));
		}


		void testGetCerts()
		{
			PreVerificationSettings settings;
			settings.addLinkCertificate(cvcs.at(0));
			settings.addLinkCertificate(cvcs.at(1));
			settings.addLinkCertificate(cvcs.at(2));
			settings.addLinkCertificate(cvcs.at(3));
			QVERIFY(settings.getLinkCertificates().contains(cvcs.at(0)));
			QVERIFY(settings.getLinkCertificates().contains(cvcs.at(1)));
			QVERIFY(settings.getLinkCertificates().contains(cvcs.at(2)));
			QVERIFY(settings.getLinkCertificates().contains(cvcs.at(3)));
		}


		void testRemoveCert()
		{
			PreVerificationSettings settings;
			settings.addLinkCertificate(cvcs.at(0));
			settings.addLinkCertificate(cvcs.at(1));
			settings.save();

			settings.removeLinkCertificate(cvcs.at(0));
			QCOMPARE(settings.getLinkCertificates().size(), 1);
			settings.save();

			QFile testFile(settings.mStore->fileName());
			QVERIFY(testFile.exists());
			QVERIFY(testFile.open(QIODevice::ReadOnly | QIODevice::Text));
			QCOMPARE(testFile.readAll(), QByteArray("[preverification]\nlinkcertificates\\1\\linkcertificate=@ByteArray(7f218201b67f4e82016e5f290100420e44455445535465494430303030327f4982011d060a04007f000702020202038120a9fb57dba1eea9bc3e660a909d838d726e3bf623d52620282013481d1f6e537782207d5a0975fc2c3057eef67530417affe7fb8055c126dc5c6ce94a4b44f330b5d9832026dc5c6ce94a4b44f330b5d9bbd77cbf958416295cf7e1ce6bccdc18ff8c07b68441048bd2aeb9cb7e57cb2c4b482ffc81b7afb9de27e1e3bd23c23a4453bd9ace3262547ef835c3dac4fd97f8461a14611dc9c27745132ded8e545c1d54c72f0469978520a9fb57dba1eea9bc3e660a909d838d718c397aa3b561a6f7901e0e82974856a786410474ff63ab838c73c303ac003dfee95cf8bf55f91e8febcb7395d942036e47cf1845ec786ec95bb453aac288ad023b6067913cf9b63f908f49304e5cfc8b3050dd8701015f200e44455445535465494430303030347f4c12060904007f0007030102025305fc0f13ffff5f25060102000501015f24060105000501015f37405c035a0611b6c58f0b5261fdd009decab7dc7a79482d5248cca119059b7d82b2157cf0c4a499bcf441efdd35e294a58c0af19a34a0762159533285acf170a505)\nlinkcertificates\\size=1\n"));
		}


		void testRemoveAlreadyRemovedCert()
		{
			PreVerificationSettings settings;
			settings.addLinkCertificate(cvcs.at(0));
			settings.addLinkCertificate(cvcs.at(1));

			settings.removeLinkCertificate(cvcs.at(0));
			settings.removeLinkCertificate(cvcs.at(0));
			QCOMPARE(settings.getLinkCertificates().size(), 1);
		}


		void testEnabled()
		{
			PreVerificationSettings settings;
			QVERIFY(settings.isEnabled());
			settings.setEnabled(false);
			QVERIFY(!settings.isEnabled());
		}


};

QTEST_GUILESS_MAIN(test_PreVerificationSettings)
#include "test_PreVerificationSettings.moc"
