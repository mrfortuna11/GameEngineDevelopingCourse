file(GLOB 
	PRIVATE_FILES 
	private/*.cpp 
	private/*.h
)
file(GLOB 
	PUBLIC_FILES 
	public/*.h
)

include(GenerateExportHeader)
add_library(Audio SHARED)
generate_export_header(Audio EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME AUDIO_API)

set_target_properties(Audio PROPERTIES FOLDER "Engine")

target_compile_definitions(Audio 
	PRIVATE 
		AUDIO_EXPORTS 
		UNICODE)

target_sources(Audio
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

target_include_directories(Audio
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
    	PRIVATE ${CMAKE_LIBRARY_PATH}/fmod/api/core/inc
)


find_library(FMOD_LIB NAMES fmod_vc
    PATHS ${CMAKE_LIBRARY_PATH}/fmod/api/core/lib/x64
    NO_DEFAULT_PATH)
set(FMOD_LIBRARIES ${FMOD_LIB})


target_link_libraries(Audio PRIVATE
    ${FMOD_LIBRARIES}
)

