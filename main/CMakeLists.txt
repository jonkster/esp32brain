idf_component_register(
	SRC_DIRS "."
	INCLUDE_DIRS "." "include"
	PRIV_REQUIRES "console driver vfs esp_driver_uart esp_app_format nvs_flash esp_psram esp_wifi esp-tls json"
)

# updates the version.txt file based on git hash each compile
add_custom_command(
	OUTPUT "version.txt"
	COMMAND echo "␛[32m------JK Custom build stuff: version.txt-------␛[0m"
	COMMAND git describe --dirty --tags > "${PROJECT_DIR}/version.txt"
	COMMENT "UPDATING VERSION"
	VERBATIM
)
add_custom_target(version DEPENDS version.txt)
add_dependencies(${COMPONENT_LIB} version)
set_property(
	DIRECTORY "${COMPONENT_DIR}"
	APPEND PROPERTY ADDITIONAL_CLEAN_FILES version.txt
)

