project(spatiumbadge)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( spatiumbadge spatiumbadge spatiumbadge.cpp )
target_include_directories( spatiumbadge PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( spatiumbadge ${CMAKE_SOURCE_DIR}/../ricardian )