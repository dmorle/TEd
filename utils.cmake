function(merge_dir DST SRC)
	string(REPLACE "\\" "/" SRC "${SRC}")
	string(REPLACE "\\" "/" DST "${DST}")
	if (WIN32)
		execute_process(
			COMMAND "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe"
			"-ExecutionPolicy" "Bypass" "-File" "${PROJECT_SOURCE_DIR}/merge_dir.ps1"
			"${DST}" "${SRC}" RESULT_VARIABLE RESULT
		)
	else ()
		message(FATAL_ERROR "This script currently only supports Windows")
	endif ()
	string(COMPARE NOTEQUAL "${RESULT}" "0" LS_FAILED)
	if ("${LS_FAILED}")
		message(FATAL_ERROR "ls failed with exit code ${RESULT}\nstderr: ${STDERR}")
	endif ()
endfunction()
