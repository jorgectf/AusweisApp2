import common.Build

def j = new Build
	(
		name: 'MacOS',
		libraries: ['MacOS'],
		label: 'MacOS',
		xunit: true
	).generate(this)


j.with
{
	steps
	{
		shell('security unlock-keychain ${KEYCHAIN_CREDENTIALS} ${HOME}/Library/Keychains/login.keychain-db')

		shell('cd source; cmake --preset ci-macos')

		shell('''\
			export DYLD_FRAMEWORK_PATH=${WORKSPACE}/libs/build/dist/lib
			export DYLD_LIBRARY_PATH=${WORKSPACE}/libs/build/dist/lib
			cmake --build build
			'''.stripIndent().trim())

		shell('''\
			export DYLD_FRAMEWORK_PATH=${WORKSPACE}/libs/build/dist/lib
			export DYLD_LIBRARY_PATH=${WORKSPACE}/libs/build/dist/lib
			export QT_PLUGIN_PATH=${WORKSPACE}/libs/build/dist/plugins
			export QML2_IMPORT_PATH=${WORKSPACE}/libs/build/dist/qml
			ctest -C Debug --test-dir build --output-on-failure
			'''.stripIndent().trim())
	}
}
