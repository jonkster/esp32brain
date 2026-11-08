#/bin/sh

rm app-flash_args
rm -fr bootloader
rm -fr build
rm bootloader-flash_args
rm -fr bootloader-prefix
rm CMakeCache.txt
rm -fr CMakeFiles
rm cmake_install.cmake
rm compile_commands.json
rm -fr config
rm config.env
rm -fr esp-idf
rm dependencies.lock
rm flash_args
rm flash_args.in
rm -fr gdbinit
rm kconfigs.in
rm kconfigs_projbuild.in
rm Makefile
rm -fr managed_components
rm -fr partition_table
rm -fr specs
rm -fr toolchain
rm partition-table-flash_args
rm project_description.json
rm component_manager_run_counter.*
rm flash_*
rm flasher_args.json
rm ldgen_libraries
rm ldgen_libraries.in
rm sdkconfig.old

rm .bin_timestamp
rm project_elf_src_esp32.c
rm switch.bin
rm switch.elf
rm switch.map
rm x509_crt_bundle.S

rm app-flash_args.in
rm components_with_manifests_list.temp.overrides
rm sdkconfig
rm flasher_args.json.in
rm hints.yml

rm *.bin
rm *.c
rm *.elf
rm *.map


echo ========= REMEMBER =========================
echo to set up environment:
echo        eim fix
echo
echo then to activate the environment:
echo        'source "/home/jonk/.espressif/tools/activate_idf_v6.1.sh"   #(or whichever version)'
echo
echo  then set the target, eg:
echo        "idf.py set-target esp32p4   #(or whichever version)"
echo
echo  then
echo       cmake .
echo ============================================

