if(Qt STREQUAL "Qt5")
	set(QT5 ON)
	set(QT_VERSION "${Qt5Core_VERSION}")
	set(QT_HOST_PREFIX "${_qt5Core_install_prefix}")

elseif(Qt STREQUAL "Qt6")
	set(QT6 ON)
	set(QT_VERSION "${Qt6Core_VERSION}")
	set(QT_HOST_PREFIX "${_qt6Core_install_prefix}")

else()
	message(FATAL_ERROR "Qt version not supported: ${Qt}")
endif()

if(QT5)
	if(NOT COMMAND qt_add_binary_resources)
		macro(qt_add_binary_resources)
			qt5_add_binary_resources(${ARGV})
		endmacro()
	endif()

	if(NOT COMMAND qt_add_resources)
		macro(qt_add_resources)
			qt5_add_resources(${ARGV})
		endmacro()
	endif()

	if(NOT COMMAND qt_add_translation)
		macro(qt_add_translation)
			qt5_add_translation(${ARGV})
		endmacro()
	endif()

	if(NOT COMMAND qt_create_translation)
		macro(qt_create_translation)
			qt5_create_translation(${ARGV})
		endmacro()
	endif()
endif()