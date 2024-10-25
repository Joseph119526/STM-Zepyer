add_subdirectory(src)
add_subdirectory(ports)

zephyr_include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/ports/zephyr
	)